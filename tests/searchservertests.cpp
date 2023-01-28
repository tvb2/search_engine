#include <vector>
#include <string>
#include "invertedindex.h"
#include "searchserver.h"
#include "gtest/gtest.h"
#include "entry.h"
#include <sstream>
#include <filesystem>
#include "relativeindex.h"

void TestSearchServerFunctionality(
		const std::vector<std::string>& docs,
		const std::vector<std::string>& request,
		const std::vector<std::vector<RelativeIndex>>& expected
) {
	InvertedIndex idx;
	SearchServer srv(idx);
	std::vector<std::vector<RelativeIndex>> result_raw = srv.search(request);
	std::vector<std::vector<RelativeIndex>> result;
	result.resize(result_raw.size());
	/*
	formatted result:
	expected results modified due to the fact that in my program SearchServer raw output is sorted
	from least to highest rank
	Also, limit output to 5 top results only due to the fact that im my application limiting is performed by
	JSON converter class, not by SearchServer class
	*/
for (size_t i = 0; i < result.size(); ++i){
	size_t count = 0;
	for (auto it = result_raw[i].crbegin(); it != result_raw[i].crend() && count < 5 ; ++it){
		result[i].emplace_back(*it);
		++count;
	}
}
ASSERT_EQ(result, expected);
}

TEST(TestCaseSearchServer, TestSimple) {
const std::vector<std::string> docs = {

		"milk milk milk milk water water water",
		"milk water water",
		"milk milk milk milk milk water water water water water",
		"americano cappuccino"
};
		const std::vector<std::vector<RelativeIndex>> expected = {
	/*
	expected results modified due to the fact that in my program SearchServer raw output is sorted
	from least to highest rank
	also for a null result SearchServer outputs rank -1 in my case. This is later processed by
	JSON converter
	*/
				{
						{2, 1},
						{0, 0.7},
						{1, 0.3},

				},
				{
						{0, -1}
				},

			};
	const std::vector<std::string> request = {"milk water", "sugar"};

	std::filesystem::path path = std::filesystem::current_path();
	path /= "database";
	//clear database/ before the test
	for (auto &p: std::filesystem::recursive_directory_iterator(path)){
		std::filesystem::remove(p);

	}

	path = std::filesystem::current_path();
	//create test files in the database/ folder
	for (size_t i = 0; i< docs.size(); ++i) {
		std::string test = "database\\test" + std::to_string(i) + ".txt";
		path /= test;
		std::ofstream testFile(path);
		testFile << docs[i] << " ";
		testFile.close();
		path = std::filesystem::current_path();
	}

	TestSearchServerFunctionality(docs, request, expected);

}

TEST(TestCaseSearchServer, TestTop5) {
	const std::vector<std::string> docs = {

			"london is the capital of great britain",
			"paris is the capital of france",
			"berlin is the capital of germany",
			"rome is the capital of italy",
			"madrid is the capital of spain",
			"lisboa is the capital of portugal",
			"bern is the capital of switzerland",
			"moscow is the capital of russia",
			"kiev is the capital of ukraine",
			"minsk is the capital of belarus",
			"astana is the capital of kazakhstan",
			"beijing is the capital of china",
			"tokyo is the capital of japan",
			"bangkok is the capital of thailand",
			"welcome to moscow the capital of russia the third rome",
			"amsterdam is the capital of netherlands",
			"helsinki is the capital of finland",
			"oslo is the capital of norway",
			"stockholm is the capital of sweden",
			"riga is the capital of latvia",
			"tallinn is the capital of estonia",
			"warsaw is the capital of poland",

	};
	const std::vector<std::string> request = {"moscow is the capital of russia"};
	const std::vector<std::vector<RelativeIndex>> expected = {

			{
					{19, 1},
					{6, 1},
					{18, 0.666666687},
					{17, 0.666666687},
					{16, 0.666666687}
			}
	};
	std::filesystem::path path = std::filesystem::current_path();
	path /= "database";
	//clear database/ before the test
	for (auto &p: std::filesystem::recursive_directory_iterator(path)){
		std::filesystem::remove(p);

	}

	path = std::filesystem::current_path();
	//create test files in the database/ folder
	for (size_t i = 0; i< docs.size(); ++i) {
		std::string test = "database\\test" + std::to_string(i) + ".txt";
		path /= test;
		std::ofstream testFile(path);
		testFile << docs[i] << " ";
		testFile.close();
		path = std::filesystem::current_path();
	}

	TestSearchServerFunctionality(docs, request, expected);
}