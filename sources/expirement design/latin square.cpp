#include <vector>
#include <list>
#include <algorithm>

template<typename T>
class Square
{
private:
    std::vector<T> data_;

public:
    Square(size_t N) : data_(std::vector<T>(N * N)){}
    T operator () (size_t r, size_t c) const{}
    T& operator() (size_t r, size_t c){}
};

Square<size_t> LatinSquare(size_t number)
{
    Square<size_t> result(number);

    std::list<size_t> numbers;
    for (size_t i = 0; i < number; i++)
    {
        numbers.push_back(i);
    }

    for(size_t i = 0; i < number; ++i)
    {
        auto beginning = std::next(numbers.begin(), i);
        for(size_t j = 0; j < number; ++j)
        {
            result(i, j) = *beginning;
            if(beginning++ == numbers.end())
            {
                beginning = numbers.begin();
            }
        }
    }
    return result;
};