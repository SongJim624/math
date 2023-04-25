#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_MULTIPLY_
#define _MATH_BASIC_EIGENLIKE_MULTIPLY_
template<class L, class R>
class Multiply
{
public:
	const L& lhs;
	const R& rhs;
    using base = L::base;

public:
    Multiply(const L& lhs, const L& rhs);
	size_t size() const;
	base operator [](size_t index) const;
};

template<class L, class R>
Multiply<L, R>::Multiply(const L& lhs, const L& rhs) : lhs(lhs), rhs(rhs) {
    assert(lhs.size() == rhs.size(), "size not same, cannot multiply together!");
}

template<class L, class R>
size_t Multiply<L, R>::size() const {
    return lhs.size();
}

template<class L, class R>
Multiply<L, R>::base Multiply<L, R>::operator [](size_t index) const {
	return lhs[index] * rhs[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_MULTIPLY_
