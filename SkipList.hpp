/*
 * @Author: Tairan Gao
 * @Date:   2023-09-01 11:58:22
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 02:15:28
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
#include "SkipListNode.hpp"

class SkipList
{
private:
    int level_;
    size_t size_;
    bool isAscending_;
    SkipListNodeFactory factory_;
    SkipListNode *head_;
    std::unordered_map<std::size_t, std::list<Order *>::iterator> id_to_linked_list_itor_map_; // order_id -> iterator of linked-list in skip list

    std::mt19937 rng;
    std::geometric_distribution<int> level_distribution;

public:
    explicit SkipList(bool isAscending = true);
    void insertOrder(Order *order);
    void removeOrder(const Order &order);
    bool empty() const { return (size_ == 0 || head_ == nullptr || head_->forward_[0] == nullptr); }
    std::list<Order *> &operator[](int price) const { return factory_.getNodeByPrice(price)->innerList_; };
    std::vector<std::string> summarize() const;

    SkipList(const SkipList &) = delete;            // copy constructor
    SkipList &operator=(const SkipList &) = delete; // copy assignment
    SkipList(SkipList &&) = default;                // move constructor
    SkipList &operator=(SkipList &&) = default;     // move assignment

    SkipListNode *front() const { return head_->forward_[0]; };

private:
    inline int randomLevel() { return std::min(level_distribution(rng) + 1, MAX_LEVEL); }

    SkipListNode *insertPrice(int price);
    void removePrice(int price);
};

#endif // SKIPLIST_H