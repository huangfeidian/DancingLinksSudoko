#include <dancing_links_solver.h>
#include <iostream>
#include <algorithm>
dancing_links_solver::dancing_links_solver(std::uint32_t in_col_num, bool in_use_dlx_heap, std::uint32_t in_reserve_node_num)
: col_num(in_col_num)
, col_counter(in_col_num + 1, 0)
, avail_cols(in_col_num, 0)
, use_heap_dlx(in_use_dlx_heap)
, col_count_heap(in_col_num + 1)
{
    _nodes.reserve(in_reserve_node_num);
    for(std::uint32_t i = 0; i< col_num + 1; i++)
    {
        dancing_links_node temp_node;
        temp_node.col = i;
        temp_node.row = 0;
        temp_node.up = i;
        temp_node.down = i;
        temp_node.right = i + 1;
        temp_node.left = i - 1;
        _nodes.push_back(temp_node);
    }
    _nodes[0].left = col_num;
    _nodes[col_num].right = 0;
    row_header.push_back(0);
	tmp_removed_rows.reserve(50);
	removed_rows_total.reserve(col_num + 10);
	selected_nodes.reserve(col_num + 10);
	remove_rows_accu_count.reserve(col_num);
    col_count_heap.reset(0);
    col_count_heap.update(0, std::numeric_limits<std::uint32_t>::max());
}

std::uint32_t dancing_links_solver::add_row(const col_desc& col_pos_indexes)
{
    if(col_pos_indexes.empty())
    {
        return 0;
    }
    auto cur_node_idx = _nodes.size();
    auto next_node_idx = cur_node_idx;
    for(auto one_col: col_pos_indexes)
    {
        dancing_links_node temp_node;
        temp_node.col = one_col;
        temp_node.row = row_header.size();
        temp_node.left = next_node_idx - 1;
        temp_node.right = next_node_idx + 1;
        temp_node.up = _nodes[one_col].up;
        temp_node.down = one_col;
        next_node_idx++;
        _nodes.push_back(temp_node);
        
    }
    next_node_idx--;
    _nodes[cur_node_idx].left = next_node_idx;
    _nodes[next_node_idx].right = cur_node_idx;
    row_header.push_back(cur_node_idx);
    relink_row(row_header.size() - 1);
    return row_header.size() - 1;
}

void dancing_links_solver::relink_row(std::uint32_t row_idx)
{
    auto begin_node_idx = row_header[row_idx];
    auto end_node_idx = _nodes[row_header[row_idx]].left;
    for(auto i = begin_node_idx; i<= end_node_idx; i++)
    {
        
        auto& temp_node = _nodes[i];
        auto one_col = temp_node.col;
		_nodes[temp_node.up].down = i;
		_nodes[temp_node.down].up = i;
        col_counter[one_col]++;
        if (use_heap_dlx)
        {
            col_count_heap.update(one_col, col_counter[one_col]);
        }
        else
        {
            if (col_counter[one_col] == 1)
            {
                avail_cols[avail_col_idx] = one_col;
                avail_col_idx++;
                //std::cout << "avail_col_idx add to " << avail_col_idx << " for col " << one_col << std::endl;
            }
        }
        
    }
}
void dancing_links_solver::unlink_row(std::uint32_t row_idx)
{
    auto begin_node_idx = row_header[row_idx];
    auto end_node_idx = _nodes[row_header[row_idx]].left;
    for(auto i = begin_node_idx; i<= end_node_idx; i++)
    {
        
        auto& temp_node = _nodes[i];
        auto one_col = temp_node.col;
        _nodes[temp_node.up].down = temp_node.down;
        _nodes[temp_node.down].up = temp_node.up;
        col_counter[one_col]--;
        if (use_heap_dlx)
        {
            col_count_heap.update(one_col, col_counter[one_col]);
            if (col_counter[one_col] == 0)
            {
                col_count_heap.pop();
            }
        }
    }
}


void dancing_links_solver::select_row(std::uint32_t row_idx)
{
	tmp_removed_rows.clear();
    auto row_node_begin = row_header[row_idx];
    auto row_node_end = _nodes[row_header[row_idx]].left;
    for(auto node_idx = row_node_begin; node_idx <= row_node_end; node_idx++)
    {
        unlink_col(node_idx);
    }
	tmp_removed_rows.push_back(row_idx);
    unlink_row(row_idx);
    std::reverse(tmp_removed_rows.begin(), tmp_removed_rows.end());
    // stack to relink

}

void dancing_links_solver::unlink_col(std::uint32_t node_idx)
{
    const auto& cur_node = _nodes[node_idx];
    auto cur_row_idx = cur_node.row;
    auto cur_col_idx = cur_node.col;
    auto down_node = _nodes[cur_col_idx].down;
	// disconnect col node
	auto& cur_col_node = _nodes[cur_col_idx];
	_nodes[cur_col_node.left].right = cur_col_node.right;
	_nodes[cur_col_node.right].left = cur_col_node.left;
	// remove related rows
    while(down_node != cur_col_idx)
    {
        const auto& temp_node = _nodes[down_node];
        down_node = temp_node.down;
        if(temp_node.row != cur_row_idx)
        {
			tmp_removed_rows.push_back(temp_node.row);
            unlink_row(temp_node.row);
        }
        
    }

}
col_desc dancing_links_solver::row_to_col_desc(std::uint32_t row_idx) const
{
    col_desc result;
    auto row_node_begin = row_header[row_idx];
    auto row_node_end = _nodes[row_header[row_idx]].left;
	result.reserve(row_node_end - row_node_begin + 1);
    for(auto node_idx = row_node_begin; node_idx <= row_node_end; node_idx++)
    {
        result.push_back(_nodes[node_idx].col);
    }
    return result;
}

std::uint32_t dancing_links_solver::pick_next_col() 
{
    std::uint32_t col_idx = 0;
    std::uint32_t i = 0;
	std::uint32_t pre_min_value = 0;
	auto col_counter_p = col_counter.data();
	pick_col_counter++;
    if (use_heap_dlx)
    {
        auto cur_top = col_count_heap.top();
        return cur_top.mutual_idx;
    }
    else
    {
        while (i < avail_col_idx)
        {
            auto cur_col = avail_cols[i];
            auto cur_col_counter = col_counter_p[cur_col];
            if (cur_col_counter == 0)
            {

                avail_col_idx--;
                //std::cout << "avail_col_idx dec to " << avail_col_idx << " for col " << avail_cols[i] << std::endl;
                std::swap(avail_cols[i], avail_cols[avail_col_idx]);
            }
            else
            {
                if (col_idx == 0)
                {
                    col_idx = cur_col;
                    pre_min_value = cur_col_counter;
                }
                else
                {
                    if (pre_min_value > cur_col_counter)
                    {
                        col_idx = cur_col;
                        pre_min_value = cur_col_counter;
                    }
                }
                i++;

            }

        }
        return col_idx;
    }
    
    
        
}
std::vector<uint32_t> dancing_links_solver::solve_one()
{
	reset_solution();
    if(!solve_one_impl())
    {
        return {};
    }
    std::vector<uint32_t> result;
	result.reserve(selected_nodes.size());
    for(auto one_node_idx: selected_nodes)
    {
        const auto& cur_node = _nodes[one_node_idx];
        result.push_back(cur_node.row);
    }
    return result;

}
void dancing_links_solver::backtrace_restore()
{
	auto cur_node_idx = selected_nodes.back();
	selected_nodes.pop_back();
	auto& cur_row_idx = _nodes[cur_node_idx].row;
	auto row_node_begin = row_header[cur_row_idx];
	auto row_node_end = _nodes[row_header[cur_row_idx]].left;
	// reconnect cols
	for (auto node_idx = row_node_end; node_idx >= row_node_begin; node_idx--)
	{
		auto temp_col_idx = _nodes[node_idx].col;
		auto& cur_col_node = _nodes[temp_col_idx];
		_nodes[cur_col_node.left].right = temp_col_idx;
		_nodes[cur_col_node.right].left = temp_col_idx;
	}
	pop_remove_rows();

	
}
void dancing_links_solver::pop_remove_rows()
{
	auto after_total = remove_rows_accu_count.back();
	remove_rows_accu_count.pop_back();
	auto pre_total = 0;
	if (remove_rows_accu_count.size())
	{
		pre_total = remove_rows_accu_count.back();
	}
	for (std::uint32_t i = pre_total; i < after_total; i++)
	{
		relink_row(removed_rows_total[i]);
	}
}
void dancing_links_solver::push_remove_rows()
{
	auto cur_row_count = tmp_removed_rows.size();
	auto pre_total = 0;
	if (remove_rows_accu_count.size())
	{
		pre_total = remove_rows_accu_count.back();
	}
	std::copy(tmp_removed_rows.begin(), tmp_removed_rows.end(), removed_rows_total.data() + pre_total);
	remove_rows_accu_count.push_back(cur_row_count + pre_total);
}
bool dancing_links_solver::backtrace_and_choose_next()
{
    if(selected_nodes.empty())
    {
        // cant backtrace anymore
        return false;
    }
    //backtrace restore
	auto cur_node_idx = selected_nodes.back();
	backtrace_restore();
    auto next_node_idx = _nodes[cur_node_idx].down;
    if(next_node_idx <= col_num)
    {
        // we hit the last node of this col
        // backtrace again
        backtrace_flag = true;
    }
    else
    {
        // select next node for this col
        auto& next_node = _nodes[next_node_idx];
		select_row(next_node.row);
		push_remove_rows();
        selected_nodes.push_back(next_node_idx);
		//print_row("backtrace select", next_node.row);
    }
    return true;
}
bool dancing_links_solver::solve_one_impl()
{
    backtrace_flag = false;
    while(_nodes[0].right != 0)
    {
		auto cur_col = pick_next_col();
        if (cur_col)
        {
            print_select_nodes(selected_nodes);
        }
        if(backtrace_flag)
        {
            backtrace_flag = false;
            if(!backtrace_and_choose_next())
            {
                return false;
            }
        }
        else
        {
            if(cur_col == 0)
            {
                // cant select any col 
                // backtrace
                backtrace_flag = true;
            }
            else
            {
                auto next_node_idx = _nodes[cur_col].down;
                auto& next_node = _nodes[next_node_idx];
				select_row(next_node.row);
				push_remove_rows();
				//print_row("select", next_node.row);
                selected_nodes.push_back(next_node_idx);
            }
        }

    }
	auto cur_col = pick_next_col();
    return true;
}
std::vector<std::vector<uint32_t>> dancing_links_solver::solve_all()
{
	reset_solution();
    backtrace_flag = false;
    std::vector<std::vector<uint32_t>> all_result;
    while (true)
    {
        if(backtrace_flag)
        {
            backtrace_flag = false;
            if(!backtrace_and_choose_next())
            {
                return all_result;
            }
        }
        else
        {
            if(solve_one_impl())
            {
                std::vector<uint32_t> result;
                for(auto one_node_idx: selected_nodes)
                {
                    const auto& cur_node = _nodes[one_node_idx];
                    result.push_back(cur_node.row);
                }
                all_result.push_back(result);
            }
            backtrace_flag = true;
        }
        
    }
    
}
void dancing_links_solver::print_row(const std::string& prefix, std::uint32_t row_idx) const
{
	if (!debug_on)
	{
		return;
	}
    if(row_idx == 0 || row_idx >= row_header.size())
    {
        std::cout<<"invalid row_idx: "<<row_idx<<std::endl;
    }
    auto col_desc = row_to_col_desc(row_idx);
    std::cout<<prefix << " row "<<row_idx<<": ";
    std::vector<uint8_t> result(col_num, 0);
    for(auto one_col: col_desc)
    {
        result[one_col - 1] = 1;
    }
    for(auto one_value: result)
    {
        std::cout<<int(one_value);
    }
    std::cout<<std::endl;
}
void dancing_links_solver::print_select_nodes(const std::vector<std::uint32_t>& cur_selected_nodes) const
{
	if (!debug_on)
	{
		return;
	}
	std::cout << "selected_rows: ";
	for (auto one_node : cur_selected_nodes)
	{
		std::cout << _nodes[one_node].row << ", ";
	}
	std::cout << std::endl;

}

void dancing_links_solver::set_debug(bool debug_flag)
{
	debug_on = debug_flag;
}
void dancing_links_solver::reset_solution()
{
	while (!selected_nodes.empty())
	{
		backtrace_restore();
	}
	if (removed_rows_total.size() < row_header.size())
	{
		removed_rows_total = std::vector<std::uint32_t>(row_header.size() + 100, 0);
	}
}
void dancing_links_solver::reset_data()
{
	_nodes.clear();
	row_header.clear();
	selected_nodes.clear();
	tmp_removed_rows.clear();
	remove_rows_accu_count.clear();
	for (auto& one_col : col_counter)
	{
		one_col = 0;
	}
	avail_col_idx = 0;
	for (auto& one_col : avail_cols)
	{
		one_col = 0;
	}
    col_count_heap.reset(0);
    col_count_heap.update(0, std::numeric_limits<std::uint32_t>::max());
	for (std::uint32_t i = 0; i < col_num + 1; i++)
	{
		dancing_links_node temp_node;
		temp_node.col = i;
		temp_node.row = 0;
		temp_node.up = i;
		temp_node.down = i;
		temp_node.right = i + 1;
		temp_node.left = i - 1;
		_nodes.push_back(temp_node);
	}
	_nodes[0].left = col_num;
	_nodes[col_num].right = 0;
	row_header.push_back(0);
	
}