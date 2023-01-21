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

size_t i = 0;
std::vector<std::thread> th(boost::thread::hardware_concurrency());

//for (; i < index.files.size()-th.size()+1; i += th.size() ){
for (; i < 100; i += th.size() ){
	//index.indexDB(index.docs[i],i);
	size_t ind = i;
	for (size_t t = 0; t < th.size(); ++t){
		size_t ind = i+t;
		th[t] = std::thread{&InvertedIndex::indexDB, &index, std::ref(ind)};
		boost::this_thread::sleep_for(boost::chrono::milliseconds(50));
	}
	for (size_t t = 0; t < th.size(); ++t){
		th[t].join();
	}

}
 


auto stop = std::chrono::high_resolution_clock::now();

 std::chrono::duration<float> duration =  stop - start;

	index.printIndex();

std::cout<<"duration: " << duration.count() <<"\n";

	std::cout<<"Program complete..\n";

}
