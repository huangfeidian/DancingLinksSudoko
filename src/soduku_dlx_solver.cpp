#include <sudoku_dlx_solver.h>

using namespace std;

sudoku_dlx_solver::sudoku_dlx_solver(const sudoku_configuration& configure)
: dancing_links_solver(324, 2000)
{
	row_header.reserve(400);
	selected_nodes.reserve(100);
	removed_rows.reserve(400);
    for(std::uint32_t i = 0; i< 9; i++)
    {
        for(std::uint32_t j = 0; j<9; j++)
        {
            auto cur_value = configure.data[i][j];
            if(cur_value == 0)
            {
				continue;
            }
			auto cur_col_desc = pos_val_to_row(i, j, cur_value);
			add_row(cur_col_desc);
			
        }
    }
	//std::vector<std::uint32_t> pre_config;
	//for (int i = 1; i < row_header.size(); i++)
	//{
	//	pre_config.push_back(row_header[i]);
	//}
	//std::cout << "initial sudoku config is" << std::endl;
	//debug_on = true;
	//print_select_nodes(pre_config);
	//debug_on = false;
	std::vector<std::uint32_t> temp_col_counter = col_counter;
	for (std::uint32_t i = 0; i < 9; i++)
	{
		for (std::uint32_t j = 0; j < 9; j++)
		{
			auto cur_value = configure.data[i][j];
			if (cur_value != 0)
			{
				continue;
			}
			for (std::uint32_t k = 1; k <= 9; k++)
			{
				auto cur_col_desc = pos_val_to_row(i, j, k);
				bool invalid_val = false;
				for (auto one_col : cur_col_desc)
				{
					if (temp_col_counter[one_col])
					{
						invalid_val = true;
						break;
					}
				}
				if (!invalid_val)
				{
					add_row(cur_col_desc);
				}
			}

		}
	}
	
    //std::cout<<"after init nodes num is "<<_nodes.size() <<" row size is:" <<row_header.size()<<std::endl;
}
col_desc sudoku_dlx_solver::pos_val_to_row(std::uint32_t i, std::uint32_t j, std::uint8_t cur_value) const
{
	col_desc cur_desc(4, 0);
	// position cover
	cur_desc[0] = (1 + i * 9 + j);
	// number in row cover
	cur_desc[1] = (82 + i * 9 + cur_value - 1);
	// number in col cover
	cur_desc[2] = (163 + j * 9 + cur_value - 1);
	// small square cover
	cur_desc[3] = (244 + (((i / 3) * 3) + j / 3) * 9 + cur_value - 1);
	return cur_desc;
}

sudoku_configuration sudoku_dlx_solver::col_desc_to_sudoku(const std::vector<std::uint32_t>& row_indexes) const
{
    sudoku_configuration result;
	for (std::uint32_t i = 0; i < 9; i++)
	{
		for (std::uint32_t j = 0; j < 9; j++)
		{
			result.data[i][j] = 0;
		}

	}
    if(row_indexes.empty())
    {
        return result;
    }
    for(auto row_idx: row_indexes)
    {
        col_desc cur_row_info = row_to_col_desc(row_idx);
        std::uint32_t pos_idx = cur_row_info[0] - 1;
        std::uint32_t cur_value = (cur_row_info[1] - 1) % 9 + 1;
        result.data[pos_idx / 9][pos_idx % 9] = cur_value;
    }
    return result;
}
std::optional<sudoku_configuration> sudoku_dlx_solver::solve_one()
{
    auto row_result = dancing_links_solver::solve_one();
	if (row_result.empty())
	{
		return {};
	}
    return col_desc_to_sudoku(row_result);
}
std::vector<sudoku_configuration> sudoku_dlx_solver::solve_all()
{
    std::vector<sudoku_configuration> result;
    for(auto& one_result: dancing_links_solver::solve_all())
    {
        result.push_back(col_desc_to_sudoku(one_result));
    }
    return result;
}
void sudoku_dlx_solver::print_row(const std::string& prefix, std::uint32_t row_idx) const
{
	if (!debug_on)
	{
		return;
	}
	if (row_idx == 0 || row_idx >= row_header.size())
	{
		std::cout << "invalid row_idx: " << row_idx << std::endl;
	}
	auto col_desc = row_to_col_desc(row_idx);
	std::cout << prefix << " row " << row_idx << ": ";
	std::uint32_t pos_idx = col_desc[0] - 1;
	std::uint32_t cur_value = (col_desc[1] - 1) % 9 + 1;
	std::cout << "r: " << pos_idx / 9 << " c:" << (pos_idx % 9) << " v: " << cur_value << std::endl;
}
void sudoku_dlx_solver::print_select_nodes(const std::vector<std::uint32_t>& cur_selected_nodes) const
{
	if (!debug_on)
	{
		return;
	}

	std::cout << "selected_nodes configuration with node size "<< cur_selected_nodes.size()<<std::endl;
	sudoku_configuration cur_config;
	for (auto one_node : cur_selected_nodes)
	{
		auto row_idx = _nodes[one_node].row;
		auto col_desc = row_to_col_desc(row_idx);
		std::uint32_t pos_idx = col_desc[0] - 1;
		std::uint32_t cur_value = (col_desc[1] -1)  % 9 + 1;
		cur_config.data[pos_idx / 9][pos_idx % 9] = cur_value;
	}
	std::cout <<cur_config<< std::endl;

}
