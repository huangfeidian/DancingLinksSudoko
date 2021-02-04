#pragma once
#include <array>
#include <iostream>
#include <optional>
#include "dancing_links_solver.h"

struct sudoku_configuration
{
	std::array<std::array<std::uint8_t, 9>, 9> data = {};// 0 for empty
    friend std::ostream& operator<<(std::ostream& output, const sudoku_configuration& configuration)
    {
        for(const auto& one_row: configuration.data)
        {
            for(auto one_cell: one_row)
            {
                output<<int(one_cell)<<",";
            }
            output<<std::endl;
        }
        return output;
    }
    bool is_complete() const
    {
        for(const auto& one_row: data)
        {
            for(auto one_cell: one_row)
            {
                if(!one_cell)
                {
                    return false;
                }
            }
        }
        return true;
    }
};
class sudoku_dlx_solver: public dancing_links_solver
{
public:
    sudoku_dlx_solver(bool use_dlx_heap = false);
    std::optional<sudoku_configuration> solve_one();
    std::vector<sudoku_configuration> solve_all();
    sudoku_configuration col_desc_to_sudoku(const std::vector<std::uint32_t>& row_indexes) const;
	void print_row(const std::string& prefix, std::uint32_t row_idx) const;
	void print_select_nodes(const std::vector<std::uint32_t>& cur_selected_nodes) const;
	void reset_data(const sudoku_configuration& configure);
	void set_configure(const sudoku_configuration& configure);

private:
	void pos_val_to_row(std::uint32_t i, std::uint32_t j, std::uint8_t val, col_desc& result) const;
};