/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 10:17:38
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-04 17:03:02
 */

#include <string>
#include <vector>
#include <sstream>

#include "MatchingEngine.hpp"
#include "OrderFactory.hpp"
#include "Order.hpp"

std::vector<std::string> split(const std::string &s, char delimiter)
{
    // Check if the stream is in a good state

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
        OrderType type = tokens[1] == "GFD" ? OrderType::GFD : OrderType::IOC;
        int price = std::stoi(tokens[2]);
        std::size_t quantity = std::stoi(tokens[3]);
        OrderSide side = tokens[0] == "BUY" ? OrderSide::Buy : OrderSide::Sell;

        auto order_ptr = OrderFactory::createOrder(order_id, price, quantity, side, type);
        if (order_ptr != nullptr)
        {
            order_book_.addOrder(order_ptr);
        }
    }

    if (tokens[0] == "CANCEL")
    {
        std::string order_id = tokens[1];
        Order *order_ptr = OrderFactory::getOrderById(order_id);

        if (order_ptr == nullptr)
            return;

        OrderFactory::removeOrder(*order_ptr);
        order_book_.cancelOrder(*order_ptr);
        return;
    }

    if (tokens[0] == "MODIFY")
    {
        std::string order_id = tokens[1];
        Order *order_ptr = OrderFactory::getOrderById(order_id);
        if (order_ptr == nullptr)
            return;

        OrderType type = tokens[5] == "GFD" ? OrderType::GFD : OrderType::IOC;
        if (order_ptr->getType() == OrderType::IOC)
            return;

        OrderSide new_side = tokens[2] == "BUY" ? OrderSide::Buy : OrderSide::Sell;

        int new_price = std::stoi(tokens[3]);
        int new_quantity = std::stoi(tokens[4]);

        OrderFactory::removeOrder(*order_ptr);
        order_book_.cancelOrder(*order_ptr);

        Order *new_order_ptr = OrderFactory::createOrder(order_id, new_price, new_quantity, new_side, type);
        order_book_.addOrder(new_order_ptr);

        return;
    }

    if (tokens[0] == "PRINT")
    {
        order_book_.print();
        return;
    }
}
