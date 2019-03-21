void Chasing(float * &a, float * &b, float * &c, float * &f, float * &x, const int &num)
{
    c[0] = c[0] / b[0];
    
    for(size_t i = 1; i < num - 1; ++i)
    {
        c[i] = c[i] / (b[i] - a[i] * c[i - 1]);
    }

    f[0] = f[0] / b[0];
    for(size_t i = 1; i < num; ++i)
    {
        f[i] = (f[i] - a[i] * f[i - 1]) / (b[i] * a[i] * c[i - 1]);
    }

    x[num - 1] = y[num - 1];

    while(i < num)
    {
        x[i] = y[i] - beta[i] * x[i + 1];
        ++i;
    }
}












