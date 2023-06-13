#include <map>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <cassert>

#ifndef _MATH_OPTIMIZATION_OPTIMIZOR_
#define _MATH_OPTIMIZATION_OPTIMIZOR_
class Objective
{
public:
	virtual void operator() (const double* decisions, double* objectives) = 0;
	virtual ~Objective() {}
};

class Constraint
{
public:
	virtual void operator() (const double* decisions, const double* objectives, double* voilations = nullptr) = 0;
	virtual ~Constraint() {}
};

class Result
{
public:
	virtual std::vector<std::vector<double>> decisions() const = 0;
	virtual std::vector<std::vector<double>> objectives() const = 0;
	virtual void Write(const char*) const = 0;
	virtual ~Result() {}
};

class Configuration
{
private:
	std::map<std::string, std::variant<size_t, double, std::vector<double>>> dictionary_;

public:
	std::variant<size_t, double, std::vector<double>> operator [] (const std::string& name) const {
		auto iter = dictionary_.find(name);
		assert((void("item " + name + " not found!"), iter != dictionary_.end()));
		return iter->second;
	}

	std::variant<size_t, double, std::vector<double>>& operator [] (const std::string& name) {
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

class Optimizor
{
public:
	class Configuration;
	class Result;

public:
	virtual const Result* Optimize(Configuration*) = 0;
	virtual ~Optimizor() {};
};

class Optimizor::Configuration
{

};

class Optimizor::Result
{

};
#endif //!_MATH_OPTIMIZATION_OPTIMIZOR_
