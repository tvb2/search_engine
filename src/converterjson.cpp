#include "converterjson.h"
#include "invertedindex.h"
#include "nlohmann/json.hpp"
#include "searchserver.h"

#include <filesystem>
#include <map>
#include <fstream>

//#define DEBUG_GETEXTDOCUMENTS

	ConverterJSON::ConverterJSON (InvertedIndex const &ind) {
		std::map<std::filesystem::path,int> files;
		files = ind.getFilesFromIndex();
		nlohmann::json f = nlohmann::json::array();
		for (auto item:files) {
			f.emplace_back(item.first);
		}
		nlohmann::json found;
		std::ifstream input ("config.json");
		input >> found;
		found["files"] = f;
		input.close();
		std::ofstream output ("config.json");
		output << std::setw(4) << found << "\n";
		output.close();
		std::cout<<"JSON module initialized\n";
	}

	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	std::vector<std::string> ConverterJSON::getTextDocuments(){
		std::vector<std::string> result;
		nlohmann::json config;
		std::ifstream f("config.json");
		f >> config;
		nlohmann::json files = nlohmann::json::array();
		files = config["files"];
		std::ifstream doc;
		std::string buffer, text;
		for (auto item:files){
			doc.open(item);
			while (!doc.eof()) {
				std::getline(doc,buffer);
				text += buffer + " ";
	//			buffer = "";
			}
				result.emplace_back(buffer);
			doc.close();
		}
#ifdef DEBUG_GETEXTDOCUMENTS
		for (auto i:result){
			for (size_t j = 0; j<10; ++j){
				std::cout<<i[j];
			}
			std::cout<<"\n";
		}
#endif
		return result;
	}

	/**
	 initialize list of files for search server. Performed at server startup and when requested by user
	 * */
	void ConverterJSON::initializeFilesList(){

	}

	/**
	* Метод считывает поле max_responses для определения предельного
	* количества ответов на один запрос
	* @return
	*/
	int ConverterJSON::getResponsesLimit(){
		nlohmann::json config, lim;
		std::ifstream input("config.json");
			input >> config;
			input.close();
			for (auto it = config["config"].begin(); it != config["config"].end(); ++it) {
				if (it.key() == "max_responses")
					return it.value();
			}
		return 5;
	}

	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> ConverterJSON::getRequests(){
		std::vector<std::string> result;
		nlohmann::json requests = nlohmann::json::array();
		std::ifstream input("requests.json");
		input >> requests;
		input.close();
		for (auto it = requests["requests"].begin(); it != requests["requests"].end(); ++it) {
			result.emplace_back(it.value());
		}
		return result;
	}

	std::string ConverterJSON::createName(size_t const i) {
		std::string reqName = "request";
		if (i < 10)
			reqName += "00" + std::to_string(i+1);
		else if (i >= 10 && i < 100)
			reqName += "0" + std::to_string(i+1);
		else
			reqName += std::to_string(i+1);
		return reqName;
	}

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void ConverterJSON::putAnswers(std::vector<std::vector<RelativeIndex>>answers){
		std::ofstream answersFile("answers.json");
		nlohmann::json ans, req;
		std::string reqName;
		for (size_t i = 0; i < answers.size(); ++i){
			reqName = createName(i);
			if (answers[i][0].rank != -1) {
				req[reqName]["result"] = "true";
				nlohmann::json res;
				auto it = answers[i].rbegin();
				int count{0}, maxRes{getResponsesLimit()};
				while (it != answers[i].rend() && count < maxRes){
					res["doc_id"] = it->doc_id;
					res["rank"] = it->rank;
					req[reqName]["relevance"] += res;
					++count; ++it;
				}
			}
			else
				req[reqName]["result"] = "false";
		}
		ans["answers"] = req;
		answersFile << std::setw(4) << std::fixed << std::setprecision(4) << ans;
		answersFile.close();
	}