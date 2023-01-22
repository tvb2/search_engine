#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H
#include <vector>
#include <iostream>
#include <string>
#include <map>
#include <filesystem>
#include <fstream>
#include "entry.h"

class InvertedIndex{
private:
	std::vector<std::string> docs;
	std::map<std::string, std::vector<Entry>> index;
	std::vector<std::string> extensions {".txt", ".doc"};
	std::map<std::filesystem::path,int> files;

public:
		InvertedIndex();

	/**
	Обновить или заполнить базу документов, по которой будем совершать
	поиск @param texts_input содержимое документов
	*/
	void UpdateDocumentBase();

	/**
	 Perform one file indexation
	 */
	//void indexDB(std::string const &doc, size_t fileNum);
	void indexFile(size_t fileNum);

	/**
	 Perform database indexation
	 * */
	 void indexDB();

	/**
	 Search dir for files to be indexed. File extensions are stored in extensions vector
	 */
	void getFilesToIndex();


	/**
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> GetWordCount(const std::string& word);


	/**
	 Print index
	*/
	void printIndex(){
		for (auto ind:index){
		std::cout<<"word " << ind.first <<":\n";
		for (auto vec:ind.second){
			std::cout << "doc id: " <<vec.doc_id<< ", count: " << vec.count <<"\n";
		}
	}
	}

};



#endif //SEARCH_ENGINE_INVERTEDINDEX_H
