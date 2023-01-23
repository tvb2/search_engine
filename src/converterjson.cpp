#include "converterjson.h"
#include "invertedindex.h"
#include "nlohmann/json.hpp"

#include <filesystem>
#include <map>
#include <fstream>

	ConverterJSON::ConverterJSON (InvertedIndex const &ind) {
		std::map<std::filesystem::path,int> files;
		files = ind.getFilesFromIndex();
		nlohmann::json f = nlohmann::json::array();
		for (auto item:files) {
			f.emplace_back(item.first);
		}
		for (auto item:f){
			std::cout << item <<"\n";
		}
	}

std::vector<std::string> ConverterJSON::getTextDocuments(){
	std::vector<std::string> result;
	return result;
}

	/**
	 initialize list of files for search server. Performed at server startup and when requested by user
	 * */
	void ConverterJSON::initializeFilesList(){

	}


int ConverterJSON::getResponsesLimit(){
	return 5;
}

std::vector<std::string> ConverterJSON::getRequests(){
	std::vector<std::string> result;
	return result;
}

void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>answers){

}