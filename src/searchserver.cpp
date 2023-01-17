#include "searchserver.h"

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx){ }

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<std::vector<RelativeIndex>> SearchServer::search(const
                                               std::vector<std::string>& queries_input){
		std::vector<std::vector<RelativeIndex>> result;
		return result;
	}