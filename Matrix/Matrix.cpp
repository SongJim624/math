# include "Matrix.h"

Matrix::Matrix() : 
    IMatrix(0, 0), data(nullptr)
{}

Matrix::Matrix(const Matrix& matrix) :
    IMatrix(matrix.row, matrix.column), data(new float[row * column])
{
    memcpy(data, matrix.data, row * column * sizeof(float));
}

Matrix::Matrix(long r, long c) 
    : IMatrix(r, c), data(new float[row * column]{0}) 
{}

Matrix::Matrix(long row, long column, float* data) : 
    IMatrix(row, column), data(new float [row * column])
{
    memcpy(this->data, data, row * column * sizeof(float));
}

Matrix::Matrix(long row, long column, float** data) 
    : IMatrix(row, column), data(new float[row * column])
{
    for (long i = 0; i < row; ++i)
    {
        memcpy(this->data + i * column, data[i], column * sizeof(float));
    }
}

Matrix::Matrix(const std::vector<std::vector<float>>& matrix) :
    IMatrix(matrix.size(), matrix[0].size()), data(new float[row * column])
{
    for (long i = 0; i < row; ++i)
    {
        memcpy(data + i * column, &matrix[i][0], column* sizeof(float));
    }
}

Matrix::~Matrix()
{
    if(data == nullptr)
    {
        return;
    }

    delete[] data;
    data = nullptr;
}

float& Matrix::operator()(long r, long c)
{
    return *(data + (r * column + c));
}

float Matrix::operator ()(long r, long  c) const
{
    return *(data + (r * column + c));
}

Matrix& Matrix::operator = (const Matrix& A)
{
    row = A.row;
    column = A.column;

    delete[] data;
    data = new float[row * column];
    memcpy(data, A.data, row * column * sizeof(float));

    return *this;
}

Matrix& Transposite(Matrix& A)
{
    for(long i = 0;  i< A.row; ++i)
    {
        for(long j = 0; j < i + 1; ++j)
        {
             std::swap(*(A.data + i * A.column + j), 
                 *(A.data + j * A.column + i));
        }
    }
    
    std::swap(A.row, A.column);
    
    return A;
}