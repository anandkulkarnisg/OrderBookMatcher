#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include<algorithm>
#include<cstdlib>

double MatchingEngine::m_lastTradedPrice = 0;
long MatchingEngine::m_lastTradedQty = 0;
long MatchingEngine::m_lastTradeNumber = 1543267;

MatchingEngine::MatchingEngine() : m_logStreamType(outputStreamType::consoleStream) { }

MatchingEngine::MatchingEngine(const MatchingEngine& copyRef)
{
	this->m_SellOrderQueue = copyRef.m_SellOrderQueue;
	this->m_BuyOrderQueue = copyRef.m_BuyOrderQueue;
	this->m_badOrders = copyRef.m_badOrders;
	this->m_activeOrderSet = copyRef.m_activeOrderSet;
	this->m_lastTradedPrice = copyRef.m_lastTradedPrice;
	this->m_lastTradedQty = copyRef.m_lastTradedQty;
	this->m_lastTradeNumber = copyRef.m_lastTradeNumber;
	this->m_logStreamType = copyRef.m_logStreamType;
	this->m_stockSymbol = copyRef.m_stockSymbol;
}

MatchingEngine& MatchingEngine::operator=(const MatchingEngine& assignRef)
{
	this->m_SellOrderQueue = assignRef.m_SellOrderQueue;
	this->m_BuyOrderQueue = assignRef.m_BuyOrderQueue;
	this->m_badOrders = assignRef.m_badOrders;
	this->m_activeOrderSet = assignRef.m_activeOrderSet;
	this->m_lastTradedPrice = assignRef.m_lastTradedPrice;
	this->m_lastTradedQty = assignRef.m_lastTradedQty;
	this->m_lastTradeNumber = assignRef.m_lastTradeNumber;
	this->m_logStreamType = assignRef.m_logStreamType;
	this->m_stockSymbol = assignRef.m_stockSymbol;
	return(*this);
}

MatchingEngine::~MatchingEngine()
{
	m_BuyOrderQueue.erase(m_BuyOrderQueue.begin(), m_BuyOrderQueue.end());
	m_SellOrderQueue.erase(m_BuyOrderQueue.begin(), m_BuyOrderQueue.end());
	m_outputFileStream.close();

	// If the file is empty and mode is console then delete the file.
	if(m_logStreamType == outputStreamType::consoleStream)
	{
		std::string commandStr = "rm -f ";
		commandStr += getStockSymbol();
		commandStr += ".output.log";
		std::system(commandStr.c_str());
	}
}

void MatchingEngine::setoutputStreamType(outputStreamType argStreamType)
{
	m_logStreamType = argStreamType;
}

outputStreamType MatchingEngine::getoutputStreamType()
{
	return m_logStreamType;
}

void MatchingEngine::writeToStream(const std::string& message, std::ostream& os = std::cout)
{
	os << "Thread Id = " << std::this_thread::get_id() << ", Message = " << message << '\n';
}

void MatchingEngine::writeToLog(const std::string& logMessage)
{
	std::ostream& os = getStreamRef();
	writeToStream(logMessage, os);
}

void MatchingEngine::setStockSymbol(const std::string& stockSymbolName) 
{
	m_stockSymbol= stockSymbolName;
} 

void MatchingEngine::setOutPutStream()
{
	setOutPutLogFileStream();
}

void MatchingEngine::setOutPutLogFileStream()
{
	// Initialize the file stream for log output here.
	std::string stockSymbol = getStockSymbol();
	stockSymbol += ".output.log";
	m_outputFileStream.open(stockSymbol);	
}

std::string MatchingEngine::getStockSymbol() { return(m_stockSymbol); }

void MatchingEngine::addIntoBuyQueue(const Order& order)
{
	// If the order action type is Add / A and it is not found in the activeOrderSet then add it.
	if(order.getActionType() == Order::orderAddition)
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
	if(order.getActionType() == Order::orderRemove)
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
	if(order.getActionType() == Order::orderModify)
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
	if(order.getActionType() == Order::orderAddition)
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
	if(order.getActionType() == Order::orderRemove)
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
	if(order.getActionType() == Order::orderModify)
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
	std::ostream& os = getStreamRef();
	for(const auto& iter : m_BuyOrderQueue)
		writeToStream(iter.getOrderDetails(), os);
}

void MatchingEngine::displaySellQueue()
{
	std::ostream& os = getStreamRef();
	for(const auto& iter : m_SellOrderQueue)
		writeToStream(iter.getOrderDetails(), os);
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
	std::ostream& os = getStreamRef();
	for(const auto& iter : m_badOrders)
		writeToStream(iter, os);
}

std::ostream& MatchingEngine::getStreamRef()
{
	if(getoutputStreamType() == outputStreamType::consoleStream)
		return(std::cout);
	else
		return(m_outputFileStream);
}

void MatchingEngine::runMatching()
{
	// Initialize the logging stream type here.
	std::ostream& os = getStreamRef();

	auto startTime = std::chrono::high_resolution_clock::now();

	// Dump some logs.
	writeToStream("RunMatching is called", os);

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

				if(m_lastTradedPrice != bestSellPrice)
				{
					m_lastTradedPrice = bestSellPrice;
					m_lastTradedQty = std::min(sellQty,buyQty);	
				}	
				else
				{
					m_lastTradedQty += std::min(sellQty,buyQty);
				}

				std::string logMessage = "Trade Number = ";
				logMessage  += std::to_string(m_lastTradeNumber) + ", T," + std::to_string(std::min(sellQty,buyQty)) + ","  + std::to_string(m_lastTradedPrice) + "=>";
				logMessage += std::to_string(m_lastTradedQty) + "@" + std::to_string(m_lastTradedPrice);
				writeToStream(logMessage, os);			
				m_lastTradeNumber++;

				if(tradeDirection == 0)
				{
					std::string logMessage = "INFO : Closing the order ids = ";
					logMessage += std::to_string(bestSellOrder.getOrderId()) + std::to_string(bestBuyOrder.getOrderId());
					writeToStream(logMessage, os);

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
						std::string logMessage = "INFO : Closing the order id = ";
						logMessage += std::to_string(m_BuyOrderQueue[0].getOrderId());
						writeToStream(logMessage, os);
						m_activeOrderSet.erase(m_activeOrderSet.find(m_BuyOrderQueue[0].getOrderId()));
						m_BuyOrderQueue.pop_front();
						m_SellOrderQueue[0].setOrderQty(tradeDirection);
					}
					else
					{
						std::string logMessage = "INFO : Closing the order id = ";
						logMessage += std::to_string(m_SellOrderQueue[0].getOrderId());
						writeToStream(logMessage, os);
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
			std::string logMessage = "The best bid is = " + std::to_string(bestBuyPrice) + " and the best ask is = " + std::to_string(bestSellPrice);
			logMessage += ".No match is possible given bid offer spread is = ";
			logMessage += std::to_string(bidOfferSpread);
			writeToStream(logMessage, os);
			return;
		}
	}
	else
	{
		if(m_SellOrderQueue.empty())
			writeToStream("The sell queue is empty", os);
		if(m_BuyOrderQueue.empty())
			writeToStream("The buy queue is empty", os);
	}

	auto endTime = std::chrono::high_resolution_clock::now();
	auto int_ms_long = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

	std::string logMessage = "The overall processing and matching took ";
	logMessage += std::to_string(int_ms_long);
	logMessage += " micro seconds";
	writeToStream(logMessage, os);

}
