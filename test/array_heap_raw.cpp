#include "array_heap_raw.h"


array_heap_raw::array_heap_raw(std::uint32_t size)
: heap_size(size)
{
	_nodes =std::vector<heap_node_raw>(heap_size, heap_node_raw{0,0});
    mutual_idxes = std::vector<std::uint32_t>(heap_size, 0);
	reset();
    // todo sort heap
}
bool array_heap_raw::empty() const
{
    return remain_heap_num == 0;
}

void array_heap_raw::maintain_heap(std::uint32_t idx)
{
	std::uint32_t k, current_min;
	k = idx;
	while (2 * k + 1 < remain_heap_num)//然后，下面便是一些维护性的工作，用来维护最小堆
	{
		current_min = _nodes[k].value;
		current_min = current_min < _nodes[2 * k + 1].value ? current_min : _nodes[2 * k + 1].value;
		if (2 * k + 2 < remain_heap_num)
		{
			current_min = current_min < _nodes[2 * k + 2].value ? current_min : _nodes[2 * k + 2].value;
		}
		if (current_min == _nodes[k].value)
		{
			return;
		}
		else
		{
			if (current_min == _nodes[2 * k + 1].value)
			{
				swap_node(k, 2 * k + 1);
				k = 2 * k + 1;
			}
			else
			{
				swap_node(k, 2 * k + 2);
				k = 2 * k + 2;
			}
		}
	}
}
void array_heap_raw::pop()
{
	int k;
	int current_min;
	if (remain_heap_num != 1)
	{
		swap_node(0, remain_heap_num - 1);//交换最高元素与最低元素
		remain_heap_num--;//然后将堆的大小进行缩减
		maintain_heap(0);
	}
	else//如果只剩下一个元素，那就不需要进行交换，直接将堆元素的个数降低一
	{
		remain_heap_num = 0;
	}
}
void array_heap_raw::swap_node(std::uint32_t index_one, std::uint32_t index_two)
{
	if (index_one == index_two)
	{
		return;
	}


	std::uint32_t intermidate_one, intermidate_two;
	intermidate_one = _nodes[index_one].value;
	intermidate_two = _nodes[index_one].mutual_idx;

	_nodes[index_one].value = _nodes[index_two].value;
	_nodes[index_one].mutual_idx = _nodes[index_two].mutual_idx;
	_nodes[index_two].value = intermidate_one;
	_nodes[index_two].mutual_idx = intermidate_two;
	mutual_idxes[_nodes[index_two].mutual_idx] = index_two;
	mutual_idxes[_nodes[index_one].mutual_idx] = index_one;
}
std::uint32_t array_heap_raw::top() const
{
    return _nodes[0].mutual_idx;
}

void array_heap_raw::update_by_dec(std::uint32_t pos)
{
	auto k = mutual_idxes[pos];
	auto& cur_node = _nodes[k];
	if(cur_node.value == 0)
	{
		return;
	}
	cur_node.value--;
	while (k > 0 && (_nodes[(k - 1) / 2].value > _nodes[k].value))//维护堆
	{
		swap_node((k - 1) / 2, k);
		k = (k - 1) / 2;
	}
	if (_nodes[k].value == 0)//如果是赋值为0，则从堆中进行删除，因为我们每次操纵一个元素，所以最多会有一个元素为0，所以肯定是最小值。
	{
		pop();
	}
}

void array_heap_raw::update_by_inc(std::uint32_t pos)
{
	auto k = mutual_idxes[pos];
	auto& cur_node = _nodes[k];
	cur_node.value++;
	if (cur_node.value == 1)//如果新的值是1，则把这个元素重新加入堆中
	{
		
		swap_node(k, remain_heap_num);
		k = remain_heap_num;
		while (k > 0 && (_nodes[(k - 1) / 2].value > _nodes[k].value))//由于新的值是1，所以不可能比上一个数大
		{
			swap_node((k - 1) / 2, k);
			k = (k - 1) / 2;
		}
		remain_heap_num++;//扩大堆的范围
	}
	else//如果不是1，说明已经在堆中，所以不需要扩大堆的范围，直接赋值之后进行维护堆结构就行
	{
		maintain_heap(k);
	}
}

std::ostream& operator<<(std::ostream& output, const array_heap_raw& data)
{
	for(auto& one_node:data._nodes)
	{
		output<<"("<<one_node.mutual_idx<<","<<one_node.value<<")";
	}
	return output;
}
void array_heap_raw::reset()
{
	auto node_begin = _nodes.data();
	for (int i = 0; i < heap_size; i++)
	{
		node_begin->value = 0;
		node_begin->mutual_idx = i;
		
		node_begin++;
		
	}
	auto idx_begin = mutual_idxes.data();
	for (int i = 0; i < heap_size; i++)
	{
		*idx_begin = i;
		idx_begin++;
	}
	avail_size = 1;
	remain_heap_num = 0;
}
std::uint32_t array_heap_raw::remain_size() const
{
	return remain_heap_num;
}
void array_heap_raw::set_data(const std::uint32_t* _init_values)
{

	for (std::uint32_t i = 0; i < heap_size; i++)
	{
		_nodes[i].value = _init_values[i];
		_nodes[i].mutual_idx = i;
		mutual_idxes[i] = i;
	}
	avail_size = heap_size + 1;
	remain_heap_num = heap_size;
	if (heap_size < 2)
	{
		return;
	}
	for (std::uint32_t i = heap_size / 2; i != 0; i--)
	{
		maintain_heap(i - 1);
	}

}