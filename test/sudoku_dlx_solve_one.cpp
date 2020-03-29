#include "sudoku_dlx_solver.h"

using namespace std;

int main()
{
    sudoku_configuration instance;
    string raw_data = "000000010400000000020000000000050407008000300001090000300400200050100000000806000";
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            instance.data[i][j] = raw_data[i * 9 + j] - '0';
        }
    }
	cout << instance << endl;
    sudoku_dlx_solver cur_solver(instance);
	cur_solver.set_debug(false);
    auto result = cur_solver.solve_one();
    std::cout<<result<<std::endl;
	std::cout << "solve one finish" << std::endl;
	auto all_result = cur_solver.solve_all();
	std::cout << "test case has " << all_result.size() << " solutions" << std::endl;
	for (const auto& one_solution : all_result)
	{
		std::cout << "solution begin" << std::endl;
		std::cout << one_solution << std::endl;

	}

}