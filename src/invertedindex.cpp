#include "invertedindex.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#include <vector>
#include <boost/thread.hpp>
#include <boost/bind.hpp>

 //#define DEBUG

InvertedIndex::InvertedIndex () {
	getFilesToIndex();
	UpdateDocumentBase();
	// indexDB();

#ifdef DEBUG
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
void InvertedIndex::UpdateDocumentBase(){
	//clear database before indexation
	docs.clear();
	docs.resize(files.size());
	for (size_t i = 0; i < files.size(); ++i){
		std::string buffer;
		std::ifstream current(files[i]);
		if (current.is_open()) {
			while (!current.eof()) {
				std::getline(current, buffer);
				docs[i].append(buffer);
			}
		}
	}
}

/**
 Perform database indexation
 */
boost::mutex indexAccess;
//void InvertedIndex::indexDB(std::string const &doc, size_t fileNum){
	void InvertedIndex::indexDB(size_t fileNum){
	std::string buffer, word;
	size_t i = 0;
	while (i < docs[fileNum].length()){
		if (docs[fileNum][i] != ' '){
			buffer += docs[fileNum][i];
		}
		else{
			if (buffer !=""){
				word = buffer;
				bool done = false;
				if (index.find(word) == index.end()){//no instance of word yet in the index
					Entry e = {fileNum,1};
					std::vector<Entry> temp{e};
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
			}
			word = "";
			buffer = "";
		}
		++i;
	}
	
}
/**
 Search dir for files to be indexed. File extensions are stored in extensions vector
 */
void InvertedIndex::getFilesToIndex() {

	std::filesystem::path path = std::filesystem::current_path();
	path /= "database";
	std::filesystem::recursive_directory_iterator it;

	for (auto extension:extensions) {
		for (auto &p: std::filesystem::recursive_directory_iterator(path)) {
			if (p.is_regular_file()) {
				if (p.path().extension() == extension) {
					files.emplace(files.end(),p.path());
				}
			}
		}
	}
}

/**
* Метод определяет количество вхождений слова word в загруженной базе
документов
* @param word слово, частоту вхождений которого необходимо определить
* @return возвращает подготовленный список с частотой слов
*/
std::vector<Entry> InvertedIndex::GetWordCount(const std::string& word){
	std::vector<Entry> result;
	return result;
}



