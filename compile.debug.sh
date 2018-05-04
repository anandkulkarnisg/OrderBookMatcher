#!/bin/bash

rm -f *.o OrderMatchingEngine
g++ -g -c Order.cpp
g++ -g -c OrderValidator.cpp
g++ -g -c MatchingEngine.cpp
g++ -g -c Main.cpp
g++ -g *.o -o OrderMatchingEngine
