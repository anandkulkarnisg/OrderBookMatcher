#include<iostream>
#include<string>
#include<vector>
#include<algorithm>
#include<set>
#include<deque>
#include "Order.h"

using namespace std;

// The main MatchingEngine class.

#ifndef MatchingEngine_H
#define MatchingEngine_H

class MatchingEngine
{
	private:
		std::deque<Order> m_SellOrderQueue;
		std::deque<Order> m_BuyOrderQueue;
		std::vector<std::string> m_badOrders;
		std::set<long> m_activeOrderSet;
		static double lastTradedPrice;
		static long lastTradedQty;
		static long lastTradeNumber;

	public:

		static const char orderAddition;
        static const char orderModify;
        static const char orderRemove;

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
};

#endif
