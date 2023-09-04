/*
 * @Author: Tairan Gao
 * @Date:   2023-08-31 11:28:00
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 17:16:50
 */

#include <iostream>
#include <string>
#include <memory>

#include "OrderBook.hpp"
#include "SkipList.hpp"

// Implementation of OrderBook

void OrderBook::cancelOrder(const std::string &order_id)
{

    OrderPtr order = OrderFactory::getOrderById(order_id);
    OrderSide side = order->getSide();

    if (side == OrderSide::Buy)
    {
        buy_book_.remove_order(order);
    }
    else
    {
        sell_book_.remove_order(order);
    }
    // remove from overall map
    OrderFactory::removeOrder(order_id);
}

void OrderBook::addOrder(OrderPtr order)
{
    matchNewOrder(order);
    unsigned int quantity = order->getQuantity();
    if (quantity == 0)
    {
        return;
    }

    OrderFactory::addOrder(order);
    if (order->getSide() == OrderSide::Buy)
    {
        buy_book_.insert_order(order);
    }
    else
    {
        sell_book_.insert_order(order);
    }
}

void OrderBook::matchNewOrder(OrderPtr &order)
{
    int price = order->getPrice();
    unsigned int quantity = order->getQuantity();
    bool isBuy = (order->getSide() == OrderSide::Buy);
    SkipList &counter_party = isBuy ? sell_book_ : buy_book_;

    while (!counter_party.empty())
    {
        OrderPtr top_counter_order = counter_party.get_top_order();
        int best_price = top_counter_order->getPrice();

        if (quantity == 0)
            break;

        if ((isBuy && best_price > price) || (!isBuy && best_price < price))
            break;

        unsigned int counter_order_quantity = top_counter_order->getQuantity();
        unsigned int trade_quantity = std::min(quantity, counter_order_quantity);

        quantity -= trade_quantity;
        counter_order_quantity -= trade_quantity;

        printTrade(top_counter_order, order, trade_quantity);
        if (counter_order_quantity > 0)
        {
            top_counter_order->setQuantity(counter_order_quantity);
        }
        else
        {
            counter_party.pop_top_order();
            OrderFactory::removeOrder(top_counter_order->getOrderId());
        }
    }
    order->setQuantity(quantity);
}

void OrderBook::modifyOrder(const std::string &order_id, OrderSide new_side, int new_price, int new_quantity)
{

    OrderPtr order_to_modify = OrderFactory::getOrderById(order_id);
    if (order_to_modify->getType() == OrderType::IOC)
    {
        return;
    }
    cancelOrder(order_to_modify->getOrderId());

    OrderPtr new_order = OrderFactory::createOrder(order_id, new_price, new_quantity, new_side, OrderType::GFD);
    addOrder(new_order);
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

void OrderBook::printTrade(const OrderPtr &order1, const OrderPtr &order2, unsigned int trade_quantity)
{
    std::cout << "TRADE";
    std::cout << " " << order1->getOrderId();
    std::cout << " " << std::to_string(trade_quantity);
    std::cout << " " << order2->getOrderId();
    std::cout << " " << std::to_string(trade_quantity);
    std::cout << std::endl;
}