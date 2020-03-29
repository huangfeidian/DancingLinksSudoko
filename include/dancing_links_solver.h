#pragma once
#include <vector>
#include <unordered_map>
#include <cstdint>
#include <string>

struct dancing_links_node
{
    std::uint32_t up;
    std::uint32_t down;
    std::uint32_t left;
    std::uint32_t right;
    std::uint32_t col; // col = 0 for header col
    std::uint32_t row; // row =0 for header row
};

using col_desc = std::vector<std::uint32_t>;

class dancing_links_solver
{
protected:
    std::vector<dancing_links_node> _nodes;
    std::vector<std::uint32_t> row_header;
    std::vector<std::uint32_t> col_counter;
	bool debug_on = false;
	std::vector<std::uint32_t> selected_nodes;// chosed node 
	std::vector<std::vector<std::uint32_t>> removed_rows;
	bool backtrace_flag = false;
public:
    const std::uint32_t col_num;
    dancing_links_solver(std::uint32_t col_num, std::uint32_t reserve_node_num = 1000);
    std::uint32_t add_row(const col_desc& col_pos_idxes);
    std::vector<uint32_t> solve_one();
    std::vector<std::vector<uint32_t>> solve_all();
    virtual void print_row(const std::string& prefix, std::uint32_t row_idx) const;
	virtual void print_select_nodes(const std::vector<std::uint32_t>& cur_selected_nodes) const;
    col_desc row_to_col_desc(std::uint32_t row_idx) const;
	void set_debug(bool debug_flag);
protected:
    void unlink_row(std::uint32_t row_idx);
    void relink_row(std::uint32_t row_idx);
    std::uint32_t pick_next_col() const; //select col which has the smallest col count return 0 if fail
    
    // remove all rows of current column except the row _nodes[node_idx].row
    void unlink_col(std::uint32_t node_idx, std::vector<std::uint32_t>& remove_rows);
    // remove all rows that share column with current row_idx
    // row_idx remove at the last
    // return the reverse order of removed rows, for the convinience of relink
    std::vector<std::uint32_t> select_row(std::uint32_t row_idx);
    bool solve_one_impl();
    bool backtrace_and_choose_next();
	void backtrace_restore();

	void reset();
};
