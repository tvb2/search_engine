#include "invertedindex.h"
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <thread>
#include <ctime>

// #define DEBUG_CONSTRUCTOR
//#define DEBUG_DBINDEX
	/**
	 * Constructor
	 * */
	InvertedIndex::InvertedIndex () {
		updateIndexDB();

	#ifdef DEBUG_CONSTRUCTOR
		for (auto it:files){
			//std::cout<<it<<"\n";
		}
		std::cout<<docs.size() << " docs records in the index \n";
		for (auto i = 0; i<docs.size(); ++i){
			std::cout<<"docs["<<i<<"]: ";
			for (size_t j = 0; j<10; ++j){
				if (j<docs[i].length())
					std::cout<<docs[i][j];
			}
			std::cout<<"\n";
		}
	#endif

	}

	/**
	* Обновить или заполнить базу документов, по которой будем совершать
	поиск
	* @param texts_input содержимое документов
	*/
	void InvertedIndex::updateDocumentBase(){
	//clear database before indexation
	docs.clear();
	docs.resize(files.size());
	auto it = files.begin();
	for (size_t i = 0; i < files.size(); ++i){
		std::string buffer;
		std::ifstream current((it++)->first);
		if (current.is_open()) {
			while (!current.eof()) {
				std::getline(current, buffer);
				docs[i].append(buffer);
			}
		}
	}
}

	std::mutex indexAccess;
	/**
	 Perform single file indexation
	 */
	void InvertedIndex::updateIndexFile(size_t fileNum){
	std::string word;
	std::stringstream stream(docs[fileNum]);
	while (!stream.eof()) {
		stream >> word;
		bool done = false;
		if (word == "")
			continue;
		if (index.find(word) == index.end()){//no instance of word yet in the index
			std::vector<Entry> temp{{fileNum,1}};
			indexAccess.lock();
				index.emplace(word,temp);
			indexAccess.unlock();
		}
		else{
			for (size_t i = 0; i< index[word].size(); ++i){
				if (index[word][i].doc_id == fileNum){
					indexAccess.lock();
						++index[word][i].count;
					indexAccess.unlock();
					done = true;
					break;
				}
			}
			if (!done){
				Entry e = {fileNum,1};
				indexAccess.lock();
					index[word].emplace_back(e);
				indexAccess.unlock();
			}
		}
		word = "";
	}
}

	/**
	 Perform database indexation
	 * */
	void InvertedIndex::updateIndexDB() {
		auto start = std::chrono::high_resolution_clock::now();
		this->setFilesToIndex();
		this->updateDocumentBase();
		size_t i = 0;
		std::vector<std::thread> th(std::thread::hardware_concurrency());
#ifndef DEBUG_DBINDEX
		for (; i < this->files.size() - th.size() + 1 && i < this->files.size(); i += th.size()) {
#endif
#ifdef DEBUG_DBINDEX
		for (; i < 10 && i < this->files.size(); i += th.size()) {
#endif
			size_t ind = i, threads = 0;
			for (size_t t = 0; t < th.size() && t < this->files.size(); ++t) {
				size_t ind = i + t;
				th[t] = std::thread{&InvertedIndex::updateIndexFile, this, std::ref(ind)};
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
				++threads;
			}
			for (size_t t = 0; t < threads; ++t) {
				th[t].join();
			}
		}
		auto stop = std::chrono::high_resolution_clock::now();
		std::chrono::duration<float> duration = stop - start;
		std::cout << "indexing duration: " << duration.count() << " seconds \n";
		std::cout << "Total: " << this->index.size() << " unique words indexed \n";
#ifdef DEBUG_DBINDEX
//		this->printIndex();
#endif
}

	/**
	 * monitor flag needUpdate and re-index database when commanded
	 * @param needUpdate
	 */
	void InvertedIndex::periodicIndexing(bool const &needUpdate, bool &indexComplete){
		while (true) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1500));
			if (needUpdate) {
				std::cout << "initiate db update\n";
				this->updateIndexDB();
				indexComplete = true;
			}
		}
	}

	//from cpp reference
	std::time_t to_timeT(std::filesystem::file_time_type const& ftime) {
		std::time_t cftime = std::chrono::system_clock::to_time_t(
				std::chrono::file_clock::to_sys(ftime));
		return cftime;
	}
	std::string to_string(std::filesystem::file_time_type const& ftime) {
		std::time_t cftime = std::chrono::system_clock::to_time_t(
				std::chrono::file_clock::to_sys(ftime));
		std::string str = std::asctime(std::localtime(&cftime));
		str.pop_back();  // rm the trailing '\n' put by `asctime`
		return str;
	}

	/**
	 Search dir for files to be indexed. File extensions are stored in extensions vector
	 */
	void InvertedIndex::setFilesToIndex() {

		std::filesystem::path path = std::filesystem::current_path();
		path /= "database";
		std::filesystem::recursive_directory_iterator it;
		for (auto extension:extensions) {
			for (auto &p: std::filesystem::recursive_directory_iterator(path)) {
				if (p.is_regular_file()) {
					if (p.path().extension() == extension) {
						//here could include logic to track new files to only index the newly found files.
						//if some files missing - need to delete all index values for this file and remove file from files list
						//Not implemented here as not required by the task.
						std::filesystem::file_time_type ftime = std::filesystem::last_write_time(p);
						std::string lastTime = to_string(ftime);
						std::cout << "File write time is " << lastTime << "\n";
						//new file
						if (files.find(p.path()) == files.end()){
							newFiles.emplace(p.path(),ftime);
							files.emplace(p.path(), ftime);
						}
						//file exists
						else {
							//file was modified since last indexing
							if (files[p.path()] != ftime) {
								newFiles[p.path()] = ftime;
								std::cout << "differences in file: " << p.path().filename() << "\n";
								files[p.path()] = ftime;
							}
						}
					}
				}
			}
		}
	}

	/**
	* return filesystem::path to the file by its id
	* @param doc_id
	* @return std::filesystem::path&
	*/
	const std::filesystem::path& InvertedIndex::getFilePath(size_t const &doc_id){
		auto it = this->files.begin();
		for (size_t i = 0; i < doc_id; ++i){
			++it;
		}
		return it->first;
	}

	/**
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> InvertedIndex::getWordCount(const std::string& word){
		//std::vector<Entry> result;
		return this->index[word];
	}