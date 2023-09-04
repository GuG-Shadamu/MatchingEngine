/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 18:58:41
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 16:53:38
 */

#ifndef ORDERFACTORY_H
#define ORDERFACTORY_H

#include <memory>
#include <string>
#include <unordered_map>

#include "Order.hpp"

class OrderFactory
{
public:
    using OrderPtr = std::shared_ptr<Order>;

    template <class... Args>
    static OrderPtr createOrder(Args &&...args)
    {
        return std::make_shared<Order>(std::forward<Args>(args)...);
    }

    static void addOrder(const OrderPtr& order)
    {
        orderIDMap_[order->getOrderId()] = order;
    }

    static void removeOrder(const std::string &orderID)
    {
        orderIDMap_.erase(orderID);
    }

    static bool isOrderExist(const std::string &orderID)
    {
        return orderIDMap_.find(orderID) != orderIDMap_.end();
    }

    static OrderPtr getOrderById(const std::string &orderID)
    {
        return orderIDMap_[orderID];
    }
    // make OrderFactory cannot be copied, just for good practice
    OrderFactory(const OrderFactory &) = delete;
    OrderFactory &operator=(const OrderFactory &) = delete;

private:
    // static inline: only one instance of this variable shared across all instances
    // of OrderFactory for the same template arguments.
    static std::unordered_map<std::string, OrderPtr> orderIDMap_;
};

#endif // ORDERFACTORY_H
