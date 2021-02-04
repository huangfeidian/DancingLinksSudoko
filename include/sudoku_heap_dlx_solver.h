#include "sudoku_dlx_solver.h"
#include "array_heap.h"

class sudoku_dlx_heap_solver
{
	struct basic_node
	{
		//这里之所以没有left和right是因为我们每次分配一行的时候，是四个点一起分的，所以可以直接通过加减1来搞定左右关系
		int down;
		int up;
		int column;
	};
public:
	sudoku_dlx_heap_solver();
	std::optional<sudoku_configuration> solve_one();
	std::vector<sudoku_configuration> solve_all();
	void set_configure(const sudoku_configuration& configure);

private:
	struct basic_node total_nodes[324 + 81 * 9 * 4];//324个头节点，81个格子，每个格子有9种情况，每种情况有四个点。
	int avail_node_index = 324;//分配节点时的编号
	int node_stack[81];
	int stack_index = 0;
	std::uint32_t column_counter[324] = { 0 };
	int available_column = 323;//这个是当前可用列数
	array_heap<std::uint32_t> cur_heap;
	int out[9][9];
public:
	void initial(void);

	void print_heap();

	void node_heap_decrease(int node_index);//对于一个点进行她所在的行的删除，因为一行中一定有四个元素，所以有四列，我们对这四列的度数都进行减少1

	void node_heap_increase(int node_index);//增加与减少的顺序是刚好相反的
	void insert_one_node(std::uint32_t node_idx, std::uint32_t column_index);

	void insert_row(int current_row_index, int current_column_index, int value);


	std::string result_to_line();

	void print_result();//打印出结果


	void creat_dlx_sudoku();//利用矩阵来建立十字网格

	void in_stack(int target_to_stack);

	void out_stack();//注意出栈的时候是相反的操作，所有删除都相反

	int find_next();//用来找下一个可以入栈的元素，如果无法入栈或者已经找到了解，则返回并进行回溯操作

	void seek_sudoku();

};