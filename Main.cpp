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

using namespace std;

ThreadPool pool(4);

int main(int argc, char* argv[])
{
	if(argc != 4)
	{
		std::cout << "Invalid Usage : Proper usage is OrderMatchingEngine [inputfile] [symbol] [F/C]" << std::endl;
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
    std::this_thread::sleep_for(std::chrono::seconds(10));	
	//std::cout << "Duration = " << measure<std::chrono::milliseconds>::execution(func) << " = Milli Seconds" << std::endl;
	return(0);
}

