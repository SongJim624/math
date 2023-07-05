#include "../math.h"
using namespace math;

void operation(size_t length, std::array<const double*, 2> ptr, double * res, std::array<size_t, 3> inc, std::function<double(double, double)> opr)
{
    for (size_t i = 0; i < length; ++i)
    {
        *res = opr(*ptr[0], ptr[1] == nullptr ? 0 : *ptr[1]);

        ptr[0] += inc[0];
        ptr[1] += inc[1];
        res += inc[2];
    }
}

void math::add(size_t length, const double * left, const double * right, double * results)
{
    operation(length, { left, right }, results, { 1, 1, 1 }, [](double l, double r) {return l + r; });
}

void math::sub(size_t length, const double* left, const double* right, double* results)
{
    operation(length, { left, right }, results, { 1, 1, 1 }, [](double l, double r) {return l - r; });
}

void math::mul(size_t length, const double* left, const double* right, double* results)
{
    operation(length, { left, right }, results, { 1, 1, 1 }, [](double l, double r) {return l * r; });
}

void math::div(size_t length, const double* left, const double* right, double* results)
{
    operation(length, { left, right }, results, { 1, 1, 1 }, [](double l, double r) {return l / r; });
}

void math::pow(size_t length, const double* left, const double* right, double* results)
{
    operation(length, { left, right }, results, { 1, 1, 1 }, [](double l, double r) {return std::pow(l, r); });
}

void math::exp(size_t length, const double* expontential, double* results)
{
    operation(length, { expontential, nullptr }, results, { 1, 0, 1 }, [](double l, double r) {return std::exp(l); });
}

void math::addI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc)
{
    operation(length, { left, right }, results, { linc, rinc, inc }, [](double l, double r) {return l + r; });
}

void math::subI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc)
{
    operation(length, { left, right }, results, { linc, rinc, inc }, [](double l, double r) {return l - r; });
}

void math::mulI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc)
{
    operation(length, { left, right }, results, { linc, rinc, inc }, [](double l, double r) {return l * r; });
}

void math::divI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc)
{
    operation(length, { left, right }, results, { linc, rinc, inc }, [](double l, double r) {return l / r; });
}

void math::powI(size_t length, const double* left, size_t linc, const double* right, size_t rinc, double* results, size_t inc)
{
    operation(length, { left, right }, results, { linc, rinc, inc }, [](double l, double r) {return std::pow(l, r); });
}

void math::expI(size_t length, const double* exponential, size_t linc, double* results, size_t inc)
{
    operation(length, {exponential, nullptr }, results, { linc, 0, inc }, [](double l, double r) {return std::exp(l); });
}

void math::scal(size_t length, double factor, double* vector, size_t inc)
{
    operation(length, { vector, nullptr }, vector, { inc, 0, inc }, [factor](double l, double r) { return factor * l; });
}

double math::dot(size_t length, const double* left, size_t linc, const double* right, size_t rinc)
{
    auto temporary = std::unique_ptr<double[], decltype(&math::free)>{ math::allocate(length), math::free };
    mulI(length, left, linc, right, rinc, temporary.get(), 1);
    return std::accumulate(temporary.get(), temporary.get() + length, 0.0);
}

void math::xpby(size_t length, const double* left, size_t linc, double factor, double* right, size_t rinc)
{
    operation(length, { left, right }, right, { linc, rinc, rinc }, [factor](double l, double r) { return l + factor * r; });
}