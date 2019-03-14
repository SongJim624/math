void Jacobi_Iter(const float** &A, const float* &b, float * &x, const int &num, const float &Tol)
{
//Condition Number :
    float * sum = new float[num];
    float norm = +INFINITY;
    float inter;
    while(norm < Tol)
    {
        for(size_t i = 0; i < num; ++i)
        {
            sum[i] = 0;
            for(size_t j = 0; j < num; ++j)
            {
                sum[i] += a[i][j] * x[j];
            }
        }

        for(size_t i = 0; i < num; ++i)
        {
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