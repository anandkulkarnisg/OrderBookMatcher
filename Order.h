#include<iostream>
#include<string>

using namespace std;

#ifndef Order_H
#define Order_H

enum class orderType { buyOrder, sellOrder };

class Order
{
	private:
		// We tag a particular increasing sequence id for each order incoming for the symbol from 0 onwards. This can also be replaced with timestamp suitably to work out the sequence of order arrivals.
		int m_seqId;

		// The action type of the order is a capital letter of either A-> Add , X -> delete and M-> Modify.
		char m_actionType;

		// The orderid is a unique positive integer. 
		long m_orderId;

		// The side is either a buy / sell [ B/S ].
		char m_orderSide;

		// The order quantity is assumed to be long +ve integer.
		long m_orderQty;

		// Price is assumed to be double.
		double m_orderPrice;

	public:

		// Default constructor.
		Order(const int& , const char , const long& , const char , const long& ,const double& );

		// Copy Constructor.
		Order(const Order& copyOrder);

		// Assignment Operator.
		Order& operator=(const Order& assignOrder);

		// Display the properties of an order. required for debugging.
		void show() const;

		// Some important getters are needed.
		int getSeqId() const; 
		const char getActionType() const; 
		long getOrderId() const; 
		const char getOrderside() const; 
		long getOrderQty() const; 
		double getOrderPrice() const; 

		// Some important setters are needed.
		void setOrderQty(const int& newQty);

		// These operators are used for comparison and sorting.
		friend bool operator < (const Order& lhs, const Order& rhs);
		friend bool operator > (const Order& lhs, const Order& rhs);

		// Find what is the orderType.
		orderType getOrderType(); 

		// Generate the order message from the order properties.
		std::string getOrderMessage() const;
};

#endif
