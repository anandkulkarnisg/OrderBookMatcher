#include<iostream>
#include<string>
#include "OrderValidator.h"

using namespace std;

long OrderValidator::seqId = 0;

std::tuple<std::pair<bool,std::string>,Order> OrderValidator::validateOrder(const std::string& inputOrderString)
{
    // Let us have a blank orderType that is returned and not used if the message parsing fails.
    Order emptyOrder(-1,Order::orderAddition,0,'B',0,0);
    std::string failMessage = "";

    // Now let us try to get the seperate tokens and validate each of them.
    std::vector<std::string> orderItems;
    char_separator<char> seperator(",");
    tokenizer<char_separator<char>> tokens(inputOrderString, seperator);
    for(const auto& iter : tokens)
        orderItems.push_back(iter);

    // If the number of tokens is not five then simply fail the validation.
    if(orderItems.size() != 5)
    {
        failMessage = "Invalid Message. Wrong number of tokens in the message";
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    // Check the order type i.e action length should be 1.
    std::string orderType = orderItems[0];
    if(orderType.length() != 1)
    {
        failMessage = "Invalid order action type length : " + orderType;
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    // Order type should be either A/X/M.
    if(std::toupper(orderType[0]) != Order::orderAddition && std::toupper(orderType[0]) != Order::orderRemove && std::toupper(orderType[0]) != Order::orderModify)
    {
        failMessage = "Invalid order action type : " + orderType + ". It should be either A(Add)/X(Remove)/M(Modify)";
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    // Now extract the orderId. It should be an integer / long assumed here.
    long orderId = 0;
	    try
    {
        orderId = std::stol(orderItems[1]);
    } catch(const std::invalid_argument& exception)
    {
        failMessage = "Failed to convert the orderId into proper long : Exception is : ";
        std::string msgType(exception.what());
        failMessage += msgType;
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    std::string orderSide = orderItems[2];
    if(orderSide.length() != 1)
    {
        failMessage = "Invalid OrderSide Length : " + orderSide + ". It should be either B/S";
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    if(orderSide[0] != 'B' && orderSide[0] != 'S')
    {
        failMessage = "Invalid orderSide : " + orderSide + ". It should be either B/S";
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    long orderQty = 0;
    try
    {
        orderQty = std::stol(orderItems[3]);
    } catch(const std::invalid_argument& exception)
    {
        failMessage = "Failed to convert the " + orderItems[3] + " into valid order quantity long type.Exception is : " + exception.what();
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    if(orderQty <= 0)
    {
        failMessage = "The order quantity has to be a +ve long. Currently it is : " + orderItems[3];
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }
	    double orderPrice = 0;
    try
    {
        orderPrice = std::stod(orderItems[4]);
    } catch(const std::invalid_argument& exception)
    {
        failMessage = "Failed to convert the " + orderItems[4] + " into valid order price od double type.Exception is : " + exception.what();
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    if(orderPrice <= 0)
    {
        failMessage = "The order price has to be a +ve price. Currently it is : " + orderItems[4];
        return(std::make_tuple(std::make_pair(false,failMessage),emptyOrder));
    }

    // This means we have a valid order if we reached here.Construct and return here.   
    Order validOrder(seqId, (orderType.c_str())[0], orderId, (orderSide.c_str())[0], orderQty, orderPrice);
    seqId++;
    return(std::make_tuple(std::make_pair(true,""),validOrder));
}


