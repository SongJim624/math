#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_SUBSTRACT_
#define _MATH_BASIC_EIGENLIKE_SUBSTRACT_
template<class L, class R>
class Substract
{
public:
	const L& lhs;
	const R& rhs;
    using base = L::base;

public:
    Substract(const L& lhs, const L& rhs);
	size_t size() const;
	base operator [](size_t index) const;
};

template<class L, class R>
Substract<L, R>::Substract(const L& lhs, const L& rhs) : lhs(lhs), rhs(rhs) {
    assert(lhs.size() == rhs.size(), "size not same, cannot Substract together!");
}

template<class L, class R>
size_t Substract<L, R>::size() const {
    return lhs.size();
}

template<class L, class R>
Substract<L, R>::base Substract<L, R>::operator [](size_t index) const {
	return lhs[index] - rhs[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_SUBSTRACT_
