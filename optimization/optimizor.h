#include <map>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_OPTIMIZOR_
#define _MATH_OPTIMIZATION_OPTIMIZOR_
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

class Optimizor::Objective
{
public:
	virtual const double* operator() (const double* decisions, double* objectives) = 0;
	virtual ~Objective() {}
};

class Optimizor::Constraint
{
public:
	virtual const double* operator() (const double* decisions, const double* objectives, double* voilations = nullptr) = 0;
	virtual ~Constraint() {}
};

class Optimizor::Result
{
public:
	virtual std::vector<std::vector<double>> decisions() const = 0;
	virtual std::vector<std::vector<double>> objectives() const = 0;
	virtual void Write(const char*) const = 0;
	virtual ~Result() {}
};

class Optimizor::Configuration
{
public:
	using T = std::variant<size_t, double, std::vector<double>>;

private:
	std::map<std::string, T> dictionary_;

public:
	T operator [] (const std::string& name) const {
		auto iter = dictionary_.find(name);
		assert((void("item " + name + " not found!"), iter != dictionary_.end()));
		return iter->second;
	}

	T& operator [] (const std::string& name) {
		return dictionary_[name];
	}

public:
	virtual size_t dimensions() const = 0;
	virtual size_t scales() const = 0;
	virtual size_t constraints() const = 0;

	virtual const double* uppers() const = 0;
	virtual const double* lowers() const = 0;
	virtual const bool* integers() const = 0;
	virtual ~Configuration(){}
public:

	std::unique_ptr<Objective> objective;
	std::unique_ptr<Constraint> constraint;
};
#endif //!_MATH_OPTIMIZATION_OPTIMIZOR_
