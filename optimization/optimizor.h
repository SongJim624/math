#include <map>
#include <string>
#include <variant>
#include <memory>

#ifndef _MATH_OPTIMIZATION_OPTIMIZOR_
#define _MATH_OPTIMIZATION_OPTIMIZOR_
namespace math
{
	class Optimizor
	{
	public:
		class Objective;
		class Configuration;
		class Result;

	public:
		virtual const Result& Optimize(Configuration& configuration) = 0;
		virtual ~Optimizor() {};
	};

	class Optimizor::Objective
	{
	public:
		virtual void operator () (const double* decisions, double* objectives, double* voilations) = 0;
		virtual ~Objective() {};
	};

	class Optimizor::Result
	{
	public:
		virtual void Write(const char* path) const = 0;
		virtual ~Result() {};
	};

	class Optimizor::Configuration
	{
	private:
		std::map<std::string, std::variant<size_t, double, std::vector<double>, std::vector<std::vector<double>>>> dictionary;

	public:
		const auto& operator [] (const std::string& name) const
		{
			return dictionary.find(name)->second;
		}

		auto& operator [] (const std::string& name)
		{
			return dictionary[name];
		}

	public:
		std::unique_ptr<Objective> objective;
	};
}
#endif //!_MATH_OPTIMIZATION_OPTIMIZOR_
