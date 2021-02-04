#include <iostream>
#include <vector>
#include <stack>
#include <map>
#include <ctime>
#include <fstream>
#include <string>
#include <array_heap.h>
#include "sudoku_heap_dlx_solver.h"

using namespace std;

int main()
{
	clock_t clock_one, clock_two, clock_three;
	ifstream suduko_file("../data/sudoku.txt");
	ofstream result_sudoku_file("sudoku_result_raw.txt");
	char temp[82];
	clock_one = clock();
	int line = 1;
	std::vector<std::string> total_result;
	total_result.reserve(50000);
	std::uint32_t case_size = 49152;
	sudoku_dlx_heap_solver cur_solver;
	//case_size = 2;
	while (line!= case_size)
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
		creat_dlx_sudoku();
		seek_sudoku();
		total_result.push_back(result_to_line());
		line++;
	}
	clock_three = clock();
	for (const auto& one_line : total_result)
	{
		result_sudoku_file << one_line << endl;
	}
	result_sudoku_file.close();
	printf("%d mscond passed in seek_sudoku\n", clock_three - clock_one);
}