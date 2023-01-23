#include "converterjson.h"
#include "invertedindex.h"
#include "nlohmann/json.hpp"
#include "searchserver.h"

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
/*
 *to do: convert path to relative
 * std::filesystem::relative, std::filesystem::proximate
 * */
		nlohmann::json found;
		std::ifstream input ("config.json");
		input >> found;
		found["files"] = f;
		input.close();
		std::ofstream output ("config.json");
		output << std::setw(4) << found << "\n";
		output.close();
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
	//		std::cout<<"item: " << item <<"\n";
			doc.open(item);
			while (!doc.eof()) {
				std::getline(doc,buffer);
				text += buffer + " ";
	//			buffer = "";
			}
			result.emplace_back(buffer);
			doc.close();
		}
	//	for (auto i:result){
	//		for (size_t j = 0; j<10; ++j){
	//			std::cout<<i[j];
	//		}
	//		std::cout<<"\n";
	//	}
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

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void ConverterJSON::putAnswers(std::vector<std::vector<std::pair<int, float>>>answers){

	}