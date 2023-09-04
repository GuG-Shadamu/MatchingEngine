/*
 * @Author: Tairan Gao
 * @Date:   2023-08-29 16:59:45
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-08-31 12:28:12
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
    Order(std::string order_id, int price, unsigned int quantity, OrderSide side, OrderType type)
            : order_id_(std::move(order_id)), price_(price), quantity_(quantity), side_(side), type_(type){};

    virtual ~Order() = default;

    [[nodiscard]] std::string getOrderId() const { return order_id_; }
    void setOrderID(std::string id) { order_id_ = std::move(id); }

    [[nodiscard]] int getPrice() const { return price_; }
    void setPrice(int price) { price_ = price; }

    [[nodiscard]] unsigned int getQuantity() const { return quantity_; }
    void setQuantity(unsigned int quantity) { quantity_ = quantity; }

    [[nodiscard]] OrderSide getSide() const { return side_; }
    void setSide(OrderSide side) { side_ = side; }

    [[nodiscard]] OrderType getType() const { return type_; }
    void setType(OrderType type) { type_ = type; }

    // make Order cannot be copied
    // so all OrderID are unique
    Order(const Order &) = delete;
    Order &operator=(const Order &) = delete;
    Order(Order &&) noexcept = delete;
    Order &operator=(Order &&) noexcept = delete;

private:
    std::string order_id_;
    int price_;
    unsigned int quantity_; // assume quantity is integer
    OrderSide side_;
    OrderType type_;
};

#endif // ORDER_H