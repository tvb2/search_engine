#include <iostream>
#include <converterjson.h>
#include "invertedindex.h"
#include "searchserver.h"
#include "userinterface.h"
#include <string>

#include <filesystem>
#include <fstream>
#include <vector>
#include <map>
#include <thread>
#include "sstream"

int main () {
	UserInterface ui;

	std::cout<<"Program LeoT  started..\n";
	std::cout << "creating index... please wait...\n";
	InvertedIndex index;
	SearchServer server(index);
	std::cout << "index created.\n";
	ConverterJSON js(index);

	std::vector<std::string> docs;
	docs = js.getTextDocuments();
	std::vector<std::string> requests;
	requests = js.getRequests();


	std::string command = "search";

 while (true) {
	 std::cout<<"'index', 'search', 'exit' to exit the program\n";
//	 std::cin >> command;
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
		 std::vector<std::vector<RelativeIndex>> relInd;
		 relInd = server.search(request);
	 }
	 else if (command == "exit")
		 break;
	 else
		 std::cout<<"Wrong command!\n";
 }
	std::cout << "Program complete..\n";

}
