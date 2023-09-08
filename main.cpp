/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 02:57:47
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-04 16:18:00
 */

// main program to test out matching engine

#include <iostream>
#include <string>

#include "MatchingEngine.hpp"

int main()
{
    MatchingEngine matching_engine;

    std::string command;
    while (std::getline(std::cin, command))
    {
        try
        {
            matching_engine.processCommand(command);
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
            continue;
        }
    }

    return 0;
}