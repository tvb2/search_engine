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

	std::vector<std::string> docs;
	docs = json.getTextDocuments();
	std::vector<std::string> requests;
	requests = json.getRequests();
	std::vector<std::vector<RelativeIndex>> relInd;
	relInd = server.search(requests);
	json.putAnswers(relInd);

	std::string command = "exit";

//	json.getFileList();

	bool needUpdate = false;
	std::thread monitor(&ConverterJSON::indexingRequired, json, std::ref(needUpdate));
	monitor.detach();

 while (true) {
	 std::cout<<"'index', 'search', 'exit' to exit the program\n";
	 std::cin >> command;
	 if (command == "index") {
		 std::cout<<"started indexing the files\n";
		 index.indexDB();
		 std::cout << "Task complete..\n";
	 }
	 else if (command == "search"){
		 std::cout<<"enter line to search: \n";
		 std::string searchRequest;
//		 std::getline (std::cin, searchRequest);

		 std::vector<std::string> request{
			 {"war and piece"},
			 {"nuclear powerstation"},
			 {"random abracadabra"},
			 {"villages of russian people"}};
//		 std::vector<std::vector<RelativeIndex>> relInd;
//		 relInd = server.search(request);
	 }
	 else if (command == "exit")
		 break;
	 else
		 std::cout<<"Wrong command!\n";
    if (needUpdate)
		index.indexDB();
 }
	std::cout << "Program complete..\n";

}
