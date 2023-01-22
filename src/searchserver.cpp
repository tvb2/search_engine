#include "searchserver.h"
#include <map>

#define DEBUG_CONSTRUCTOR

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx){
#ifdef DEBUG_CONSTRUCTOR
	std::cout<<"search sever initialized!\n";
#endif
}

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<RelativeIndex> SearchServer::search(const std::vector<std::string>& queries_input){
		std::map<int, int> absI;
		for (auto word:queries_input){
//			if (_index.in)
		}
		std::vector<RelativeIndex> result;

		return result;
	}