
void Bubble(double * &a, size_t &num)
{
    bool swap = true;
    double inter;
    while(swap)
    {
        swap = false;
        for(size_t i = 0; i < num - 1; ++i)
        {
            if(a[i] > a[i + 1])
            {
                inter = a[i + 1];
                a[i + 1] = a[i];
                a[i] = inter;
                swap = true;
            }
        }
        --num;
    }
}