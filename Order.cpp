#include<iostream>
#include<string>
#include "Order.h"

using namespace std;

// Default constructor implementation.
Order::Order(const int& seqId, const char actionType, const long& orderId, const char orderSide, const long& orderQty,const double& orderPrice) :            
	m_seqId(seqId) , m_actionType(actionType), m_orderId(orderId), m_orderSide(orderSide), m_orderQty(orderQty), m_orderPrice(orderPrice)
{ 

}

// Copy constructor implementation.
Order::Order(const Order& copyOrder)
{
	m_seqId = copyOrder.m_seqId;
	m_actionType = copyOrder.m_actionType;
	m_orderId = copyOrder.m_orderId;
	m_orderSide = copyOrder.m_orderSide;
	m_orderQty = copyOrder.m_orderQty;
	m_orderPrice = copyOrder.m_orderPrice;
}

// Assignment operator implementation.
Order& Order::operator=(const Order& assignOrder)
{
	m_seqId = assignOrder.m_seqId;
	m_actionType = assignOrder.m_actionType;
	m_orderId = assignOrder.m_orderId;
	m_orderSide = assignOrder.m_orderSide;
	m_orderQty = assignOrder.m_orderQty;
	m_orderPrice = assignOrder.m_orderPrice;
	return(*this);
}

// Display Order properties for debugging purpose.
void Order::show() const
{
	std::cout << "Order seqId = " << m_seqId << std::endl;
	std::cout << "Order ActionType = " << m_actionType << std::endl;
	std::cout << "OrderId = " << m_orderId << std::endl;
	std::cout << "OrderSide = " << m_orderSide << std::endl;
	std::cout << "OrderQty = " << m_orderQty << std::endl;
	std::cout << "OrderPrice = " << m_orderPrice << std::endl;
}

// Some very important getters and setters required for the Order. 
// Getters are below.
int Order::getSeqId() const { return(m_seqId); }
const char Order::getActionType() const { return(m_actionType); }
long Order::getOrderId() const { return(m_orderId); }
const char Order::getOrderside() const { return(m_orderSide); }
long Order::getOrderQty() const { return(m_orderQty); }
double Order::getOrderPrice() const { return(m_orderPrice); }

// Setters are below.
void Order::setOrderQty(const int& newQty) { m_orderQty = newQty; }

// The comparison operators for the Order structure sorting are implemented below.
bool operator < (const Order& lhs, const Order& rhs)
{
	// This is a 2 criteria sorter. First by price. if prices are same then by seqId. The lesser the seqId the earlier the order had come hence it is given priority in the queue.
	if(lhs.getOrderPrice() < rhs.getOrderPrice())
	{
		return(true);
	}

	if(lhs.getOrderPrice() > rhs.getOrderPrice())
	{
		return(false);
	}

	if(lhs.getOrderPrice() == rhs.getOrderPrice())
	{
		return(lhs.getSeqId() < rhs.getSeqId());
	}
}

bool operator > (const Order& lhs, const Order& rhs)
{
    // This is a 2 criteria sorter. First by price. if prices are same then by seqId. The lesser the seqId the earlier the order had come hence it is given priority in the queue.
    if(lhs.getOrderPrice() > rhs.getOrderPrice())
    {
        return true;
    }

    if(rhs.getOrderPrice() < lhs.getOrderPrice())
    {
        return false;
    }

    if(lhs.getOrderPrice() == rhs.getOrderPrice())
    {
        return(lhs.getSeqId() < rhs.getSeqId());
    }
}

bool operator == (const Order& lhs, const Order& rhs)
{
	return(lhs.getOrderId() == rhs.getOrderId()); 
}

// Construct the order message from the properties.
std::string Order::getOrderMessage() const
{
    // A,100000,B,2,94 
    std::string str(1, m_actionType);
    std::string returnMessage = str;returnMessage += ",";
    returnMessage += std::to_string(m_orderId); returnMessage += ",";
    returnMessage += m_orderSide; returnMessage += ",";
    returnMessage += std::to_string(m_orderQty); returnMessage += ",";
    returnMessage += std::to_string(m_orderPrice);
    return(returnMessage);
}




orderType Order::getOrderType() { if(m_orderSide == 'B') return(orderType::buyOrder); else return(orderType::sellOrder); }
