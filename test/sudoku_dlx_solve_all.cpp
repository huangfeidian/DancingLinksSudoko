#include <fstream>
#include <sudoku_dlx_solver.h>
#include <ctime>

using namespace std;

int main()
{
    clock_t clock_one, clock_two, clock_three;
	ifstream suduko_file("../data/sudoku.txt");
	char temp[82];
	clock_one = clock();
	int line = 1;
	sudoku_dlx_solver cur_solver;
	while (line!= 49152)
	{
		suduko_file.getline(temp, 82);
        sudoku_configuration instance;
		for (int i = 0; i < 9; i++)
		{
			for (int j = 0; j < 9; j++)
			{
				instance.data[i][j] = temp[i * 9 + j] - '0';
			}
		}
		cur_solver.set_configure(instance);
        auto result = cur_solver.solve_one();
		if (!result)
		{
			std::cout << "fail to solve case " << line << std::endl;
			std::cout << instance << std::endl;
		}
		line++;
	}
	clock_three = clock();
    cout<<"solver all sudoku in "<<clock_three - clock_one<<"ms with pick col counter "<< cur_solver.pick_col_counter<< endl;
}