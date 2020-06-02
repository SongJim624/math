/*
The index of row and column begins with 0
The structure of the matrix is :
    b, c, 0, 0
    a, b, c, 0
    0, a, b, c
    0, 0, a, b
*/
template<typename T>
class Triangle
{
private:
    std::vector<T> A, B, C;

public:
    Triangle(const std::vector<T>& a, const std::vector<T>& b, const std::vector<T>& c)
        : A(a), B(b), C(c) {}

    T& operator ()(const size_t& r, const size_t& c)
    {
        if (r == c){return B[r];}
        if (r == c + 1) { return A[r - 1]; }
        if (r == c - 1) { return C[r]; }
    }

    size_t size() { return B.size(); }
};

template<typename T>
std::vector<T> Chasing(Triangle<T>& A, const std::vector<T>& b)
{
    size_t num = b.size();
    std::vector<T> u(num);
    std::vector<T> l(num);
    std::vector<T> y(num);
    std::vector<T> x(num);

    u[0] = A(0, 0);

    for (size_t i = 1; i < num; ++i)
    {
        l[i] = A(i, i - 1) / u[i - 1];
        u[i] = A(i, i) - A(i - 1, i) * l[i];
    }

    y[0] = b[0];
    for (size_t i = 1; i < num; ++i)
    {
        y[i] = b[i] - l[i] * y[i - 1];
    }

    x[num - 1] = y[num - 1] / u[num - 1];

    for (size_t i = num - 2; i != 0; --i)
    {
        x[i] = (y[i] - A(i, i + 1) * x[i + 1]) / u[i];
    }

    x[0] = (y[0] - A(0, 1) * x[1]) / u[0];
    return x;
}