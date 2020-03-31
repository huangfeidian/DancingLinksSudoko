#include <array_heap.h>

void test_case(array_heap& cur_heap, std::uint32_t pos, bool is_inc)
{
    if(is_inc)
    {
        cur_heap.update_by_inc(pos);
    }
    else
    {
        cur_heap.update_by_dec(pos);
    }
    std::cout<<cur_heap<<std::endl;
    
}
int main()
{
    array_heap cur_heap(4);
	test_case(cur_heap, 0, true);
	test_case(cur_heap, 0, true);
	test_case(cur_heap, 0, true);
    test_case(cur_heap, 1, true);
    test_case(cur_heap, 2, true);
    test_case(cur_heap, 3, true);
    test_case(cur_heap, 1, true);
    test_case(cur_heap, 2, true);
    test_case(cur_heap, 1, true);
    test_case(cur_heap, 3, false);
    test_case(cur_heap, 2, false);
    test_case(cur_heap, 1, false);
    test_case(cur_heap, 1, false);
    test_case(cur_heap, 1, false);
    test_case(cur_heap, 2, false);
	test_case(cur_heap, 0, false);
	test_case(cur_heap, 0, false);
	test_case(cur_heap, 0, false);


}