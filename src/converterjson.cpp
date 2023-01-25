#include "converterjson.h"
#include "invertedindex.h"
#include "nlohmann/json.hpp"
#include "searchserver.h"

#include <filesystem>
#include <map>
#include <fstream>
#include <thread>
#include <stdexcept>

//#define DEBUG_GETEXTDOCUMENTS


class ConfigMissingException : public std::exception {
public:
	const char *what() const noexcept override {
		return "config file is missing!\n";
	}
};
class WrongVersionException : public std::exception {
public:
	const char *what() const noexcept override {
		return "config.json has incorrect file version!\n";
	}
};

	ConverterJSON::ConverterJSON (InvertedIndex const &ind) {
		//get data from config.json
		nlohmann::json found = this->getConfigData();

		//check programm version and throw error if it doesn't match the latests version
		try{
			if (found["config"]["version"] != this->version)
				throw WrongVersionException();
		}
		catch (WrongVersionException &x){
			std::cerr << x.what();
		}

		//get list of files from InvertedIndex class and update config.json "files"
		found["last update"] = this->getTimeStamp(); //dBUpdateTime;
		found["files"] = this->getFileList(ind);
		std::cout<<"JSON module initialized\n";
	}

	nlohmann::json ConverterJSON::getConfigData(){
		nlohmann::json found;
		std::ifstream input;

		//throw error if config.json file is missing
		try {
			input.open("config.json");
			if (!input.is_open())
				throw ConfigMissingException();
		}
		catch(ConfigMissingException &x) {
			std::cerr << x.what();
		}
		input >> found;
		input.close();
		return found;
	}

	std::string ConverterJSON::getTimeStamp(){
		std::time_t t = std::time(nullptr);
		std::tm *local = std::localtime(&t);
		std::string dBUpdateTime;
		std::stringstream stream (dBUpdateTime);
		stream << std::put_time(local,"%y/%m/%d %H:%M:%S");
		std::getline(stream, dBUpdateTime);
		return dBUpdateTime;
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
	 * check if dB scheduled update is required. Change external bool needUpdate param when update needed
	 * @param needUpdate
	 */
	void ConverterJSON::indexingRequired(bool &needUpdate){
		while (true) {
			nlohmann::json found;
			std::ifstream input("config.json");
			input >> found;
			std::string date = found["last update"];
			int interval = found["update interval minutes"];
			std::stringstream stream(date);
			std::tm lastUpdate;
			stream >> std::get_time(&lastUpdate, "%y/%m/%d %H:%M:%S");
			std::time_t t = std::time(nullptr);
			std::tm *local = std::localtime(&t);
			if (local->tm_min - lastUpdate.tm_min > interval) {
				std::cout << "update was requested!\n";
				needUpdate = true;
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
				needUpdate = false;
			}
			else {
				std::cout << "update not required\n";
				std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			}
		}
	}

	/**
	 initialize list of files for search server. Performed at server startup and when requested by user
	 * */
	nlohmann::json ConverterJSON::getFileList(InvertedIndex const &ind){
		std::map<std::filesystem::path,int> files;
		files = ind.getFilesFromIndex();
		nlohmann::json f = nlohmann::json::array();
		for (auto item:files) {
			f.emplace_back(item.first);
		}
		return f;
	}

	void ConverterJSON::updateFileList(nlohmann::json const &files){
		std::ofstream output ("config.json");
		output << std::setw(4) << files << "\n";
		output.close();
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

	/**
	 * create name of request group
	 * @param i
	 * @return
	 */
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