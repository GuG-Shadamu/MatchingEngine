cmake_minimum_required(VERSION 3.21)
project(Matching-Engine VERSION 0.1.0 LANGUAGES C CXX)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)

# Set binary (executable) output directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/out)

add_executable(matching_engine main.cpp
    Order.hpp
    SkipList.cpp
    SkipList.hpp
    MatchingEngine.cpp
    MatchingEngine.hpp
    OrderBook.cpp
    OrderBook.hpp
    OrderFactory.hpp
    OrderFactory.cpp
    SkipListNode.cpp
    SkipListNode.hpp)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
