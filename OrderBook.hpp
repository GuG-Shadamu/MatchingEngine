/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 17:16:51
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 01:35:26
 */

#ifndef ORDERBOOK_H
#define ORDERBOOK_H

#include <unordered_map>
#include <unordered_set>
#include <memory>
#include <list>
#include <string>

#include "Order.hpp"
#include "SkipList.hpp"

class OrderBook
{
public:
    OrderBook() : buy_book_(SkipList(false)), sell_book_(SkipList(true)){};

    void cancelOrder(const Order &order);
    bool matchNewOrder(Order *Order);
    void addOrder(Order *order);
    void print() const;

    ~OrderBook() = default;
    OrderBook(const OrderBook &) = delete;
    OrderBook &operator=(const OrderBook &) = delete;
    OrderBook(OrderBook &&) = delete;
    OrderBook &operator=(OrderBook &&) = delete;

private:
    static void printTrade(const Order &order1, const Order &order2, std::size_t trade_quantity);
    void settleTrades(Order *order, const std::vector<Order *> &orders_to_remove, Order *order_to_modify, std::size_t &trade_quantity);

private:
    SkipList buy_book_;
    SkipList sell_book_;
};

#endif // ORDERBOOK_H