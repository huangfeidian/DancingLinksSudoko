#include "sudoku_dlx_solver.h"
#include "array_heap.h"

class sudoku_dlx_heap_solver
{
	struct basic_node
	{
		//����֮����û��left��right����Ϊ����ÿ�η���һ�е�ʱ�����ĸ���һ��ֵģ����Կ���ֱ��ͨ���Ӽ�1���㶨���ҹ�ϵ
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
	struct basic_node total_nodes[324 + 81 * 9 * 4];//324��ͷ�ڵ㣬81�����ӣ�ÿ��������9�������ÿ��������ĸ��㡣
	int avail_node_index = 324;//����ڵ�ʱ�ı��
	int node_stack[81];
	int stack_index = 0;
	std::uint32_t column_counter[324] = { 0 };
	int available_column = 323;//����ǵ�ǰ��������
	array_heap<std::uint32_t> cur_heap;
	int out[9][9];
public:
	void initial(void);

	void print_heap();

	void node_heap_decrease(int node_index);//����һ������������ڵ��е�ɾ������Ϊһ����һ�����ĸ�Ԫ�أ����������У����Ƕ������еĶ��������м���1

	void node_heap_increase(int node_index);//��������ٵ�˳���Ǹպ��෴��
	void insert_one_node(std::uint32_t node_idx, std::uint32_t column_index);

	void insert_row(int current_row_index, int current_column_index, int value);


	std::string result_to_line();

	void print_result();//��ӡ�����


	void creat_dlx_sudoku();//���þ���������ʮ������

	void in_stack(int target_to_stack);

	void out_stack();//ע���ջ��ʱ�����෴�Ĳ���������ɾ�����෴

	int find_next();//��������һ��������ջ��Ԫ�أ�����޷���ջ�����Ѿ��ҵ��˽⣬�򷵻ز����л��ݲ���

	void seek_sudoku();

};