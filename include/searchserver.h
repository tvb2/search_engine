#ifndef SEARCH_ENGINE_SEARCHSERVER_H
#define SEARCH_ENGINE_SEARCHSERVER_H
#include "invertedindex.h"
#include <vector>
#include <string>
#include "relativeindex.h"

class SearchServer {
public:
	/**
	* @param idx в конструктор класса передаётся ссылка на класс
	InvertedIndex,
	* чтобы SearchServer мог узнать частоту слов встречаемых в
	запросе
	*/
	SearchServer(InvertedIndex& idx);

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<RelativeIndex> search(const
	                                               std::vector<std::string>& queries_input);
private:
	InvertedIndex _index;
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
