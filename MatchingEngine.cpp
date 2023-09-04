/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 10:17:38
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 17:09:39
 */

#include <string>
#include <memory>
#include <vector>
#include <sstream>

#include "MatchingEngine.hpp"
#include "OrderFactory.hpp"
#include "Order.hpp"

using OrderPtr = std::shared_ptr<Order>;

std::vector<std::string> split(const std::string &s, char delimiter)
{
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);
    while (std::getline(tokenStream, token, delimiter))
    {
        tokens.push_back(token);
    }
    return tokens;
}

void MatchingEngine::processCommand(const std::string &command)
{
    std::vector<std::string> tokens = split(command, ' ');
    if (tokens[0] == "BUY" or tokens[0] == "SELL")
    {
        // create order
        std::string order_id = tokens[4];
        if (OrderFactory::isOrderExist(order_id))
            return;

        OrderType type = tokens[1] == "GFD" ? OrderType::GFD : OrderType::IOC;
        int price = std::stoi(tokens[2]);
        unsigned int quantity = std::stoi(tokens[3]);
        OrderSide side = tokens[0] == "BUY" ? OrderSide::Buy : OrderSide::Sell;

        OrderPtr order = OrderFactory::createOrder(order_id, price, quantity, side, type);
        order_book_.addOrder(order);
        return;
    }

    if (tokens[0] == "CANCEL")
    {
        std::string order_id = tokens[1];
        if (!OrderFactory ::isOrderExist(order_id))
            return;

        order_book_.cancelOrder(order_id);
        return;
    }

    if (tokens[0] == "MODIFY")
    {
        std::string order_id = tokens[1];
        if (!OrderFactory::isOrderExist(order_id))
            return;
        OrderSide new_side = tokens[2] == "BUY" ? OrderSide::Buy : OrderSide::Sell;
        int new_price = std::stoi(tokens[3]);
        int new_quantity = std::stoi(tokens[4]);
        order_book_.modifyOrder(order_id, new_side, new_price, new_quantity);
        return;
    }

    if (tokens[0] == "PRINT")
    {
        order_book_.print();
        return;
    }
}
