#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<set>
#include<deque>
#include<fstream>
#include<ostream>
#include "Order.h"
#include<thread>

using namespace std;

// The main MatchingEngine class.

#ifndef MatchingEngine_H
#define MatchingEngine_H

enum class outputStreamType { fileStream, consoleStream };

class MatchingEngine
{
	private:
		std::deque<Order> m_SellOrderQueue;
		std::deque<Order> m_BuyOrderQueue;
		std::vector<std::string> m_badOrders;
		std::set<long> m_activeOrderSet;
		static double m_lastTradedPrice;
		static long m_lastTradedQty;
		static long m_lastTradeNumber;
		outputStreamType m_logStreamType;
		void writeToStream(const std::string&, std::ostream&);
		std::ostream& getStreamRef();
		std::ofstream m_outputFileStream;
		std::string m_stockSymbol;
		void setOutPutLogFileStream();

	public:
        MatchingEngine(const MatchingEngine&);
        MatchingEngine& operator=(const MatchingEngine&);
		void setOutPutStream();
		void addIntoBuyQueue(const Order& order);
		void addIntoSellQueue(const Order& order);
		void pushInBadOrder(const std::string& orderMessage, const std::string& parseError);
		MatchingEngine();
		~MatchingEngine();
		void displayBadOrders();
		void runMatching();
		void displayBuyQueue();
		void displaySellQueue();
		void displayOrderBook();
		void setoutputStreamType(outputStreamType argStreamType);
		outputStreamType getoutputStreamType();		
		void writeToLog(const std::string&);
		void setStockSymbol(const std::string&);
		std::string getStockSymbol();
};

#endif
