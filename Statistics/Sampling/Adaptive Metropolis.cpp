#include "Sampling.h"

template<typename T>
bool DR(T& X, const T& variance)
{

}

template<typename T>
T Diagnose(const std::vector<std::vector<T>>& chains)
{
    size_t m = chains.size();
    size_t n = chains[0].size();

//B / n
    std::vector<T> theta(m, 0);

    for (size_t i = 0; i < m; ++i)
    {
        for (size_t j = 0; j < n; ++j)
        {
            theta[i] += chains[i][j];
        }

        theta[i] /= n;
    }

    T mean = Mean(theta);

    T B = 0;
    for (size_t i = 0; i < m; ++i)
    {
        B += (theta[i] - mean) * (theta[i] - mean);
    }

    B /= (m - 1);

    T W = 0;

    for (size_t i = 0; i < m; ++i)
    {
        T s = 0;

        for (size_t j = 0; j < n; ++j)
        {
            s += (chains[i][j] - theta[i]) * (chains[i][j] - theta[i]);
        }

        W += s / (n - 1);
    }

    W /= m;

    T sigma = (n - 1) * W / n + B;
    T v = 2 * pow(sigma + B / m, 2) / W;

    
    return ((n - 1) / (T)n + (m + 1) / (T)m * B / W) * (v + 3) / (v + 1);
}


template<typename T>
std::vector<T> AM(Distribution<T>& dist, const T& X0, const size_t& N)
{
    size_t m = 50;
    std::vector<std::vector<T>> chains(m, std::vector<T>(2 * N, 0));
    T eps = std::numeric_limits<T>::epsilon();
    T C0 = 1;
    
    size_t n0 = 100;
    T sd = 5.76; //2.4 * 2.4
//begin with standard normal sidtribution    

    size_t i = 1;
    T mean = X0; 
    T variance = 0;

    T X = X0;

    while(i != n0 + 1)
    {
        T X_next = Box_Muller(X, C0);

//        if (rand() / (T)RAND_MAX < min(dist.pdf(X_next) / dist.pdf(X), 1)) 
        if (rand() / (T)RAND_MAX < min(exp(-0.5 * X_next * X_next) / exp(-0.5 * X * X), 1)) 
        { 
            mean += X_next;
            variance += X_next * X_next;
            X = X_next;
            ++i; 
        }
    }

    for (size_t i = 0; i < m; ++i)
    {
        T mean_i = mean;
        T variance_i = variance;

        chains[i][0] = mean;

        size_t j = 1;
        while (j != 2 * N)
        {
            T Cn = sd * (eps + 
                sqrt((variance_i - mean_i * mean_i / (n0 + j - 1)) / (n0 + j - 2)));
            
            T X_next = Box_Muller(chains[i][j - 1], Cn);

            /*
            T X_next = chain[i][j - 1];
            if(DR(X_next, variance_i))
            */
//            if (rand() / (T)RAND_MAX < min(dist.pdf(X_next) / dist.pdf(chains[i][j - 1]), 1))
            if (rand() / (T)RAND_MAX < min(exp(-0.5 * X_next * X_next) / exp(-0.5 * X * X), 1))
            {
                mean_i += X_next;
                variance_i += X_next * X_next;
                chains[i][j] = X_next;

                ++j;
            }
        }
    }

    T R = Diagnose(chains);

    if (R > 1.2)
    {
        MessageBox("Markov Chain does NOT Converge!")
        return std::vector<T>();
    }
    else
    {
        std::vector<T> chain;
        
        for (size_t i = 0; i < m; ++i)
        {
            chain.insert(chain.end(), chains[i].begin(), chains[i].end());
        }

        return chain;
    }
}