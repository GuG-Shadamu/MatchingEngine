/*
 * @Author: Tairan Gao
 * @Date:   2023-09-01 13:06:57
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-01 14:56:50
 */

#include "SkipList.hpp"

const float P = 0.5;

std::string SkipList::SkipListNode::summarize() const
{
    std::string summary;
    summary += std::to_string(price_) + " ";
    unsigned int total_quantity = 0;

    for (const auto &order : innerList_)
    {
        total_quantity += order->getQuantity();
    }
    summary += std::to_string(total_quantity);
    return summary;
}

SkipList::SkipList(bool isAscending)
        : isAscending_(isAscending), size_(0), level_(1), level_distribution(P)
{
    head_ = new SkipListNode();
    std::random_device rd;
    rng.seed(rd());

    std::fill(head_->forward_.begin(), head_->forward_.end(), nullptr);
    std::fill(head_->backward_.begin(), head_->backward_.end(), nullptr);
}

SkipList::~SkipList()
{
    SkipListNode *current = head_;
    while (current)
    {
        SkipListNode *next = current->forward_[0];
        delete current;
        current = next;
    }
}

void SkipList::insert(int price)
{
    auto *new_node = new SkipListNode(price);
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
    price_to_linked_list_map_[price] = new_node;
}

void SkipList::remove(int price)
{
    if (price_to_linked_list_map_.find(price) == price_to_linked_list_map_.end())
    {
        throw std::runtime_error("Price not found");
    }

    SkipListNode *current_node = price_to_linked_list_map_[price];
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

    size_--;
    price_to_linked_list_map_.erase(price);
    delete current_node;
}

void SkipList::insert_order(const OrderPtr& order)
{
    int price = order->getPrice();
    SkipListNode *node;
    if (price_to_linked_list_map_.find(price) == price_to_linked_list_map_.end())
    {
        insert(price);
    }
    node = price_to_linked_list_map_[price];
    node->innerList_.push_back(order);
    node->size_++;
    id_to_linked_list_itor_map_[order->getOrderId()] = std::prev(node->innerList_.end());
}

void SkipList::remove_order(OrderPtr &order)
{
    // remove from Linked-List
    auto itor = id_to_linked_list_itor_map_.find(order->getOrderId());
    if (itor == id_to_linked_list_itor_map_.end())
    {
        throw std::runtime_error("Order not found");
    }
    SkipListNode *node = price_to_linked_list_map_[order->getPrice()];
    node->innerList_.erase(itor->second);
    node->size_--;
    id_to_linked_list_itor_map_.erase(itor);

    // remove the linked-list node if no order left
    if (node->size_ == 0)
    {
        int price = order->getPrice();
        remove(price);
    }
}

SkipList::SkipListNode *SkipList::front() const
{
    if (empty())
    {
        return nullptr;
    }

    return head_->forward_[0];
}

std::list<OrderPtr> &SkipList::operator[](int price) const
{
    auto it = price_to_linked_list_map_.find(price);
    if (it != price_to_linked_list_map_.end())
    {
        return it->second->innerList_;
    }
    throw std::out_of_range("Price not found in SkipList");
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

void SkipList::pop_top_order()
{
    if (empty())
    {
        throw std::runtime_error("Empty list");
    }

    OrderPtr &top_order = get_top_order();
    remove_order(top_order);
}
