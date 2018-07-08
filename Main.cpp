#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include "OrderValidator.h"
#include "StockMatcher.h"
#include <fstream>
#include <thread>
#include <functional>
#include <chrono>
#include <tuple>
#include "ThreadPool.h"
#include "FeedFileHandler.h"

using namespace std;

ThreadPool pool(4);

int main(int argc, char* argv[])
{
	if(argc == 2)
	{
		FeedFileHandler handler(argv[1],pool);
		handler.process();
		exit(0);
	}

	if(argc != 4 || argc != 2)
	{
		std::cout << "Invalid Usage : Proper usage is OrderMatchingEngine [inputfile] [symbol] [F/C]" << std::endl;
		std::cout << "Invalid Usage : Proper usage is OrderMatchingEngine [feedConfigFile]" << std::endl;
		exit(1);
	}

	std::string inputFile = argv[1];
	std::string symbolName = argv[2];
	std::string streamType = argv[3];
	outputStreamType stream;

	if(streamType == "F")
		stream = outputStreamType::fileStream;
	else
		stream = outputStreamType::consoleStream;

	StockMatcher matcher(symbolName , inputFile, stream);
	auto func = std::bind(&StockMatcher::executeMatching, &matcher);
	std::vector<std::future<void>> results;
	results.emplace_back(pool.enqueue(func));
	std::cout << "Main Thread id = " << std::this_thread::get_id() << std::endl;
	for(auto&& iter : results)
		iter.get();
	return(0);
}

