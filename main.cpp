/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 02:57:47
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-08-31 17:00:15
 */

// main program to test out matching engine

#include <iostream>
#include <string>

#include "MatchingEngine.hpp"
#include "OrderFactory.hpp"

std::unordered_map<std::string, typename OrderFactory::OrderPtr> OrderFactory::orderIDMap_;

int main()
{
    MatchingEngine matching_engine;

    std::string command;
    while (std::getline(std::cin, command))
    {
        matching_engine.processCommand(command);
    }

    return 0;
}