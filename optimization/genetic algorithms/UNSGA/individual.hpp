#include "unsga.hpp"

template<typename T, class allocator>
UNSGA<T, allocator>::Population::Individual::Individual(const std::vector<T, allocator>& decision, size_t dimension, size_t constraint) :
    decisions(decision),
    objectives(std::vector<T, allocator>(scale)),
    voilations(std::vector<T, allocator>(constriant))
{
}