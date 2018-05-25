#!/bin/bash

rm -f *.o OrderMatchingEngine

for fileItem in $(ls *.cpp)
do
	g++ -c $1 $fileItem
done

g++ $1 *.o -o OrderMatchingEngine
