#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<set>
#include<deque>
#include "Order.h"
#include "MatchingEngine.h"
#include "OrderValidator.h"
#include<fstream>

using namespace std;

// The main StockMatcher class.

#ifndef StockMatcher_H
#define StockMatcher_H

class StockMatcher
{
	private:
		std::string m_symbol;	// The primary symbol of the stock that is being dealt.
		OrderValidator m_validator;	// This is the validator instance that will check the validity of buy and sell messages.
		MatchingEngine m_stockMatchEngine;	// This is the stock Matching engine instance which will continuously match and buy and sell queues.
		std::ifstream m_inputFileStream;	// This is the input file stream from which the stock prices are read until they finish.
        std::vector<std::string> m_cachedStream; // This is required as ifstream does not seem capable of thread safety and even with locks it does not work.
		void populateCacheStream();

	public:
        StockMatcher(const std::string& , const std::string& , const outputStreamType);
        StockMatcher(const StockMatcher&);
		StockMatcher& operator=(const StockMatcher&);
		void executeMatching();
};

#endif

