/*
 * @Author: Tairan Gao
 * @Date:   2023-09-01 11:58:22
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 14:55:20
 */

#ifndef SKIPLIST_H
#define SKIPLIST_H

#include <iostream>
#include <list>
#include <vector>
#include <unordered_map>
#include <limits>
#include <random>
#include <memory>

#include "Order.hpp"

constexpr int MAX_LEVEL = 5;

using OrderPtr = std::shared_ptr<Order>;

class SkipList
{
private:
    struct SkipListNode
    {
        const int price_;
        size_t size_;

        std::list<OrderPtr> innerList_;
        std::array<SkipListNode *, MAX_LEVEL> forward_{};
        std::array<SkipListNode *, MAX_LEVEL> backward_{};

        SkipListNode() : price_(std::numeric_limits<int>::max()), size_(0) {}
        explicit SkipListNode(const OrderPtr& order) : price_(order->getPrice()), innerList_({order}), size_(1) {}
        explicit SkipListNode(int price) : price_(price), size_(0) {}

        std::string summarize() const;

        ~SkipListNode() = default;
        SkipListNode(const SkipListNode &) = delete;
        SkipListNode &operator=(const SkipListNode &) = delete;
        SkipListNode(SkipListNode &&) = delete;
        SkipListNode &operator=(SkipListNode &&) = delete;
    };

    int level_;
    size_t size_;
    bool isAscending_;

    SkipListNode *head_;
    std::unordered_map<int, SkipListNode *> price_to_linked_list_map_;                          // price -> linked-list node
    std::unordered_map<std::string, std::list<OrderPtr>::iterator> id_to_linked_list_itor_map_; // order_id -> iterator of linked-list in skip list

    std::mt19937 rng;
    std::geometric_distribution<int> level_distribution;

public:
    explicit SkipList(bool isAscending = true);
    ~SkipList();

    void insert_order(const OrderPtr& order);
    void remove_order(OrderPtr &order);

    bool empty() const { return (size_ == 0 || head_ == nullptr || head_->forward_[0] == nullptr); }
    std::list<OrderPtr> &operator[](int price) const;
    std::vector<std::string> summarize() const;

    int get_top_price() const { return front()->price_; }
    OrderPtr &get_top_order() const { return front()->innerList_.front(); }
    void pop_top_order();

    SkipList(const SkipList &) = delete;
    SkipList &operator=(const SkipList &) = delete;
    SkipList(SkipList &&) = default;
    SkipList &operator=(SkipList &&) = default;

private:
    inline int randomLevel() { return std::min(level_distribution(rng) + 1, MAX_LEVEL); }
    SkipListNode *front() const;
    void insert(int price);
    void remove(int price);
};

#endif // SKIPLIST_H