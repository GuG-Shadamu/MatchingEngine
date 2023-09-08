/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 18:58:41
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-04 16:20:06
 */

#ifndef ORDERFACTORY_H
#define ORDERFACTORY_H

#include <memory>
#include <string>
#include <stack>
#include <unordered_map>

#include "Order.hpp"

class OrderFactory
{

private:
    // static inline: only one instance of this variable shared across all instances
    // of OrderFactory for the same template arguments.
    static std::hash<std::string> str_hash_;
    static std::unordered_map<std::size_t, std::unique_ptr<Order>> orderMap_;
    static std::unordered_map<std::size_t, std::string> orderIdMap_;
    static std::stack<std::unique_ptr<Order>> orderPool_;

public:
    static Order *createOrder(const std::string &order_id_str, int price, std::size_t quantity, OrderSide side, OrderType type);
    static void removeOrder(const Order &order);
    static Order *getOrderById(const std::string &order_id);
    static std::string getOrderStrById(const std::size_t order_id) { return orderIdMap_[order_id]; }

    // make OrderFactory cannot be copied, just for good practice
    OrderFactory(const OrderFactory &) = delete;
    OrderFactory &operator=(const OrderFactory &) = delete;
};

#endif // ORDERFACTORY_H
