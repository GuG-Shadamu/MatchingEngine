/*
 * @Author: Tairan Gao
 * @Date:   2023-09-04 17:04:31
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 01:42:14
 */

#ifndef SKIPLISTNODE_H
#define SKIPLISTNODE_H

#include <string>
#include <array>
#include <list>
#include <memory>
#include <limits>
#include <unordered_map>
#include <stack>

#include "Order.hpp"
constexpr int MAX_LEVEL = 5;

class SkipList;            // forward declaration
class SkipListNodeFactory; // forward declaration

class SkipListNode
{
public:
    friend class SkipListNodeFactory;
    friend class SkipList;

public:
    SkipListNode *next() const { return forward_[0]; }
    const std::list<Order *> &getList() const { return innerList_; }
    int getPrice() const { return price_; }

    ~SkipListNode() = default;
    SkipListNode(const SkipListNode &) = delete;
    SkipListNode &operator=(const SkipListNode &) = delete;
    SkipListNode(SkipListNode &&) = delete;
    SkipListNode &operator=(SkipListNode &&) = delete;

private:
    int price_;
    size_t size_;

    std::list<Order *> innerList_;
    std::array<SkipListNode *, MAX_LEVEL> forward_{};
    std::array<SkipListNode *, MAX_LEVEL> backward_{};
    explicit SkipListNode(Order *order) : price_(order->getPrice()), innerList_({order}), size_(1) {}
    explicit SkipListNode(int price) : price_(price), size_(0) {}
    std::string summarize() const;
};

class SkipListNodeFactory
{
public:
    friend class SkipList;

public:
    SkipListNode *createNode();
    SkipListNode *createNode(int price);
    void removeNodeByPrice(int price);
    SkipListNode *getNodeByPrice(int price) const { return price_to_linked_list_map_.find(price)->second.get(); }
    bool hasPrice(int price) { return price_to_linked_list_map_.find(price) != price_to_linked_list_map_.end(); }

private:
    std::unordered_map<int, std::unique_ptr<SkipListNode>> price_to_linked_list_map_; // price -> linked-list node
    std::stack<std::unique_ptr<SkipListNode>> node_pool_;
};

#endif // SKIPLISTNODE_H