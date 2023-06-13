//KDE

#include <algorithm>
#include <numeric>
#include <numbers>
#include <functional>
#include <mkl.h>
#include <ipps.h>
#include <math.h>
#include <vector>
#include <map>
/*
class Distribution
{
private:
    size_t size_;
    double min_, scale_, * weights_, * means_, * variations_;

private:
    double * kernel(size_t size, const double * points, double * values);
    double regEM(double& delta, size_t size, const double * points);

public:
    double * density(size_t size, const double * points, double * value) const;
    void sample(size_t size, double * samples) const;

public:
    Distribution(size_t size, const double * pivots, double tolerance = 1e-6f, size_t iteration = 1500);
    ~Distribution();
};
*/
//inplementation according to Zdravko Botev version 1.5.0
class KDE
{
private:
    size_t size_;
    //bandwidth for density function and possibility function
    std::pair<double, double> widths_;
    double density_, possibility_;
    double width_, *points_;

public:
    virtual double * density(int size, const double * points, double * values) const;
    virtual double * probability(size_t size, const double * points, double * values) const;

public:
    KDE(int size, const double* points, double max, double min);
};