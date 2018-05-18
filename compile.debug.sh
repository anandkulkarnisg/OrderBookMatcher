#!/bin/bash

rm -f *.o OrderMatchingEngine

for fileItem in $(ls *.cpp)
do
	g++ -g -c $fileItem
done

g++ -g *.o -o OrderMatchingEngine
