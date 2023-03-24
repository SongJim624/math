#include <mkl.h>
#include <vector>

typedef vector std::vector<float>;

//this is the max form
//the length of the coeffficients is the decisions plus the slacks
//so does the matrix
vector Simplex(const vector& coefficients, const vector& matrix, const vector& value)
{
    size_t column = coefficients.size() + value.size() + 2;
    vector table(column * value.size(), 0);

    for(size_t c = 0; c < coefficients.size(); ++c)
    {
        cblas_scopy(value.size(), &matrix[c], coefficients.size(), &table[c], column);
    }

    for(size_t r = 0; r < value.size(); ++r)
    {
        table[r + coefficients.size() + r * column] = 1;
    }

    cblas_scopy(value.size(), &value[0], 1, &table[column - 2], column);
    cblas_sscal(value.size(), 0, &table[column - 1], column);

    std::vector<size_t> indicies(column - 2);
    std::iota(indicies.begin(), indicies.end(), 0);

    while(true)
    {
        std::pair<float, size_t> in {-INFINITY, 0};

        for(size_t i = 0; i < coefficients.size(); ++i)
        {
            size_t index = indicies[i];

            float check = (index < coefficeints.size() ? coefficients[index] : 0) - cblas_sdot(value.size(), &table[column - 1], column, &table[index], column);

            if(check > in.first)
            {
                in.first = check;
                in.second = index;
            }
        }

        if(!(in.first > 0))
        {
            break;
        }

        std::pair<float, size_t> out{+INFINITY, 0};

        for(size_t r = 0; r < value.size(); ++r)
        {
            float check = table[(r + 1) *  column - 2] / table[in.second + r * column];

            if((check > 0) && (check < out.first))
            {
                out.first = check;
                out.second = r;
            }
        }

        size_t index = indicies[out.second + coefficients.size()];
        table[out.second * column - 1] =  index > coefficients.size() ? 0 : coefficients[index];

        std::swap(indices[in.second], indices[out.second]);

        cblas_sscal(column - 1, 1 / table[in.second + out.second * column], &table[out.second * column], 1);
        for(size_t r = 0; r < value.size(); ++r)
        {
            if(r == out.second)
            {
                continue;
            }

            cblas_saxpy(column - 1, -table[r * column + in.second], &table[out.second * column], 1, table[r * column], 1)
        }
    }

    vector solution(column - 2);

    for(size_t r = 0; r < value.size(); ++r)
    {
        solution[indicies[r + coefficients.size()]] = table[(r + 1)* column - 1];
    }

    return solution;
}