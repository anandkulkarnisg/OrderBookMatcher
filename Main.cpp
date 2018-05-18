#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include "OrderValidator.h"
#include "StockMatcher.h"
#include<fstream>

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
	{
		stream = outputStreamType::fileStream;
	}
	else
	{
		stream = outputStreamType::consoleStream;
	}

	StockMatcher matcher(symbolName , inputFile, stream);
	matcher.executeMatching();
	return(0);
}

