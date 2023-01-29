#include <vector>
#include <string>
#include "invertedindex.h"
#include "gtest/gtest.h"
#include "entry.h"
#include <sstream>
#include <filesystem>

void TestInvertedIndexFunctionality(
		const std::vector<std::string>& docs,
		const std::vector<std::string>& requests,
		const std::vector<std::vector<Entry>>& expected
) {
	std::vector<std::vector<Entry>> result;
	InvertedIndex idx;
	for(auto& request : requests) {
		std::vector<Entry> word_count = idx.getWordCount(request);
		result.push_back(word_count);
	}
	ASSERT_EQ(result, expected);
}

void prepareTestFiles(const std::vector<std::string> &docs){
	std::filesystem::create_directory("database");
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
}

TEST(TestCaseInvertedIndex, TestBasic) {
const std::vector<std::string> docs = {

		"london is the capital of great britain",
		"big ben is the nickname for the Great bell of the striking clock"

};
	const std::vector<std::string> requests = {"london", "the"};
	const std::vector<std::vector<Entry>> expected = {
		{
				{0, 1}
		}, {
				{0, 1}, {1, 3}
		}

};
	prepareTestFiles(docs);
	TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestBasic2) {
	const std::vector<std::string> docs = {
	/*
	 * corrected errors in the expected count and in cappuccino spelling
	 * */
			"milk milk milk milk water water water",
			"milk water water",
			"milk milk milk milk milk water water water water water",
			"americano cappuccino"

	};
	const std::vector<std::string> requests = {"milk", "water", "cappuccino"};
	const std::vector<std::vector<Entry>> expected = {

			{

					{0, 4}, {1, 1}, {2, 5}

			}, {

					{0, 3}, {1, 2}, {2, 5}

			}, {

					{3, 1}

			}
	};
	prepareTestFiles(docs);
	TestInvertedIndexFunctionality(docs, requests, expected);
}

TEST(TestCaseInvertedIndex, TestInvertedIndexMissingWord) {
	const std::vector<std::string> docs = {

			"a b c d e f g h i j k l",
			"statement"

	};
	const std::vector<std::string> requests = {"m", "statement"};
	const std::vector<std::vector<Entry>> expected = {

			{
			}, {

					{1, 1}

			}
	};
	prepareTestFiles(docs);
	TestInvertedIndexFunctionality(docs, requests, expected);
}