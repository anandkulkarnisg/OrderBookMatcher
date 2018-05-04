#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include "OrderValidator.h"
#include<fstream>

int main(int argc, char* argv[])
{
	OrderValidator validator;
	MatchingEngine stockMatcher;

	if(argc != 2)
	{
		std::cout << "Invalid Usage : Proper usage is OrderMatchingEngine [inputfile]" << std::endl;
		exit(1);
	}
	
	std::string inputFile = argv[1];
	ifstream m_inputFileStream(inputFile);

	std::string textLine;
	long lineCount = 0;
	while(!m_inputFileStream.eof())
	{
		getline(m_inputFileStream,textLine);
		lineCount++;
		if(textLine.length()>0)
		{
			std::tuple<std::pair<bool,std::string>,Order> message = validator.validateOrder(textLine);
			std::pair<bool,std::string> messageStatus = std::get<0>(message);
			bool status = messageStatus.first;
			if(status)
			{
				Order order = std::get<1>(message);
				if(order.getOrderType() == orderType::buyOrder)
				{
					stockMatcher.addIntoBuyQueue(order);
				}
				else
				{
					stockMatcher.addIntoSellQueue(order);
				}
				// Now run the stockMatchering engine.
				stockMatcher.runMatching();
			}
			else
			{
				stockMatcher.pushInBadOrder(textLine, messageStatus.second);
			}
		}

		// There is a need every 10th Message to print the buy and sell queues. do that here.
		if(lineCount%10 == 0)
			stockMatcher.displayOrderBook();
	}

	// Display a final summary of any bad orders at the end of execution.
	std::cout << "Bad order summary : " << std::endl;
	stockMatcher.displayBadOrders();
	std::cout << "Final queue summary : " << std::endl;
	stockMatcher.displayOrderBook();
	return(0);
}

