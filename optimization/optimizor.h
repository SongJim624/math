#include <map>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_OPTIMIZOR_
#define _MATH_OPTIMIZATION_OPTIMIZOR_
template<typename T, class allocator = std::allocator<T>>
class Optimizor
{
public:
	class Objective;
	class Constraint;
	class Configuration;
	class Result;

public:
	virtual const Result* Optimize(const Configuration *) = 0;
	virtual ~Optimizor(){};
};

template<typename T, class allocator>
class Optimizor<T, allocator>::Objective
{
public:
	virtual void operator() (const T* decisions, T* objectives) = 0;
	virtual ~Objective() {}
};

template<typename T, class allocator>
class Optimizor<T, allocator>::Constraint
{
public:
	virtual void operator() (const T* decisions, const T*& objectives, T* voilations) = 0;
	virtual ~Constraint() {}
};

template<typename T, class allocator>
class Optimizor<T, allocator>::Result
{
public:
	virtual std::vector<std::vector<T, allocator>> decisions() const = 0;
	virtual std::vector<std::vector<T, allocator>> objectives() const = 0;
	virtual void Write(const char*) const = 0;
	virtual ~Result() {}
};

template<typename T, class allocator>
class Optimizor<T, allocator>::Configuration
{
private:
	std::map<std::string, std::variant<size_t, T, std::vector<T, allocator>>> dictionary_;

public:
	auto operator [] (const std::string& name) const
	{
		auto iter = dictionary_.find(name);
		assert((void("item " + name + " not found!"), iter != dictionary_.end()));
		return iter->second;
	}

	auto& operator [] (const std::string& name)
	{
		return dictionary_[name];
	}

public:
	virtual size_t dimensions() const = 0;
	virtual size_t scales() const = 0;
	virtual size_t constraints() const = 0;

	virtual std::vector<T, allocator> uppers() const = 0;
	virtual std::vector<T, allocator> lowers() const = 0;
	virtual std::vector<T, allocator> integers() const = 0;
	virtual ~Configuration(){}
public:

	std::unique_ptr<Objective> objective;
	std::unique_ptr<Constraint> constraint;
};
#endif //!_MATH_OPTIMIZATION_OPTIMIZOR_
