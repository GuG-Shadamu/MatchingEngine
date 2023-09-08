/*
 * @Author: Tairan Gao
 * @Date:   2023-09-04 16:10:34
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-04 16:23:14
 */

#include "OrderFactory.hpp"

std::hash<std::string> OrderFactory::str_hash_;
std::unordered_map<std::size_t, std::unique_ptr<Order>> OrderFactory::orderMap_;
std::unordered_map<std::size_t, std::string> OrderFactory::orderIdMap_;
std::stack<std::unique_ptr<Order>> OrderFactory::orderPool_;

Order *OrderFactory::createOrder(const std::string &order_id_str, int price, std::size_t quantity, OrderSide side, OrderType type)
{
    std::size_t order_id_num = str_hash_(order_id_str);
    if (orderMap_.find(order_id_num) != orderMap_.end())
    {
        return nullptr;
    }
    std::unique_ptr<Order> order_ptr;
    if (!orderPool_.empty())
    {
        order_ptr = std::move(orderPool_.top());
        orderPool_.pop();
        // Reset the order_ptr with new values
        order_ptr->reset(order_id_num, price, quantity, side, type);
    }
    else
    {
        order_ptr = std::make_unique<Order>(order_id_num, price, quantity, side, type);
    }

    Order *raw_ptr = order_ptr.get();
    orderMap_[order_id_num] = std::move(order_ptr);
    orderIdMap_[order_id_num] = order_id_str;
    return raw_ptr;
}

void OrderFactory::removeOrder(const Order &order)
{
    std::size_t order_id = order.getOrderId();
    orderPool_.push(std::move(orderMap_[order_id]));
    orderMap_.erase(order_id);
    orderIdMap_.erase(order_id);
}

Order *OrderFactory::getOrderById(const std::string &order_id)
{
    std::size_t order_id_num = str_hash_(order_id);
    if (orderMap_.find(order_id_num) != orderMap_.end())
    {
        return orderMap_[order_id_num].get();
    }
    return nullptr;
}