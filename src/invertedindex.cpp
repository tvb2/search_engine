#include "invertedindex.h"
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <mutex>
#include <thread>

// #define DEBUG_CONSTRUCTOR
#define DEBUG_DBINDEX
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
	 Perform one file indexation
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
		for (; i < files.size() - th.size() + 1; i += th.size()) {
#endif
#ifdef DEBUG_DBINDEX
		for (; i < 10; i += th.size()) {
#endif
			size_t ind = i;
			for (size_t t = 0; t < th.size(); ++t) {
				size_t ind = i + t;
				th[t] = std::thread{&InvertedIndex::updateIndexFile, this, std::ref(ind)};
				std::this_thread::sleep_for(std::chrono::milliseconds(50));
			}
			for (size_t t = 0; t < th.size(); ++t) {
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
						//Not implemented here as not required by the task.
						files.emplace(p.path(),1);
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