/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 17:16:51
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 17:07:01
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
#include "OrderFactory.hpp"

class OrderBook
{
public:
    using OrderPtr = std::shared_ptr<Order>;
    OrderBook() : buy_book_(SkipList(false)), sell_book_(SkipList(true)){};

    void cancelOrder(const std::string &order_id);
    void matchNewOrder(OrderPtr &Order);
    void addOrder(OrderPtr order);
    void modifyOrder(const std::string &order_id, OrderSide new_side, int new_price, int new_quantity);
    void print() const;

    ~OrderBook() = default;
    OrderBook(const OrderBook &) = delete;
    OrderBook &operator=(const OrderBook &) = delete;
    OrderBook(OrderBook &&) = delete;
    OrderBook &operator=(OrderBook &&) = delete;

private:
    static void printTrade(const OrderPtr &buy_order, const OrderPtr &sell_order, unsigned int trade_quantity);

private:
    SkipList buy_book_;
    SkipList sell_book_;
};

#endif // ORDERBOOK_H