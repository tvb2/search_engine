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
	 Calculate Absolute index for a given request
	 */
	void absRelevant(const std::vector<std::string>& queries_input, std::map<size_t,int> &absRelev);

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<RelativeIndex> search(const
	                                               std::vector<std::string>& queries_input);

	void printResult();

private:
	InvertedIndex _index;
	std::vector<RelativeIndex> result;
};


#endif //SEARCH_ENGINE_SEARCHSERVER_H
