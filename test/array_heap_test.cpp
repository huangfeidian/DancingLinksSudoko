#include <array_heap.h>

void test_case(array_heap<std::uint32_t>& cur_heap, std::uint32_t pos, bool is_inc)
{
    auto pre_value = cur_heap.get(pos);
    if(is_inc)
    {
        cur_heap.update(pos, pre_value + 1);
    }
    else
    {
        cur_heap.update(pos, pre_value - 1);
    }
    std::cout<<cur_heap<<std::endl;
    
}
int main()
{
    array_heap<std::uint32_t> cur_heap(4);
    cur_heap.reset(0);
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