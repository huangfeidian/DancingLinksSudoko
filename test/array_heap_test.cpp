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
    std::size_t len = 10;
    array_heap<std::uint32_t> cur_heap(len);
    cur_heap.reset(0);
    for (int i = 0; i < len; i++)
    {
        cur_heap.update(i, len - i);
    }


}