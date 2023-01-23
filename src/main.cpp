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
		 std::getline (std::cin, searchRequest);

		 std::string word;
		 std::stringstream stream(searchRequest);
		 std::vector<std::string> request;
		 while (!stream.eof()) {
			 stream >> word;
			 if (word != "")
			    request.emplace_back(word);
			 word = "";
		 }
		 std::vector<RelativeIndex> relInd;
		 relInd = server.search(request);
		 server.printResult();
	 }
	 else if (command == "exit")
		 break;
	 else
		 std::cout<<"Wrong command!\n";
 }
	std::cout << "Program complete..\n";

}
