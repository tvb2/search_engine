#include <iostream>
#include <converterjson.h>
#include "invertedindex.h"

#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <map>
#include <thread>

#include <boost/thread.hpp>


int main () {
	std::cout<<"Program started..\n";
	
	InvertedIndex index;


auto start = std::chrono::high_resolution_clock::now();

//for (size_t i = 0; i < index.files.size(); ++i )
for (size_t i = 0; i < 200; ++i )

	index.indexDB(index.docs[i],i);

auto stop = std::chrono::high_resolution_clock::now();

 std::chrono::duration<float> duration =  stop - start;
std::cout<<"duration: " << duration.count() <<"\n";

	//index.printIndex();
	
	std::cout<<"Program complete..\n";

}
