#include "Copula.h"
Copula::Copula(std::vector<std::string>& names, std::vector<float>& data, std::vector<bool>& integer)
{
    long size = data.size() / names.size();

    std::vector<float> datac = data;
    std::vector<float> X(size);

//Construct the marginal distributions
    for(long i = 0; i < names.size(); ++i)
    {
        cblas_copy(size, &datac[0 + i], names.size(), &X[0], 1);
        marginals[names[i]] = integer[i] ? new Empirical(X) : new KDE(X);
    }

//Calibrate the parameters for the copula function

}

void Copula::density(float * X, float * Y, long N)
{
// Xc is a copy for the input variables    
    std::vector<float> Xc(marginals.size() * N);
    memcpy(&Xc[0], X, Xc.size() * sizeof(float));

// temporary vector to store the X for the each marginal
    std::vector<float> x(N);

//calculate the marginal density, and store the results in the Xc
    for(auto it = marginals.begin(); it != marginals.end(); ++it)
    {
        long location = std::distance(it, marginals.begin());
        cblas_copy(N, &Xc[location], marginals.size(), &x[0], 1);

        it->second->density(&x[0], Y, N);
        cblas_copy(N, Y, 1, &Xc[location], marginals.size());
    }

//calculate results of copula function
    Gaussian(&Xc[0], Y, N);
    return;
}