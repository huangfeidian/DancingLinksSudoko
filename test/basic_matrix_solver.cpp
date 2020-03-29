#include <dancing_links_solver.h>
#include <array>
#include <iostream>

using namespace std;
template <std::uint32_t C, std::uint32_t R>
void matrix_solver(const array<array<uint8_t, C>, R>& matrix_data)
{
    auto temp_solver = dancing_links_solver(C, C * (R + 2));
    for(const auto& one_row: matrix_data)
    {
        col_desc cur_row;
        for(uint32_t i = 0; i < C; i++)
        {
            if(one_row[i])
            {
                cur_row.push_back(i + 1);
            }
        }
		temp_solver.add_row(cur_row);

    }
    auto result = temp_solver.solve_one();
    for(auto one_row: result)
    {
        temp_solver.print_row("result", one_row);
    }
    cout<<endl;

}

int main()
{
    array<array<uint8_t, 7>, 6> data = {
		array<uint8_t, 7>{0,0,1,0,1,1,0},
		array<uint8_t, 7>{1,0,0,1,0,0,1},
		array<uint8_t, 7>{0,1,1,0,0,1,0},
		array<uint8_t, 7>{1,0,0,1,0,0,0},
		array<uint8_t, 7>{0,1,0,0,0,0,1},
		array<uint8_t, 7>{0,0,0,1,1,0,1}
    };
    matrix_solver(data);
}