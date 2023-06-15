#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_SCALE_
#define _MATH_BASIC_EIGENLIKE_SCALE_
template<typename T, class V>
class Scale
{
public:
	const T& scalar;
	const V& vector;
    using base = T;

public:
    Scale(const T& scalar, const V& vector);
	size_t size() const;
	base operator [](size_t index) const;
};

template<typename T, class V>
Scale<T, V>::Scale(const T& scalar, const V& vector) : scalar(scalar), vector(vector) {
}

template<typename T, class V>
size_t Scale<T, V>::size() const {
    return vector.size();
}

template<typename T, class V>
Scale<T, V>::base Scale<T, V>::operator [](size_t index) const {
	return scalar * vector[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_SCALE_
