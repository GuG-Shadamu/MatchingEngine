cmake_minimum_required(VERSION 3.21)
project(clion_matching_engine)

set(CMAKE_CXX_STANDARD 14)

add_executable(clion_matching_engine main.cpp
        Order.hpp
        SkipList.cpp
        SkipList.hpp
        MatchingEngine.cpp
        MatchingEngine.hpp
        OrderBook.cpp
        OrderBook.hpp
        OrderFactory.hpp)
