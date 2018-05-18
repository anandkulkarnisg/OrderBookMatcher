#include<iostream>
#include<string>
#include "StockMatcher.h"

using namespace std;

// The main StockMatcher class implementation.

// Constructor Implementation.
StockMatcher::StockMatcher(const std::string& symbol, const std::string& dataInputFileName, const outputStreamType streamType) : m_symbol(symbol)
{
	// Attempt to initialize the input file and output log file.
	std::string inputFileName = dataInputFileName;
	m_inputFileStream.open(inputFileName, std::ifstream::in);

	// Set here the output file stream type if cout or a file.
	m_stockMatchEngine.setoutputStreamType(streamType);
	//m_stockMatchEngine.setoutputStreamType(outputStreamType::fileStream);

	// Set the stock symbol Name here.
	m_stockMatchEngine.setStockSymbol(symbol);
}

// Implement the stock Matching algorithm call.
void StockMatcher::executeMatching()
{
	std::string textLine;
	long lineCount = 0;
	while(!m_inputFileStream.eof())
	{
		getline(m_inputFileStream, textLine);
		lineCount++;
		if(textLine.length()>0)
		{
			std::tuple<std::pair<bool,std::string>,Order> message = m_validator.validateOrder(textLine);
			std::pair<bool,std::string> messageStatus = std::get<0>(message);
			bool status = messageStatus.first;
			if(status)
			{
				Order order = std::get<1>(message);
				if(order.getOrderType() == orderType::buyOrder)
				{
					m_stockMatchEngine.addIntoBuyQueue(order);
				}
				else
				{
					m_stockMatchEngine.addIntoSellQueue(order);
				}
				// Now run the stockMatching engine.
				m_stockMatchEngine.runMatching();
			}
			else
			{
				m_stockMatchEngine.pushInBadOrder(textLine, messageStatus.second);
			}
		}

		// There is a need every 10th Message to print the buy and sell queues. do that here.
		if(lineCount%10 == 0)
			m_stockMatchEngine.displayOrderBook();
	}

	// Display a final summary of any bad orders at the end of execution.
	m_stockMatchEngine.writeToLog("Bad order summary : ");
	m_stockMatchEngine.displayBadOrders();
	m_stockMatchEngine.writeToLog("Final queue summary : ");
	m_stockMatchEngine.displayOrderBook();
	m_inputFileStream.close();
}
