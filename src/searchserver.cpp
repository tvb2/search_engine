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
		result.clear();
		std::map<size_t, int> relIndex;
		absRelevant(queries_input, relIndex);
		int maxInd = 0;
		for (auto it:relIndex){
			if (it.second > maxInd)
				maxInd = it.second;
		}
		std::map<int, size_t> sortedIndex;
		for (auto it:relIndex){
			sortedIndex[it.second] = it.first;
		}
		float rank = 0;
		for (auto it:sortedIndex){
			rank = (float)it.first/(float)maxInd;
			RelativeIndex ri{it.second, rank};
			result.emplace_back(ri);
		}
		return result;
	}

	void SearchServer::printResult(){
		for (std::vector<RelativeIndex>::const_reverse_iterator it = result.crbegin(); it != result.crend(); ++it){
			std::cout << "doc: " <<it->doc_id << ", filename: " << _index.getFilePath(it->doc_id).filename() << ", rank: " << it->rank <<"\n";
		}

	}