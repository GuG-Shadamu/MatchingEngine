/*
 * @Author: Tairan Gao
 * @Date:   2023-09-04 17:08:08
 * @Last Modified by:   Tairan Gao
 * @Last Modified time: 2023-09-05 01:35:53
 */

#include "SkipListNode.hpp"

std::string SkipListNode::summarize() const
{
    std::string summary;
    summary += std::to_string(price_) + " ";
    std::size_t total_quantity = 0;

    for (const auto &order : innerList_)
    {
        total_quantity += order->getQuantity();
    }
    summary += std::to_string(total_quantity);
    return summary;
}

SkipListNode *SkipListNodeFactory::createNode()
{
    return createNode(std::numeric_limits<int>::max());
}

SkipListNode *SkipListNodeFactory::createNode(int price)
{
    if (node_pool_.empty())
    {
        auto *raw_ptr = new SkipListNode(price);
        std::unique_ptr<SkipListNode> node(raw_ptr);
        price_to_linked_list_map_[price] = std::move(node);
        return raw_ptr;
    }
    else
    {
        auto node = std::move(node_pool_.top());
        SkipListNode *raw_ptr = node.get();
        node_pool_.pop();
        node->price_ = price;
        price_to_linked_list_map_[price] = std::move(node);
        return raw_ptr;
    }
}

void SkipListNodeFactory::removeNodeByPrice(int price)
{
    auto itor = price_to_linked_list_map_.find(price);
    node_pool_.push(std::move(itor->second));
    price_to_linked_list_map_.erase(itor);
}