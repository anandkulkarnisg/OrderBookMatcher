#!/bin/bash

rm -f *.o OrderMatchingEngine
g++ -c Order.cpp
g++ -c OrderValidator.cpp
g++ -c MatchingEngine.cpp
g++ -c Main.cpp
g++ *.o -o OrderMatchingEngine
