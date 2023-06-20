#include <vector>
#include <type_traits>

namespace math
{
    template<typename T>
    T* allocate(size_t length)
    {
        return new T[length];
        //return mkl_calloc(length, sizeof(T), 64);
    }

    template<typename T>
    void free(T** pointer)
    {
        delete[] *pointer;
        *pointer = nullptr;
    }

	template<typename T>
	concept SCALAR = requires(T scalar)
	{
		std::is_arithmetic<T>::value == true;
	};

	template<typename T>
	concept MATRIX = requires(T matrix, size_t row, size_t col)
	{
		requires VECTOR<T>;
		matrix.row();
		matrix.col();
		matrix(row, col);
	};
    template<typename T, size_t ROW = 0, size_t COL = 0>
    class matrix
    {
    private:
        T* data_;
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
}