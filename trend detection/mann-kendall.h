

float MannKendall(float * series, size_t size)
{
    int sum = 0;

    for(size_t k = 0; k < size - 1; ++k)
    {
        for(size_t j = 0; j < size; ++j)
        {
            float diff = series[j] - series[k];
            sum += diff == 0 ? 0 : diff > 0 ? 1 : -1;
        }
    }
}