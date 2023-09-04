/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 02:58:48
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-08-31 16:49:33
 */

#ifndef MATCHINGENGINE_H
#define MATCHINGENGINE_H

#include <string>
#include <vector>

#include "OrderBook.hpp"

class MatchingEngine
{
public:
    MatchingEngine() = default;
    ~MatchingEngine() = default;

    void processCommand(const std::string &command);

    MatchingEngine(const MatchingEngine &) = delete;
    MatchingEngine &operator=(const MatchingEngine &) = delete;
    MatchingEngine(MatchingEngine &&) = delete;
    MatchingEngine &operator=(MatchingEngine &&) = delete;

private:
    // We have only one OrderBook<int> in our case
    OrderBook order_book_;
};

#endif // MATCHINGENGINE_H