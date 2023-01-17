#include "invertedindex.h"
#include <fstream>
#include <filesystem>
#include <iostream>
#include <map>
#define DEBUG

InvertedIndex::InvertedIndex () {
	getFilesToIndex();
	indexDB();

#ifdef DEBUG
	for (auto it:files){
		std::cout<<it<<"\n";
	}
	std::cout<<docs.size() << " docs records in the index \n";
#endif

}
/**
* Обновить или заполнить базу документов, по которой будем совершать
поиск
* @param texts_input содержимое документов
*/
void InvertedIndex::UpdateDocumentBase(std::filesystem::path const &file){
	std::string buffer;
	docs.emplace(docs.end(),buffer);
	auto it = docs.end()-1;
	std::ifstream current(file);
	if (current.is_open()) {
		while (!current.eof()) {
			std::getline(current, buffer);
			it->append(buffer);
		}
	}

}

/**
 Perform database indexation
 */
void InvertedIndex::indexDB(){
	//clear database before indexation
	docs.clear();
	for (auto file:files){
		UpdateDocumentBase(file);
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



