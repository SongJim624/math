#include <vector>
#include <cassert>
#include <concepts>
#include <type_traits>

#ifndef _MATH_BASIC_EIGENLIKE_VECTOR_
#define _MATH_BASIC_EIGENLIKE_VECTOR_
template<typename T>
concept Expression = requires (T x) {
	//T::base;
	x.size();
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

	auto append(const Vector<T>& vector) { return data_.insert(data_.end(), vector.data_.begin(), vector.data_.end()); }
public:
	Vector() {};
	Vector(size_t length, T value = 0) { data_.resize(length, value); };
	Vector(const Vector<T>&) = default;
	Vector(const std::vector<T>&);
	Vector(size_t length, const T* begin);
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
Vector<T>::Vector(const std::vector<T>& vector) :
	data_(vector)
{
}

template<typename T>
Vector<T>::Vector(size_t length, const T* begin) 
{
	data_ = std::vector<T>(begin, begin + length);
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
template<Expression E>
Vector<T>& Vector<T>::operator = (const E& expression) {
	data_.resize(expression.size());

	for (size_t i = 0; i < data_.size(); ++i) {
		data_[i] = expression[i];
	}

	return* this;
}
#endif //!_MATH_OPTIMIZATION_UNSGA_MATRIX_

