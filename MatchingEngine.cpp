#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include<algorithm>

double MatchingEngine::lastTradedPrice = 0;
long MatchingEngine::lastTradedQty = 0;

void MatchingEngine::addIntoBuyQueue(const Order& order)
{
	// If the order action type is Add / A and it is not found in the activeOrderSet then add it.
	if(order.getActionType() == 'A' )
	{
		if(m_activeOrderSet.find(order.getOrderId()) == m_activeOrderSet.end())
		{
			m_BuyOrderQueue.push_back(order);
			std::sort(m_BuyOrderQueue.begin(), m_BuyOrderQueue.end(), greater<Order>());
			m_activeOrderSet.insert(order.getOrderId());
		}
		else
		{
			std::string orderMessage = order.getOrderMessage();
			pushInBadOrder(orderMessage, "Duplicate Add action for the order when order already exists");
		}
	}

	// If order action is remove.
	if(order.getActionType() == 'X')
	{
		auto iterPos = m_activeOrderSet.find(order.getOrderId());
		if(iterPos != m_activeOrderSet.end())
		{
			auto iterFind = find(m_BuyOrderQueue.begin(), m_BuyOrderQueue.end(), order);
			auto sizePos = std::distance(m_BuyOrderQueue.begin(), iterFind);
			m_BuyOrderQueue.erase(m_BuyOrderQueue.begin()+sizePos);
			m_activeOrderSet.erase(iterPos);
		}
	}

	// If order action is Modify.
	if(order.getActionType() == 'M')
	{
		if(m_activeOrderSet.find(order.getOrderId()) != m_activeOrderSet.end())
		{
			auto iterFind = find(m_BuyOrderQueue.begin(), m_BuyOrderQueue.end(), order);
			auto sizePos = std::distance(m_BuyOrderQueue.begin(), iterFind);
			m_BuyOrderQueue[sizePos] = order;
		}
	}
}

void MatchingEngine::addIntoSellQueue(const Order& order)
{
	// If the order action type is Add / A and it is not found in the activeOrderSet then add it.
	if(order.getActionType() == 'A' )
	{
		if(m_activeOrderSet.find(order.getOrderId()) == m_activeOrderSet.end())
		{
			m_SellOrderQueue.push_back(order);
			std::sort(m_SellOrderQueue.begin(), m_SellOrderQueue.end(), less<Order>());
			m_activeOrderSet.insert(order.getOrderId());
		}
		else
		{
			std::string orderMessage = order.getOrderMessage();
			pushInBadOrder(orderMessage, "Duplicate Add action for the order when order already exists");
		}
	}

	// If order action is remove.
	if(order.getActionType() == 'X')
	{
		auto iterPos = m_activeOrderSet.find(order.getOrderId());
		if(iterPos != m_activeOrderSet.end())
		{
			auto iterFind = find(m_SellOrderQueue.begin(), m_SellOrderQueue.end(), order);
			auto sizePos = std::distance(m_SellOrderQueue.begin(), iterFind);
			m_SellOrderQueue.erase(m_SellOrderQueue.begin()+sizePos);
			m_activeOrderSet.erase(iterPos);
		}
	}
	// If order action is Modify.
	if(order.getActionType() == 'M')
	{
		if(m_activeOrderSet.find(order.getOrderId()) != m_activeOrderSet.end())
		{
			auto iterFind = find(m_SellOrderQueue.begin(), m_SellOrderQueue.end(), order);
			auto sizePos = std::distance(m_SellOrderQueue.begin(), iterFind);
			m_SellOrderQueue[sizePos] = order;
		}
	}
}

void MatchingEngine::displayBuyQueue()
{
	for(const auto& iter : m_BuyOrderQueue)
		iter.show();
}

void MatchingEngine::displaySellQueue()
{
	for(const auto& iter : m_SellOrderQueue)
		iter.show();
}

void MatchingEngine::displayOrderBook()
{
	displayBuyQueue();
	displaySellQueue();
}

void MatchingEngine::pushInBadOrder(const std::string& orderMessage, const std::string& parseError)
{
	std::string finalMessage = orderMessage + " | ";
	finalMessage += parseError;
	m_badOrders.push_back(finalMessage);
}

void MatchingEngine::displayBadOrders()
{
	for(const auto& iter : m_badOrders)
		std::cout << iter << std::endl;
}

void MatchingEngine::runMatching()
{
	// Dump some logs.
	std::cout << "RunMatching is called" << std::endl;

	// This is the core of matching engine logic.
	if(!m_SellOrderQueue.empty() && !m_BuyOrderQueue.empty())
	{
		// Identify the top price of each book. Verify if there is a match.
		Order bestBuyOrder = m_BuyOrderQueue[0];
		Order bestSellOrder = m_SellOrderQueue[0];

		double bestBuyPrice = bestBuyOrder.getOrderPrice();
		double bestSellPrice = bestSellOrder.getOrderPrice();
		double bidOfferSpread = bestSellPrice - bestBuyPrice;

		if( bidOfferSpread <= 0)
		{
			while(bidOfferSpread <= 0)
			{
				// We have definite match of either one or multiple orders from both bid and ask queues. The trade occurs at the sell price.
				// calculate the value of tradeDirection , i.e sellQuantity - buyQuantity. 
				// If +ve , the sell order is alive and buy order needs removal , 
				// if zero then both orders are complete and need removal from queue.
				// If -ve then buy Order is alive and sell order needs removal.

				long sellQty = bestSellOrder.getOrderQty();
				long buyQty = bestBuyOrder.getOrderQty();
				long tradeDirection = sellQty - buyQty;

				if(lastTradedPrice != bestSellPrice)
				{
					lastTradedPrice = bestSellPrice;
					lastTradedQty = std::min(sellQty,buyQty);	
				}	
				else
				{
					lastTradedQty += std::min(sellQty,buyQty);
				}

				std::cout << "T," << std::min(sellQty,buyQty) << ","  << lastTradedPrice << "=>" <<  lastTradedQty << "@" << lastTradedPrice << std::endl;
				if(tradeDirection == 0)
				{
					std::cout << "INFO : Closing the order ids = " << bestSellOrder.getOrderId() << " , " << bestBuyOrder.getOrderId() << std::endl;

					// First remove the orders from the tracking set.
					m_activeOrderSet.erase(m_activeOrderSet.find(m_BuyOrderQueue[0].getOrderId()));
					m_activeOrderSet.erase(m_activeOrderSet.find(m_SellOrderQueue[0].getOrderId()));

					m_BuyOrderQueue.pop_front();
					m_SellOrderQueue.pop_front();
				}
				else
				{
					if(tradeDirection>0)
					{
						std::cout << "INFO : Closing the order id = " << m_BuyOrderQueue[0].getOrderId() << std::endl;
						m_activeOrderSet.erase(m_activeOrderSet.find(m_BuyOrderQueue[0].getOrderId()));
						m_BuyOrderQueue.pop_front();
						m_SellOrderQueue[0].setOrderQty(tradeDirection);
					}
					else
					{
						std::cout << "INFO : Closing the order id = " << m_SellOrderQueue[0].getOrderId() << std::endl;
						m_activeOrderSet.erase(m_activeOrderSet.find(m_SellOrderQueue[0].getOrderId()));
						m_SellOrderQueue.pop_front();
						m_BuyOrderQueue[0].setOrderQty(abs(tradeDirection)); // Left amount is always +ve.
					}
				}

				if(!m_BuyOrderQueue.empty() && !m_SellOrderQueue.empty())
				{
					bestBuyOrder = m_BuyOrderQueue[0];
					bestSellOrder = m_SellOrderQueue[0];

					bestBuyPrice = bestBuyOrder.getOrderPrice();
					bestSellPrice = bestSellOrder.getOrderPrice();
					bidOfferSpread = bestSellPrice - bestBuyPrice;
				}
				else
					break;
			}
		}
		else
		{
			std::cout << "The best bid is = " << bestBuyPrice << " and the best ask is = " << bestSellPrice << ".No match is possible given bid offer spread is = " << bidOfferSpread << std::endl;
			return;
		}
	}
	else
	{
		if(m_SellOrderQueue.empty())
			std::cout << "The sell queue is empty" << std::endl;
		if(m_BuyOrderQueue.empty())
			std::cout << "The buy queue is empty" << std::endl;
	}
}
