cmake_minimum_required(VERSION 2.8.4)
project(OrderBookMatcher)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -g -Wall")
set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lpthread")

# Add test Examples below.
add_executable(OrderMatchingEngine FeedFileHandler.cpp Main.cpp MatchingEngine.cpp Order.cpp OrderValidator.cpp StockMatcher.cpp)
 
