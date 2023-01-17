#ifndef SEARCH_ENGINE_INVERTEDINDEX_H
#define SEARCH_ENGINE_INVERTEDINDEX_H
#include <vector>
#include <string>
#include <map>
#include <filesystem>
#include "entry.h"

class InvertedIndex{
private:
	std::vector<std::string> docs;
	std::map<std::string, std::vector<Entry>> freq_dictionary;
	std::vector<std::string> extensions {".txt", ".doc"};
	std::vector<std::filesystem::path> files;
public:
		InvertedIndex();

	/**
	Обновить или заполнить базу документов, по которой будем совершать
	поиск @param texts_input содержимое документов
	*/
	void UpdateDocumentBase(std::filesystem::path const &file);

	/**
	 Perform database indexation
	 */
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

};



#endif //SEARCH_ENGINE_INVERTEDINDEX_H
