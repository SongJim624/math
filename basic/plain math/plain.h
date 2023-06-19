#include "operators.hpp"
#include "doolitle.hpp"

namespace Plain
{
template<typename T, size_t LEN = 0>
class vector
{
private:
    T* data_;
    size_t length_;

public:
    size_t size() const;
    T operator [] (size_t pos) const;
    T& operator [] (size_t pos);

public:
    T& begin();
    T& end();
    T cbegin() const;
    T cend() const;
};


template<typename T, size_t ROW = 0, size_t COL = 0>
class matrix
{
private:
    T* data_;
    bool dominated_;
    size_t row_, col_;

public:
    size_t row() const;
    size_t col() const;
    size_t size() const;

public:
    T operator() (size_t row, size_t col) const;
    T& operator() (size_t row, size_t col) const;
    T operator [] (size_t pos) const;
    T& operator [] (size_t pos);

public:
//    template< typename T, size_t ROW, size_t COL>
//    matrix slice(std::pair<size_t, size_t>, std::pair<size_t, size_t>)

public:
    //enable if 
    void resize(size_t row, size_t col);

public:
    matrix(size_t row, size_t col);
};



};