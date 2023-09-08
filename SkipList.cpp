/*
 * @Author: Tairan Gao
 * @Date:   2023-09-01 13:06:57
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 02:16:28
 */

#include "SkipList.hpp"
#include "SkipListNode.hpp"

const float P = 0.5;

SkipList::SkipList(bool isAscending)
        : isAscending_(isAscending), size_(0), level_(1), level_distribution(P)
{
    head_ = factory_.createNode();
    std::random_device rd;
    rng.seed(rd());

    std::fill(head_->forward_.begin(), head_->forward_.end(), nullptr);
    std::fill(head_->backward_.begin(), head_->backward_.end(), nullptr);
}

SkipListNode *SkipList::insertPrice(int price)
{
    SkipListNode *new_node = factory_.createNode(price);
    SkipListNode *current_node = head_;
    std::array<SkipListNode *, MAX_LEVEL> before{};
    std::array<SkipListNode *, MAX_LEVEL> after{};
    before.fill(nullptr);
    after.fill(nullptr);

    if (isAscending_)
    {
        for (int i = level_ - 1; i >= 0; --i)
        {
            while (current_node->forward_[i] && current_node->forward_[i]->price_ < price)
            {
                current_node = current_node->forward_[i];
            }
            before[i] = current_node;
            if (current_node->forward_[i])
            {
                after[i] = current_node->forward_[i];
            }
        }
    }
    else
    {
        for (int i = level_ - 1; i >= 0; --i)
        {
            while (current_node->forward_[i] && current_node->forward_[i]->price_ > price)
            {
                current_node = current_node->forward_[i];
            }
            before[i] = current_node;
            if (current_node->forward_[i])
            {
                after[i] = current_node->forward_[i];
            }
        }
    }

    int new_level = randomLevel();

    // If the new level is greater than the list's current level,
    // initialize the 'before' and 'after' arrays for the new levels
    if (new_level > level_)
    {
        for (int i = level_; i < new_level; ++i)
        {
            before[i] = head_;
            after[i] = nullptr;
        }
        level_ = new_level;
    }

    for (int i = 0; i < new_level; ++i)
    {
        new_node->forward_[i] = after[i];
        if (before[i])
        {
            before[i]->forward_[i] = new_node;
        }

        if (after[i])
        {
            after[i]->backward_[i] = new_node;
        }
        new_node->backward_[i] = before[i];
    }
    size_++;
    return new_node;
}

void SkipList::removePrice(int price)
{
    if (!factory_.hasPrice(price))
    {
        throw std::runtime_error("Price not found");
    }

    SkipListNode *current_node = factory_.getNodeByPrice(price);
    size_t node_level = current_node->forward_.size();

    for (int i = 0; i < node_level; ++i)
    {
        if (current_node->backward_[i])
        {
            current_node->backward_[i]->forward_[i] = current_node->forward_[i];
        }
        if (current_node->forward_[i])
        {
            current_node->forward_[i]->backward_[i] = current_node->backward_[i];
        }
    }

    factory_.removeNodeByPrice(price);
}

void SkipList::insertOrder(Order *order)
{
    int price = order->getPrice();
    SkipListNode *node;

    if (!factory_.hasPrice(price))
    {
        node = insertPrice(price);
    }
    else
    {
        node = factory_.getNodeByPrice(price);
    }
    node->innerList_.push_back(order);
    node->size_++;
    id_to_linked_list_itor_map_[order->getOrderId()] = std::prev(node->innerList_.end());
}
void SkipList::removeOrder(const Order &order)
{
    // remove from Linked-List
    auto itor = id_to_linked_list_itor_map_.find(order.getOrderId());
    if (itor == id_to_linked_list_itor_map_.end())
    {
        throw std::runtime_error("Order not found");
    }
    SkipListNode *node = factory_.getNodeByPrice(order.getPrice());
    node->innerList_.erase(itor->second);
    node->size_--;
    id_to_linked_list_itor_map_.erase(itor);

    // remove the linked-list node if no order left
    if (node->size_ == 0)
    {
        int price = order.getPrice();
        removePrice(price);
    }
}

std::vector<std::string> SkipList::summarize() const
{
    if (empty())
    {
        return {};
    }
    std::vector<std::string> summary;
    SkipListNode *skipListNode = head_->forward_[0];
    while (skipListNode)
    {
        summary.push_back(skipListNode->summarize());
        skipListNode = skipListNode->forward_[0];
    }
    return summary;
}