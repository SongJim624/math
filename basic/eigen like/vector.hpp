#include <vector>
#include <cassert>
#include <concepts>
#include <type_traits>

#ifndef _MATH_BASIC_EIGENLIKE_VECTOR_
#define _MATH_BASIC_EIGENLIKE_VECTOR_
template<typename T>
concept Expression requires (T x) {
	x::base;
	x[];
};

template<typename T>
class Vector
{
public:
	using base = typename T;

private:
	std::vector<T> data_;

public:
	size_t size() const { return data_.size(); };
	T operator[] (size_t index) const;
	T& operator[] (size_t index);

public:
	auto begin() { return data_.begin(); }
	auto end() { return data_.end(); }

	auto insert(auto pos, auto begin, auto end) { return data_.insert(pos, begin, end); }
	auto insert(auto pos, const Vector<T>& vector) { return data_.insert(pos, vector.begin(), vector.end()); }

public:
	Vector() {};
	Vector(size_t length, T value = 0) { data_.resize(length, value); };
	Vector(const Vector<T>&) = default;
	template<Expression E> Vector(const E& expression);

public:
	void resize(auto length, auto value = 0) { data_.resize(length, value);};

public:
	template<Expression E> Vector& operator = (const E& expression);
};

template<typename T>
template<Expression E>
Vector<T>::Vector(const E& expression) {
	data_.resize(expression.size());

	for (size_t i = 0; i < data_.size(); ++i) {
		data_[i] = expression[i];
	}
}

template<typename T>
T Vector<T>::operator [] (size_t index) const {
	return data_[index];
}

template<typename T>
T& Vector<T>::operator [] (size_t index) {
	return data_[index];
}

template<typename T>
template<typename Expression>
Vector<T>& Vector<T>::operator = (const Expression& expression) {
	data_.resize(expression.size());

	for (size_t i = 0; i < data_.size(); ++i) {
		data_[i] = expression[i];
	}

	return* this;
}

template<typename T, class V>
class Power
{
public:
	const T& exponent;
	const V& base;
	size_t size() const { return base.size(); };

public:
	T operator [](size_t index) const {
		return  pow(base[index], exponent);
	}
};

template<class L, class R>
auto operator + (const L& lhs, const R& rhs) {
	return Add<L, R>{lhs, rhs};
}

template<class L, class R>
auto operator - (const L& lhs, const R& rhs) {
	return Substract<L, R>{lhs, rhs};
}

template<class L, class R>
auto operator * (const L& lhs, const R& rhs) {
	return Multiply<L, R>{lhs, rhs};
}

/*
template<typename T, class V, std::enable_if<std::is_floating_point<T>::value>>
auto operator * (const T& lhs, const V& rhs) {
	return Scale<T, V>{lhs, rhs};
}
*/

template<class L, class R>
auto operator / (const L& lhs, const R& rhs) {
	return Divide<L, R>{lhs, rhs};
}

template<class V, typename T>
auto operator ^ (const V& lhs, const T& rhs) {
	return Power<T, V>{rhs, lhs};
}


#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_

