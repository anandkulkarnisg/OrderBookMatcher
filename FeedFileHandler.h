#include<iostream>
#include<string>
#include<fstream>
#include<boost/tokenizer.hpp>
#include "StockMatcher.h"
#include "ThreadPool.h"

using namespace std;
using namespace boost;

// The main StockMatcher class.

#ifndef	FeederFileHandler_H 
#define FeederFileHandler_H

// This class takes in a config file with file that has each row consisting of three items. First one is feed of stock quotes , second one is symbol name and third one is mode [ C-Console and F-File ].

class FeedFileHandler
{

	public:
		FeedFileHandler(const std::string& fileName, ThreadPool& pool);
		void process();

	private:
		std::string m_feedFileName;
		ThreadPool& m_pool;
		std::ifstream m_inputFileStream;
		vector<tuple<std::string, std::string, char>> m_tupleItems;		
		void buildTupleItems();
};

#endif


