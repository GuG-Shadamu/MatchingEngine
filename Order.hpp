/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 16:59:45
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-04 15:37:18
 */

#ifndef ORDER_H
#define ORDER_H

#include <string>
#include <utility>
#include <chrono>

enum class OrderSide : int
{
    Buy,
    Sell
};

enum class OrderType : int
{
    GFD, // Good For Day
    IOC  // Immediate Or Cancel
};

// Order base class

class Order
{
public:
    Order(std::size_t order_id, int price, std::size_t quantity, OrderSide side, OrderType type)
            : order_id_(order_id), price_(price), quantity_(quantity), side_(side), type_(type){};

    // reset the order with new values
    void reset(std::size_t order_id, int price, std::size_t quantity, OrderSide side, OrderType type)
    {
        order_id_ = order_id;
        price_ = price;
        quantity_ = quantity;
        side_ = side;
        type_ = type;
    }

    virtual ~Order() = default;

    [[nodiscard]] std::size_t getOrderId() const { return order_id_; }
    [[nodiscard]] int getPrice() const { return price_; }
    [[nodiscard]] std::size_t getQuantity() const { return quantity_; }
    [[nodiscard]] OrderSide getSide() const { return side_; }
    [[nodiscard]] OrderType getType() const { return type_; }

    void setQuantity(std::size_t quantity) { quantity_ = quantity; }

    // make Order cannot be copied
    // so all OrderID are unique
    Order(const Order &) = delete;
    Order &operator=(const Order &) = delete;
    Order(Order &&) noexcept = delete;
    Order &operator=(Order &&) noexcept = delete;

private:
    std::size_t order_id_;
    int price_;
    std::size_t quantity_; // assume quantity is integer
    OrderSide side_;
    OrderType type_;
};

#endif // ORDER_H