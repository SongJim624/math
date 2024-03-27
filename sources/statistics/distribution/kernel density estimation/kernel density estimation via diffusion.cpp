#include "distribution.h"

constexpr double pi = std::numbers::pi_v<double>;

//this function is different from the build in DCT function
void DiscreteCosineTransform(size_t size, double * data)
{
    double* weights = (double*) MKL_calloc(2 * size, sizeof(double), 64);
    double* transformed = (double*) MKL_calloc(2 * size, sizeof(double), 64);
    double* temporary = (double*) MKL_calloc(2 * size, sizeof(double), 64);

    std::iota(temporary, temporary + size, 0);
    cblas_dscal(size, -0.5 * pi / size, temporary, 1);

    vdCos(size, temporary, weights);
    vdSin(size, temporary, weights + size);
    cblas_dscal(2 * size, 2.0, weights, 1);
    weights[0] = 1.0, weights[size] = 0.0;

    IppStatus status;
    IppsFFTSpec_R_64f * pFFTSpec = nullptr;
    IppHintAlgorithm hint = ippAlgHintNone;

    int order = log2(size), flag = IPP_FFT_NODIV_BY_ANY, sizes[3];
    status = ippsFFTGetSize_R_64f(order, flag, hint, sizes, sizes + 1, sizes + 2);

    Ipp8u* pSpec = (Ipp8u*) mkl_calloc(sizes[0], sizeof(Ipp8u), 64);
    Ipp8u* pSpecBuffer = (Ipp8u*) mkl_calloc(sizes[1], sizeof(Ipp8u), 64);
    Ipp8u* pBuffer = (Ipp8u*) mkl_calloc(sizes[2], sizeof(Ipp8u), 64);

    for (size_t i = 0; i < size / 2; ++i)
    {
        temporary[i] = data[i * 2];
        temporary[size - i - 1] = data[i * 2 + 1];
    }

    status = ippsFFTInit_R_64f(&pFFTSpec, order, flag, hint, pSpec, pSpecBuffer);
    status = ippsFFTFwd_RToPack_64f_I(temporary, pFFTSpec, pBuffer);

    transformed[0] = temporary[0];
    transformed[size / 2] = temporary[size - 1];

    for (size_t i = 1; i != size / 2; ++i)
    {
        transformed[i] = temporary[i * 2 - 1];
        transformed[size - i] = transformed[i];
        transformed[size + i] = temporary[i * 2];
        transformed[2 * size - i] = -temporary[i * 2];
    }

    vdMul(size, weights, transformed, temporary);
    vdMul(size, weights + size, transformed + size, data);
    vdSub(size, temporary, data, data);

    mkl_free(pFFTSpec);
    mkl_free(pSpec);
    mkl_free(pSpecBuffer);
    mkl_free(pBuffer);

    mkl_free(weights);
    mkl_free(transformed);
    mkl_free(temporary);

    pFFTSpec = nullptr;
    pSpec = nullptr;
    pSpecBuffer = nullptr;
    pBuffer = nullptr;

    weights = nullptr;
    transformed = nullptr;
    temporary = nullptr;
}

void Frequency(size_t data, const double * points, int mesh, const double * grids, double * frequency)
{
    size_t count_mesh = 0, count_point = 0;

    while((count_mesh != mesh) && (count_point != data))
    {
        if(*points <= *grids)
        {
            (*frequency) += 1;
            points += 1;
            count_point += 1;
        }
        else
        {
            grids += 1;
            frequency += 1;
            count_mesh += 1;
        }
    }
}

// this implements the function t-zeta*gamma^[l](t)
double fixed_point(double time, size_t size, size_t mesh, const double *identity, const double * square)
{
	size_t exponential = 7;

    auto function = [mesh, identity, square](double time, double exponential)
    {
        double* temporary = (double*)mkl_calloc(mesh - 1, sizeof(double), 64);

        vdLn(mesh - 1, identity, temporary);
        cblas_daxpby(mesh - 1, -time * powf(pi, 2.0f), identity, 1, exponential, temporary, 1);
        vdExp(mesh - 1, temporary, temporary);
        vdMul(mesh - 1, temporary, square, temporary);

        double result = 2.0 * powf(pi, 2.0f * exponential) * std::accumulate(temporary, temporary + mesh - 1, 0.0);
        mkl_free(temporary);
        temporary = nullptr;
        return result;
    };

    double temporary = time, result = function(temporary, exponential);
    for(exponential--; exponential != 1; exponential--)
    {
        double begin = -1.0;
        std::vector<double> array(exponential);
        std::generate(array.begin(), array.end(), [&begin] { return begin += 2.0; });

		double K0 = std::accumulate(array.begin(), array.end(), 1.0, std::multiplies<double>()) / sqrt(2.0 * pi);
        float constant = (1.0 + pow(0.5, exponential + 0.5)) / 3.0;

        temporary = pow(2.0 * constant * K0 / size / result, 2.0 / (3.0 + 2.0 * exponential));
        result = function(temporary, exponential);
    }

	return time - pow(2.0 * size * sqrt(pi) * result, -0.4);
}

//bi-section method to solve the euqtion
bool solve(std::function<double(double)> function, double left, double right, float& result)
{
    constexpr double tolerance = std::numeric_limits<double>::epsilon();
    std::vector<double> results{ function(left), function(right) };

    if (fabs(results[0]) <= tolerance)
    {
        result = left;
        return true;
    }

    if (fabs(results[1]) <= tolerance)
    {
        result = right;
        return true;
    }

	if(results[0] * results[1] > 0)
	{
        return false;
	}

    while (true)
    {
        result = 0.5 * (left + right);
        double value = function(result);

        if(fabs(value) <= tolerance)
        {
            return true;
        }

        std::swap(result, results[0] * value > 0 ? left : right);
        std::swap(value, results[0] * value > 0 ? results[0] : results[1]);
    }
}

//dynamic update the interval of the bi-section
double solve(std::function<double(double)> function, size_t size)
{
    float result = 0, left = 0, right = powf(10.0, -12.0) + 0.00001 * (std::min(std::max(50, size), 1050) - 50);

    while(!solve(function, left, right, result))
    {
        left = right;
		right = std::min(2.0 * right, 0.1);
        assert((void("no root found!\n"), right == 0.1));
    }

    return result;
}

KDE::KDE(size_t size, const double * points, double max, double min) :
    size_(size),
    points_((double *) mkl_calloc(size_ , sizeof(double), 64))
{
    cblas_dcopy(size_, points, 1, points_, 1);

    int mesh = 1 << 14;
    double * grids = (double *) mkl_calloc(mesh, sizeof(double), 64);
    std::iota(grids, grids + mesh, 0);
    cblas_daxpby(mesh, 1, &min, 0, (max - min) / (mesh - 1), grids, 1);

    double * frequency = (double *) mkl_calloc(mesh, sizeof(double), 64);
    Frequency(size, points_, mesh, grids, frequency);
    cblas_dcopy(mesh - 1, frequency + 1, 1, frequency, 1);
    cblas_dscal(mesh, 1.0f / std::accumulate(frequency, frequency + mesh, 0), frequency, 1);
    DiscreteCosineTransform(mesh, frequency);

    double * identity = (double *) mkl_calloc(mesh - 1, sizeof(double), 64);
    std::iota(identity, identity + mesh - 1, 1);
    vdSqr(mesh - 1, identity, identity);

    cblas_dscal(mesh - 1, 0.5, frequency + 1, 1);
    vdSqr(mesh - 1, frequency + 1, frequency + 1);

    auto function = [mesh, size, identity, frequency](double time)
    {
        return fixed_point(time, size, mesh, identity, frequency + 1);
    };

    width_ = (max - min) * sqrt(solve(function, size));

    mkl_free(identity);
    mkl_free(frequency);
    mkl_free(grids);
}

~KDE::KDE()
{
    mkl_free(points_);
    points_ = nullptr;
}
//------------------------------------------------------------------------------
double * KDE::density(int size, const double * points, double * values) const;
{
    double * temporary = (double *) mkl_calloc(size_, sizeof(double), 64);

    for(size_t i = 0; i < size; ++i)
    {
        vdSubI(size_, points_, 1, points + i, 0, temporary, 1);

// calculate the density of standard normal distribution
        vdSqr(size_, temporary, temporary);
        cblas_dscal(size_, -0.5, temporary, 1);
        vdExp(size_, temporary, temporary);
        cblas_dscal(size_, 1 / sqrtf(2 * pi), temporary, 1);

        values[i] = std::accumulate(temporary, temporary + size_, 0) / width_ / size_;
    }

    mkl_free(temporary);
    temporary = nullptr;
    return values;
}

double * KDE::probability(int size, const double * points, double * values) const;
{
    double * temporary = (double *) mkl_calloc(size_, sizeof(double), 64);

    for(size_t i = 0; i < size; ++i)
    {
        vdSubI(size_, points_, 1, points + i, 0, temporary, 1);

// calculate the density of standard normal distribution
        vdSqr(size_, temporary, temporary);
        cblas_dscal(size_, -0.5, temporary, 1);
        vdExp(size_, temporary, temporary);
        cblas_dscal(size_, 1 / sqrtf(2 * pi), temporary, 1);

        values[i] = std::accumulate(temporary, temporary + size_, 0) / widths_.second / size_;
    }

    mkl_free(temporary);
    temporary = nullptr;
    return values;
}