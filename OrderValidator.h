#include<iostream>
#include<string>
#include<tuple>
#include<boost/tokenizer.hpp>
#include "Order.h"

using namespace std;
using namespace boost;

// This is a validator/helper class that implements the validation of building an order object from string by parsing / validating etc.
// If successfull it returns true of else false.

#ifndef OrderValidator_H
#define OrderValidator_H

class OrderValidator
{
    private:
        static long seqId;
    public:
        // The validateOrder returns true/false flag on validitiy , if false it has a string message of parse failure reason.
        // It also has Order which is used only in case order is valid.
        std::tuple<std::pair<bool,std::string>,Order> validateOrder(const std::string& inputOrderString);
};

#endif
