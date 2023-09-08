/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 11:28:00
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 02:22:56
 */

#include <iostream>
#include <string>

#include "OrderBook.hpp"
#include "SkipList.hpp"
#include "OrderFactory.hpp"

// Implementation of OrderBook

void OrderBook::cancelOrder(const Order &order)
{

    OrderSide side = order.getSide();

    if (side == OrderSide::Buy)
    {
        buy_book_.removeOrder(order);
    }
    else
    {
        sell_book_.removeOrder(order);
    }
    // remove from overall map
    OrderFactory::removeOrder(order);
}

void OrderBook::addOrder(Order *order)
{
    if (!matchNewOrder(order))
    {
        OrderFactory::removeOrder(*order);
        return;
    }

    if (order->getSide() == OrderSide::Buy)
    {
        buy_book_.insertOrder(order);
    }
    else
    {
        sell_book_.insertOrder(order);
    }
}

void OrderBook::settleTrades(Order *order, const std::vector<Order *> &orders_to_remove, Order *order_to_modify, std::size_t &trade_quantity)
{

    for (const auto &counter_order : orders_to_remove)
    {
        SkipList &counter_party = order->getSide() == OrderSide::Buy ? sell_book_ : buy_book_;
        printTrade(*counter_order, *order, counter_order->getQuantity());
        counter_party.removeOrder(*counter_order);
        OrderFactory::removeOrder(*counter_order);
    }
    if (order_to_modify != nullptr)
    {
        printTrade(*order_to_modify, *order, trade_quantity);
        order_to_modify->setQuantity(order_to_modify->getQuantity() - trade_quantity);
    }
}

bool OrderBook::matchNewOrder(Order *order)
{
    int price = order->getPrice();
    std::size_t quantity = order->getQuantity();
    OrderType type = order->getType();

    std::size_t counter_order_quantity;
    std::size_t last_trade_quantity;
    std::vector<Order *> orders_to_remove;
    Order *order_to_modify = nullptr;

    bool isBuy = (order->getSide() == OrderSide::Buy);
    SkipList &counter_party = isBuy ? sell_book_ : buy_book_;
    SkipListNode *node = counter_party.front();

    while (node != nullptr)
    {
        if (quantity == 0)
            break;
        int counter_order_price = node->getPrice();
        if ((isBuy && counter_order_price > price) || (!isBuy && counter_order_price < price))
            break;

        for (const auto &counter_order : node->getList())
        {
            counter_order_quantity = counter_order->getQuantity();
            if (counter_order_quantity <= quantity)
            {
                orders_to_remove.push_back(counter_order);
                quantity -= counter_order_quantity;
            }
            else
            {
                last_trade_quantity = quantity;
                order_to_modify = counter_order;
                quantity = 0;
                break;
            }
        }
        node = node->next();
    }

    if (type == OrderType::GFD)
    {
        settleTrades(order, orders_to_remove, order_to_modify, last_trade_quantity);
        return quantity != 0;
    }

    else // type == IOC
    {
        if (quantity == 0)
        {
            settleTrades(order, orders_to_remove, order_to_modify, last_trade_quantity);
        }
        return false;
    }
}

void OrderBook::print() const
{
    std::vector<std::string> buy_book_summary = buy_book_.summarize();
    std::vector<std::string> sell_book_summary = sell_book_.summarize();

    std::cout << "BUY" << std::endl;
    for (const auto &summary : buy_book_summary)
    {
        std::cout << summary << std::endl;
    }

    // reverse print
    std::cout << "SELL" << std::endl;
    for (auto it = sell_book_summary.rbegin(); it != sell_book_summary.rend(); ++it)
    {
        std::cout << *it << std::endl;
    }
}

void OrderBook::printTrade(const Order &order1, const Order &order2, std::size_t trade_quantity)
{
    std::cout << "TRADE";
    std::cout << " " << OrderFactory::getOrderStrById(order1.getOrderId());
    std::cout << " " << std::to_string(trade_quantity);
    std::cout << " " << OrderFactory::getOrderStrById(order2.getOrderId());
    std::cout << " " << std::to_string(trade_quantity);
    std::cout << std::endl;
}