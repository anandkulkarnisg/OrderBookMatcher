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

	// Set the stock symbol Name here.
	m_stockMatchEngine.setStockSymbol(symbol);

	// populate the Cache Stream here itself.
	populateCacheStream();
}

// Copy constructor.
StockMatcher::StockMatcher(const StockMatcher& copyRef)
{
	this->m_symbol = copyRef.m_symbol;
	this->m_validator = copyRef.m_validator;
	this->m_stockMatchEngine = copyRef.m_stockMatchEngine;
	this->m_cachedStream = copyRef.m_cachedStream;
}

// Assignment operator.
StockMatcher& StockMatcher::operator=(const StockMatcher& assignRef)
{
	this->m_symbol = assignRef.m_symbol;
	this->m_validator = assignRef.m_validator;
	this->m_stockMatchEngine = assignRef.m_stockMatchEngine;
	this->m_cachedStream = assignRef.m_cachedStream;
	return(*this);
}

void StockMatcher::populateCacheStream()
{
	// This function tries to populate the cache stream from messages of the file and close the file handle. 
	// This is to avoid accessing ifstream in threaded mode since it is not safe and does not seem to work
	// With even locks.

	std::string textLine;
	while(!m_inputFileStream.eof())
	{
		getline(m_inputFileStream, textLine);
		if(textLine.length() > 0)
		{
			m_cachedStream.push_back(textLine);
		}
	}

	m_inputFileStream.close();
}

// Implement the stock Matching algorithm call.
void StockMatcher::executeMatching()
{
	m_stockMatchEngine.setOutPutStream();
    std::string textLine;
    long lineCount = 0;
    auto cacheIter = m_cachedStream.begin();
    while(cacheIter != m_cachedStream.end())
    {
        textLine = *cacheIter;
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

        cacheIter++;
    }

    // Display a final summary of any bad orders at the end of execution.
    m_stockMatchEngine.writeToLog("Bad order summary : ");
    m_stockMatchEngine.displayBadOrders();
    m_stockMatchEngine.writeToLog("Final queue summary : ");
    m_stockMatchEngine.displayOrderBook();
}

