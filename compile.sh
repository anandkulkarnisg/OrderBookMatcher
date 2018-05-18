#!/bin/bash

rm -f *.o OrderMatchingEngine

for fileItem in $(ls *.cpp)
do
	g++ -c $fileItem
done

g++ *.o -o OrderMatchingEngine
