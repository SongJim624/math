#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_DIVIDE_
#define _MATH_BASIC_EIGENLIKE_DIVIDE_
template<class L, class R>
class Divide
{
public:
	const L& lhs;
	const R& rhs;
    using base = L::base;

public:
    Divide(const L& lhs, const R& rhs);
	size_t size() const;
	base operator [](size_t index) const;
};

template<class L, class R>
Divide<L, R>::Divide(const L& lhs, const R& rhs) : lhs(lhs), rhs(rhs) {
	assert((void("size not same, cannot add together!"), lhs.size() == rhs.size()));
}

template<class L, class R>
size_t Divide<L, R>::size() const {
    return lhs.size();
}

template<class L, class R>
Divide<L, R>::base Divide<L, R>::operator [](size_t index) const {
	return lhs[index] / rhs[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_DIVIDE_
