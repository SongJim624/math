#include <math.h>

class PDF
{
private:
    const float pi = 3.1415925;
protected:

public:
    void Beta(const float &alpha, const float &beta, const float &x, float pdf);
    void Gamma(const float &alpha, const float &beta, const float &x, float &pdf);
    void Exponential(const float &lamada, const float &x, float &pdf);
    void Uniform(const float& a, const float &b, const float &x, float &pdf);
    void Normal(const float & mu,
        const float & sigma, const float &x, float &pdf);
    void KDE(const float *);

    void Fit();



    void pdf();
}