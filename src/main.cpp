#include <iostream>
#include <converterjson.h>
#include "invertedindex.h"
#include "searchserver.h"
#include <string>

#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include "sstream"

int main () {

	std::cout<<"Program LeoT  started..\n";
	std::cout << "creating index... please wait...\n";
	InvertedIndex index;
	SearchServer server(index);
	std::cout << "index created.\n";
	ConverterJSON json(index);

	/**
	*run the search for the requests from requests.json file. Put results in the answers.json file
	 */
	std::vector<std::string> requests = json.getRequests();
	std::vector<std::vector<RelativeIndex>> relInd = server.search(requests);
	json.putAnswers(relInd);
	server.printResult();

	std::string command = "exit";

	/**
	 * start background check of file database to be indexed. Run it in a detached thread. If time since last update
	 * exceeds threshold defined in config.json - perform indexing of the files
	 */
	bool needUpdate = false;
	bool indexComplete = false;
	std::thread monitor(&ConverterJSON::indexingRequired, json, std::ref(needUpdate));
	std::thread updateIndex(&InvertedIndex::periodicIndexing, index, std::ref(needUpdate), std::ref(indexComplete));
//	std::thread updateJSON(&ConverterJSON::periodicIndexing, json, std::ref(indexComplete));
	monitor.detach();
	updateIndex.detach();
//	updateJSON.detach();

	/**
	 * run search by command from terminal. Search requests are taken from requests.json
	 */
 while (true) {
	std::cout<<"'index', 'search', 'exit' to exit the program\n";
	std::cin >> command;
	if (command == "index") {
		 std::cout << "updating index... please wait...\n";
		index.updateIndexDB();
		 std::cout << "Task complete..\n";
	 }
	else if (command == "search"){
		 std::vector<std::string> request;
		 requests = json.getRequests();
		 std::vector<std::vector<RelativeIndex>> result = server.search(requests);
		 server.printResult();
	 }
	else if (command == "exit") {
		 break;
	 }
	else {
		 std::cout << "Wrong command!\n";
	 }
//	if (needUpdate) {
//		index.updateIndexDB();
//		json.getFileList(index);
//		json.updateFileList(index);
//		needUpdate = false;
//    }
 }
	std::cout << "Program complete..\n";

}
