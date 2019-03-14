void Gauss_Iter(float** &A, float* &b, float * &x, const int &num, const float &Tol)
{
//The conditional number is computed

//
    float sum = 0;
    float norm = +INFINITY;
    float inter;
    while(norm < Tol)
    {
        for(size_t i = 0; i < num; ++i)
        {
            sum = 0;
            for(size_t j = 0; j < num; ++j)
            {
                sum += a[i][j] * x[j];
            }

            sum -= a[i][i] * x[i];

            inter = (b[i] - sum) / a[i][i];

            if(absf(inter - x[i]) < norm)
            {
                norm = inter - x[i];
            }

            x[i] = inter;
        }
    }
}

