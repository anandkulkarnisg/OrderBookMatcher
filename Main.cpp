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
	while(!m_inputFileStream.eof())
	{
		getline(m_inputFileStream,textLine);
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
	}

	std::cout << "---------------------" << std::endl;
	match.displayBuyQueue();
	std::cout << "---------------------" << std::endl;
	match.displaySellQueue();
	std::cout << "---------------------" << std::endl;
	match.displayBadOrders();

	return(0);
}

