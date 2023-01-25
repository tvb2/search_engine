#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H
#include "invertedindex.h"
#include "searchserver.h"
#include "nlohmann/json.hpp"
#include <vector>
#include <string>

class ConverterJSON{
private:
	std::string version = "0.1";
public:
	ConverterJSON(InvertedIndex const &ind);

	nlohmann::json getConfigData();

	std::string getTimeStamp();

	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	std::vector<std::string> getTextDocuments();

	/**
	 * look at time stamp when indexing was performed last time and advise if re-indexing is needed
	 * @return
	 */
	void indexingRequired(bool &needUpdate);

	/**
	 initialize list of files for search server. Performed at server startup and when requested by user
	 * */
	nlohmann::json getFileList(InvertedIndex const &ind);

	void updateFileList();


		/**
		* Метод считывает поле max_responses для определения предельного
		* количества ответов на один запрос
		* @return
		*/
	int getResponsesLimit();

	/**
	* Метод получения запросов из файла requests.json
	* @return возвращает список запросов из файла requests.json
	*/
	std::vector<std::string> getRequests();

	std::string createName(size_t const i);

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(std::vector<std::vector<RelativeIndex>>answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
