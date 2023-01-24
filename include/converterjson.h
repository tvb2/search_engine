#ifndef SEARCH_ENGINE_CONVERTERJSON_H
#define SEARCH_ENGINE_CONVERTERJSON_H
#include "invertedindex.h"
#include "searchserver.h"
#include <vector>
#include <string>

class ConverterJSON{
public:
	ConverterJSON(InvertedIndex const &ind);

	/**
	* Метод получения содержимого файлов
	* @return Возвращает список с содержимым файлов перечисленных
	* в config.json
	*/
	std::vector<std::string> getTextDocuments();

	/**
	 initialize list of files for search server. Performed at server startup and when requested by user
	 * */
	void initializeFilesList();

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

	/**
	* Положить в файл answers.json результаты поисковых запросов
	*/
	void putAnswers(std::vector<std::vector<std::pair<int, float>>>answers);
};

#endif //SEARCH_ENGINE_CONVERTERJSON_H
