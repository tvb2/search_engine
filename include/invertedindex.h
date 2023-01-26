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
	void updateDocumentBase();

	/**
	 Perform one file indexation
	 */
	//void updateIndexDB(std::string const &doc, size_t fileNum);
	void updateIndexFile(size_t fileNum);

	/**
	 Perform database indexation
	 * */
	void updateIndexDB();

	/**
	 * monitor flag needUpdate and re-index database when commanded
	 * @param needUpdate
	 */
	void periodicIndexing(bool &needUpdate, bool &indexComplete);

	/**
	 * 	 Search dir for files to be indexed. File extensions are stored in extensions vector
	 */
	void setFilesToIndex();

	/**
	*
	* @return map of std::filesystem::path with the files used in index
	*/
	const std::map<std::filesystem::path,int>& getFilesFromIndex() const {
		return files;
	}

	/**
	* Метод определяет количество вхождений слова word в загруженной базе
	документов
	* @param word слово, частоту вхождений которого необходимо определить
	* @return возвращает подготовленный список с частотой слов
	*/
	std::vector<Entry> getWordCount(const std::string& word);

	/**
	 * return filesystem::path to the file by its id
	 * @param doc_id
	 * @return std::filesystem::path
	 */
	const std::filesystem::path& getFilePath(size_t const &doc_id);

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
