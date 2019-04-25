
void Shell(double * &x, const size_t &num)
{
    double temp;
    bool shell = true;
    size_t i;
    if(x[0] > x[1])
    {
        temp = x[0];
        x[0] = x[1];
        x[1] = temp;
    }

    while(shell)
    {
        shell = false;
        
        i = 1;
        for(; i < num - 1; ++i)
        {
            if(x[i] > x[i + 1])
            {
                break;
            }
        }


    }





}