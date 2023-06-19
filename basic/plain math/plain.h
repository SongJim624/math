#include <vector>
#include "operator.hpp"

namespace math
{
    template<typename T, size_t LEN = 0>
    class vector
    {
    private:
        T* data_;
        size_t length_;

    public:
        using base = T;

        size_t size() const
        {
            return length_;
        }

        T operator [] (size_t pos) const
        {
            return data_[pos];
        }

        T& operator [] (size_t pos)
        {
            return data_[pos];
        }

    public:
        //    T& begin();
        //    T& end();
        //    T cbegin() const;
        //    T cend() const;

    public:
        template<EXPRESSION E>
        vector(const E& expression) 
        {
            length_ = expression.size();
            data_ = new T[length_];
            for (size_t i = 0; i < length_; ++i)
            {
                data_[i] = expression[i];
            }
        }

        vector(std::initializer_list<T> list) : data_(nullptr), length_(list.size())
        {
            data_ = new T[length_];
            auto address = data_;

            for (const auto& value : list)
            {
                *address = value;
                address++;
            }
        }

        ~vector()
        {
            delete[] data_;
            data_ = nullptr;
        }

    public:
        template<class EXPRESSION>
        vector& operator = (const EXPRESSION& expression) : vector(expression)
        {
            return *this;
        }
    };

    /*
    template<typename T, size_t ROW = 0, size_t COL = 0>
    class matrix
    {
    private:
        T* data_;
        bool dominated_;
        size_t row_, col_;

    public:
        size_t row() const
        {
            return row_;
        }

        size_t col() const
        {
            return col_;
        }

        size_t size() const;
        {
            return row_ * col_;
        }

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
    //    void resize(size_t row, size_t col);

    public:
        matrix(size_t row, size_t col);
    };
    */
}