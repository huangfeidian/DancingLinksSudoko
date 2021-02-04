#include "sudoku_heap_dlx_solver.h"

sudoku_dlx_heap_solver::sudoku_dlx_heap_solver()
	: cur_heap(324)
{

}
void sudoku_dlx_heap_solver::initial()
{
	for (int i = 0; i < 324; i++)
	{
		total_nodes[i].column = i;
		total_nodes[i].down = i;
		total_nodes[i].up = i;
		column_counter[i] = 0;

	}
	stack_index = 0;
	available_column = 323;
	avail_node_index = 324;
}
void sudoku_dlx_heap_solver::print_heap()
{
	std::cout << cur_heap << std::endl;
}
void sudoku_dlx_heap_solver::node_heap_decrease(int node_index)//����һ������������ڵ��е�ɾ������Ϊһ����һ�����ĸ�Ԫ�أ����������У����Ƕ������еĶ��������м���1
{
	int leftmost_node;//��ǰ�ڵ������е�����ڵ������
	leftmost_node = node_index - (node_index % 4);
	cur_heap.update_by_delta(total_nodes[leftmost_node].column, -1);
	leftmost_node++;
	cur_heap.update_by_delta(total_nodes[leftmost_node].column, -1);
	leftmost_node++;
	cur_heap.update_by_delta(total_nodes[leftmost_node].column, -1);
	leftmost_node++;
	cur_heap.update_by_delta(total_nodes[leftmost_node].column, -1);
}
void sudoku_dlx_heap_solver::node_heap_increase(int node_index)//��������ٵ�˳���Ǹպ��෴��
{
	int rightmost_node;//��ǰ�ڵ������е����ҽڵ������
	rightmost_node = node_index - (node_index % 4) + 3;
	cur_heap.update_by_delta(total_nodes[rightmost_node].column, 1);
	rightmost_node--;
	cur_heap.update_by_delta(total_nodes[rightmost_node].column, 1);
	rightmost_node--;
	cur_heap.update_by_delta(total_nodes[rightmost_node].column, 1);
	rightmost_node--;
	cur_heap.update_by_delta(total_nodes[rightmost_node].column, 1);

}

void sudoku_dlx_heap_solver::insert_one_node(std::uint32_t node_idx, std::uint32_t column_index)
{
	auto& cur_node = total_nodes[node_idx];
	cur_node.column = column_index;
	cur_node.down = column_index;
	cur_node.up = total_nodes[column_index].up;
	total_nodes[total_nodes[column_index].up].down = node_idx;
	total_nodes[column_index].up = node_idx;
	column_counter[column_index]++;
}


void sudoku_dlx_heap_solver::insert_row(int current_row_index, int current_column_index, int value)
{
	int current_leftmost = avail_node_index;
	avail_node_index += 4;
	int column_index;
	column_index = current_row_index * 9 + value - 1;
	insert_one_node(current_leftmost, column_index);
	current_leftmost++;
	column_index = 81 + current_column_index * 9 + value - 1;
	insert_one_node(current_leftmost, column_index);
	current_leftmost++;
	column_index = 162 + ((current_row_index / 3) * 3 + current_column_index / 3) * 9 + value - 1;
	insert_one_node(current_leftmost, column_index);

	current_leftmost++;
	column_index = 243 + current_row_index * 9 + current_column_index;
	insert_one_node(current_leftmost, column_index);

}

std::string sudoku_dlx_heap_solver::result_to_line()
{
	int i, j, k, current_index;
	int m, n;
	int output[9][9];
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			output[i][j] = 0;
		}
	}
	for (m = 0; m < stack_index; m++)
	{
		current_index = node_stack[m] - node_stack[m] % 4;
		k = total_nodes[current_index].column % 9;
		i = (total_nodes[current_index].column - total_nodes[current_index].column % 9) / 9;
		current_index++;
		j = (total_nodes[current_index].column - total_nodes[current_index].column % 9 - 81) / 9;
		output[i][j] = k + 1;
	}
	std::string result(81, '0');
	for (m = 0; m < 9; m++)
	{
		for (n = 0; n < 9; n++)
		{
			result[m * 9 + n] = output[m][n] + '0';
		}
	}
	return result;
}

void sudoku_dlx_heap_solver::print_result()
{
	auto result_str = result_to_line();
	printf("***********************\n");
	int m, n;
	for (m = 0; m < 9; m++)
	{
		for (n = 0; n < 9; n++)
		{
			std::cout << result_str[m * 9 + n];
		}
		std::cout << std::endl;
	}
}

void sudoku_dlx_heap_solver::creat_dlx_sudoku()
{
	int i, j, k;
	int row_position[9][9];//�������
	int column_position[9][9];//�������
	int small_position[9][9];//�����ÿһ��С����
	initial();
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			row_position[i][j] = 1;
			column_position[i][j] = 1;
			small_position[i][j] = 1;
		}

	}
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (out[i][j] != 0)
			{
				row_position[i][out[i][j] - 1] = 0;
				column_position[j][out[i][j] - 1] = 0;
				small_position[(i / 3) * 3 + j / 3][out[i][j] - 1] = 0;
			}
		}
	}
	for (i = 0; i < 9; i++)
	{
		for (j = 0; j < 9; j++)
		{
			if (out[i][j] != 0)
			{
				insert_row(i, j, out[i][j]);
			}
			else
			{
				for (k = 0; k < 9; k++)
				{
					if ((row_position[i][k] * column_position[j][k] * small_position[(i / 3) * 3 + j / 3][k]) == 1)
					{
						insert_row(i, j, k + 1);
					}
					else
					{
						//do nothing
					}
				}
			}
		}
	}
	cur_heap.set_array(column_counter);
}

void sudoku_dlx_heap_solver::in_stack(int target_to_stack)
{
	int leftmost = target_to_stack - target_to_stack % 4;
	for (int i = 0; i < 4; i++)//���ڵ�ǰ�е�ÿһ��
	{
		int current_column_traversal = leftmost + i;
		current_column_traversal = total_nodes[current_column_traversal].down;
		//std::cout << "try to remove col" << total_nodes[current_column_traversal].column << std::endl;
		while (current_column_traversal != leftmost + i)//ɾ����ǰ���ཻ����
		{
			if (current_column_traversal != total_nodes[current_column_traversal].column)//������ͷ��
			{
				int temp_node = current_column_traversal - current_column_traversal % 4 - 1;
				for (int j = 0; j < 4; j++)
				{
					temp_node++;
					if (temp_node != current_column_traversal)
					{
						total_nodes[total_nodes[temp_node].down].up = total_nodes[temp_node].up;
						total_nodes[total_nodes[temp_node].up].down = total_nodes[temp_node].down;
					}
				}
				node_heap_decrease(temp_node);
			}
			current_column_traversal = total_nodes[current_column_traversal].down;
		}
	}
	node_heap_decrease(target_to_stack);//���Ե�ǰ�н���ɾ��
	node_stack[stack_index++] = target_to_stack;//Ȼ�������ջ
	available_column -= 4;
	//print_heap();
	//print_result();
}

void sudoku_dlx_heap_solver::out_stack()
{
	int target_to_stack = node_stack[--stack_index];
	int rightmost = target_to_stack - target_to_stack % 4 + 3;
	for (int i = 0; i < 4; i++)//���ڵ�ǰ�е�ÿһ��
	{
		int current_column_traversal = rightmost - i;
		current_column_traversal = total_nodes[current_column_traversal].up;
		while (current_column_traversal != rightmost - i)//ɾ����ǰ���ཻ����
		{
			if (current_column_traversal != total_nodes[current_column_traversal].column)//������ͷ��
			{
				int temp_node = current_column_traversal - current_column_traversal % 4 + 4;
				for (int j = 0; j < 4; j++)
				{
					temp_node--;
					if (temp_node != current_column_traversal)
					{
						total_nodes[total_nodes[temp_node].down].up = temp_node;
						total_nodes[total_nodes[temp_node].up].down = temp_node;
					}
				}
				node_heap_increase(temp_node);
			}
			current_column_traversal = total_nodes[current_column_traversal].up;
		}
	}
	node_heap_increase(target_to_stack);//���Ե�ǰ�н��лظ�
	available_column += 4;
	//print_result();
}

int sudoku_dlx_heap_solver::find_next()
{
	int target_position;
	int temp_node_one;
	if (available_column + 1 == cur_heap.remain_size())
	{
		if (available_column == -1)
		{
			//print_result();
			return 2;
		}
		else
		{
			target_position = cur_heap.top().mutual_idx;
			temp_node_one = total_nodes[target_position].down;
			in_stack(temp_node_one);
			return 1;
		}
	}
	else
	{
		return 0;
	}
}


std::optional<sudoku_configuration> sudoku_dlx_heap_solver::solve_one()
{
	int find_result = 0;
	int temp_node_one;
	sudoku_configuration result;
	while (1)
	{
		find_result = find_next();
		if (!find_result)//����޷���ջ��Ŀǰû���ҵ��⣬���ջ
		{
			temp_node_one = node_stack[stack_index - 1];
			out_stack();
			temp_node_one = total_nodes[temp_node_one].down;
			while ((temp_node_one == total_nodes[temp_node_one].column))//�����ǰԪ���ǵ�ǰ��ͷ�ڵ㣬��ݹ��ջ
			{
				if (stack_index == 0)//���ջ�գ������е������ռ��Ѿ�������ȫ ����
				{
					return {};
				}
				else
				{
					temp_node_one = node_stack[stack_index - 1];
					out_stack();
					temp_node_one = total_nodes[temp_node_one].down;
				}
			}
			in_stack(temp_node_one);//����ѡԪ����ջ
		}
		else
		{
			if (find_result / 2)//����Ѿ��ҵ�������򷵻أ���ʵ�����ǿ��Ը�������߼���Ӧ���ж����������������ȫ����ӡ
			{
				for (std::size_t i = 0; i < 9; i++)
				{
					for (std::size_t j = 0; j < 9; j++)
					{
						result.data[i][j] = out[i][j];
					}
				}
				return result;
			}
		}
	}
}

void sudoku_dlx_heap_solver::set_configure(const sudoku_configuration& configure)
{
	for (std::size_t i = 0; i < 9; i++)
	{
		for (std::size_t j = 0; j < 9; j++)
		{
			out[i][j] = configure.data[i][j];
		}
	}
}
void sudoku_dlx_heap_solver::seek_sudoku()
{
	//print_heap();
	
}