#pragma once
#include <vector>
#include <cstdint>
#include <iostream>

template <typename T>
struct heap_node
{
	T value;
	std::uint32_t mutual_idx;//
};
template <typename T>
class array_heap
{
private:
	std::vector<heap_node<T>> _nodes;
	std::vector<std::uint32_t> mutual_idxes;
public:
	array_heap(std::uint32_t size)
		: heap_size(size)
		, _nodes(size)
		, mutual_idxes(size)
		, remain_heap_num(0)
	{

	}
	bool empty() const
	{
		return remain_heap_num == 0;
	}
	void update(std::uint32_t pos, T value)
	{
		if (mutual_idxes[pos] >= remain_heap_num)
		{
			if (mutual_idxes[pos] != remain_heap_num)
			{
				swap_node(mutual_idxes[pos], remain_heap_num);
			}
			_nodes[remain_heap_num].value = value;
			remain_heap_num++;
			on_key_decrease(mutual_idxes[pos]);
			return;
		}
		auto pre_value = _nodes[mutual_idxes[pos]].value;
		_nodes[mutual_idxes[pos]].value = value;
		if (pre_value == value)
		{
			return;
		}
		if (pre_value < value)
		{
			on_key_increase(mutual_idxes[pos]);
		}
		else
		{
			on_key_decrease(mutual_idxes[pos]);
		}
	}
	void update_by_delta(std::uint32_t pos, T delta)
	{
		if (mutual_idxes[pos] >= remain_heap_num)
		{
			return;
		}
		auto pre_value = _nodes[mutual_idxes[pos]].value;
		auto new_value = pre_value + delta;
		_nodes[mutual_idxes[pos]].value = new_value;
		if (pre_value == new_value)
		{
			return;
		}
		if (pre_value < new_value)
		{
			on_key_increase(mutual_idxes[pos]);
		}
		else
		{
			on_key_decrease(mutual_idxes[pos]);
		}
	}
	heap_node<T> top() const
	{
		return _nodes[0];
	}
	std::uint32_t remain_size() const
	{
		return remain_heap_num;
	}
	T get(std::uint32_t idx) const
	{
		if (idx >= heap_size)
		{
			return {};
		}
		else
		{
			return _nodes[mutual_idxes[idx]].value;
		}
	}
	void pop()
	{
		if (remain_heap_num != 1)
		{
			swap_node(0, remain_heap_num - 1);//交换最高元素与最低元素
			remain_heap_num--;//然后将堆的大小进行缩减
			on_key_increase(0);
		}
		else//如果只剩下一个元素，那就不需要进行交换，直接将堆元素的个数降低一
		{
			remain_heap_num = 0;
		}
	}
	friend std::ostream& operator<<(std::ostream& output, const array_heap& data)
	{
		for(std::size_t i = 0; i < data.remain_heap_num; i++)
		{
			const auto& one_node = data._nodes[i];
			output << "(" << one_node.mutual_idx << "," << one_node.value << ")";
		}
		return output;
	}
	void reset(T max_value)
	{
		remain_heap_num = 0;
		for (std::size_t i = 0; i < heap_size; i++)
		{
			_nodes[i].value = max_value;
			_nodes[i].mutual_idx = i;
			mutual_idxes[i] = i;
		}
	}
	void set_array(T* init_data)
	{
		reset({});
		for (std::size_t i = 0; i < heap_size; i++)
		{
			update(i, init_data[i]);
		}
	}
private:
	void swap_node(std::uint32_t index_one, std::uint32_t index_two)
	{
		if (index_one == index_two)
		{
			return;
		}


		std::uint32_t intermidate_idx;
		T intermidate_value;
		intermidate_value = _nodes[index_one].value;
		intermidate_idx = _nodes[index_one].mutual_idx;

		_nodes[index_one].value = _nodes[index_two].value;
		_nodes[index_one].mutual_idx = _nodes[index_two].mutual_idx;
		_nodes[index_two].value = intermidate_value;
		_nodes[index_two].mutual_idx = intermidate_idx;
		mutual_idxes[_nodes[index_two].mutual_idx] = index_two;
		mutual_idxes[_nodes[index_one].mutual_idx] = index_one;
	}
	std::uint32_t avail_size = 0;
	const std::uint32_t heap_size = 0;
	std::uint32_t remain_heap_num = 0;
	void on_key_decrease(std::uint32_t idx)
	{
		std::uint32_t k = idx;
		while (k > 0 && (_nodes[k].value < _nodes[(k - 1) / 2].value))//维护堆
		{
			swap_node((k - 1) / 2, k);
			k = (k - 1) / 2;
		}
	}
	void on_key_increase(std::uint32_t idx)
	{
		std::uint32_t k;
		T current_min;
		k = idx;

		while (2 * k + 1 < remain_heap_num)//然后，下面便是一些维护性的工作，用来维护最小堆
		{
			current_min = _nodes[k].value;
			std::uint32_t next_k = k;
			if (_nodes[2 * k + 1].value < current_min)
			{
				current_min = _nodes[2 * k + 1].value;
				next_k = 2 * k + 1;
			}
			if (2 * k + 2 < remain_heap_num && (_nodes[2 * k + 2].value < current_min))
			{
				next_k = 2 * k + 2;
			}
			if (next_k != k)
			{
				swap_node(k, next_k);
				k = next_k;
			}
			else
			{
				break;
			}

		}
	}
};



