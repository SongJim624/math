#include <cassert>

#ifndef _MATH_BASIC_EIGENLIKE_ADD_
#define _MATH_BASIC_EIGENLIKE_ADD_
template<class L, class R>
class Add
{
public:
	const L& lhs;
	const R& rhs;
    using base = L::base;

public:
    Add(const L& lhs, const L& rhs);
	size_t size() const;
	base operator [](size_t index) const;
};

template<class L, class R>
Add<L, R>::Add(const L& lhs, const L& rhs) : lhs(lhs), rhs(rhs) {
    assert(lhs.size() == rhs.size(), "size not same, cannot add together!");
}

template<class L, class R>
size_t Add<L, R>::size() const {
    return lhs.size();
}

template<class L, class R>
Add<L, R>::base Add<L, R>::operator [](size_t index) const {
	return lhs[index] + rhs[index];
}
#endif //!_MATH_BASIC_EIGENLIKE_ADD_
