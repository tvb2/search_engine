#include "searchserver.h"
#include <map>

#define DEBUG_CONSTRUCTOR

SearchServer::SearchServer(InvertedIndex& idx) : _index(idx){
#ifdef DEBUG_CONSTRUCTOR
	std::cout<<"search sever initialized!\n";
#endif
}

	/**
	 Calculate Absolute Relevant index for a given request
	 */
	void SearchServer::absRelevant(const std::vector<std::string>& queries_input,std::map<size_t,int>& relIndex){
		for (auto word:queries_input){
			if (_index.getWordCount(word).size() != 0){
				std::vector<Entry> e = _index.getWordCount(word);
				for (auto ar:e){
					relIndex[ar.doc_id] += ar.count;
				}
			}
		}
	}

	/**
	* Метод обработки поисковых запросов
	* @param queries_input поисковые запросы взятые из файла
	requests.json
	* @return возвращает отсортированный список релевантных ответов для
	заданных запросов
	*/
	std::vector<RelativeIndex> SearchServer::search(const std::vector<std::string>& queries_input){
		std::map<size_t, int> relIndex;
		absRelevant(queries_input, relIndex);
		int maxInd = 0;
		for (auto it:relIndex){
			if (it.second > maxInd)
				maxInd = it.second;
		}
		std::vector<RelativeIndex> result;
		float rank = 0;
		for (auto it:relIndex){
			rank = (float)it.second/(float)maxInd;
			RelativeIndex ri{it.first, rank};
			result.emplace_back(ri);
		}
		return result;
	}