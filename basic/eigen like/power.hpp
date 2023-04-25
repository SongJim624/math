#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_POWER_
#define _MATH_BASIC_EIGENLIKE_POWER_
template<typename T, class V>
class Power
{
public:
	const T& exponent;
	const V& bases;
    using base = T;

public:
    Power(const T& exponent, const V& base);
	size_t size() const;
	base operator [](size_t index) const;
};

template<typename T, class V>
Power<T, V>::Power(const T& exponent, const V& bases) : exponent(exponent), bases(bases) {
}

template<typename T, class V>
size_t Power<T, V>::size() const {
    return bases.size();
}

template<typename T, class V>
Power<T, V>::base Power<T, V>::operator [](size_t index) const {
	return exponent * bases[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_Power_
