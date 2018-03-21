#include<iostream>
#include<string>
#include "MatchingEngine.h"
#include "OrderValidator.h"
#include<fstream>

int main(int argc, char* argv[])
{
	OrderValidator validator;
	MatchingEngine match;

	ifstream m_inputFileStream("input.txt");
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
					match.addIntoBuyQueue(order);
				}
				else
				{
					match.addIntoSellQueue(order);
				}
				// Now run the matching engine.
				match.runMatching();
			}
			else
			{
				match.pushInBadOrder(textLine, messageStatus.second);
			}
		}

		// There is a need every 10th Message to print the buy and sell queues. do that here.
		if(lineCount%10 == 0)
			match.displayOrderBook();
	}

	// Display a final summary of any bad orders at the end of execution.
	std::cout << "Bad order summary : " << std::endl;
	match.displayBadOrders();
	std::cout << "Final queue summary : " << std::endl;
	match.displayOrderBook();

	return(0);
}

