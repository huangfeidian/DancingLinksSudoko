﻿#pragma once
#include <vector>
#include <cstdint>
#include <iostream>

struct heap_node
{
    std::uint32_t value;
	std::uint32_t mutual_idx;//
};
class array_heap
{
public:
    std::vector<heap_node> _nodes;
    std::vector<std::uint32_t> mutual_idxes;
public:
    array_heap(std::uint32_t size);
    bool empty() const;
    void update_by_inc(std::uint32_t pos);
    void update_by_dec(std::uint32_t pos);
    std::uint32_t top() const;
	std::uint32_t remain_size() const;
    void pop();
    friend std::ostream& operator<<(std::ostream& output, const array_heap& data);
	void reset();
private:
    void swap_node(std::uint32_t index_1, std::uint32_t index_2);
    std::uint32_t avail_size = 0;
    const std::uint32_t heap_size = 0;
    std::uint32_t remain_heap_num = 0;
    void maintain_heap(std::uint32_t idx);
};