#include <any>
#include <map>
#include <string>
#include <vector>
#include <memory>

#ifndef _MATH_OPTIMIZATION_OPTIMIZOR_
#define _MATH_OPTIMIZATION_OPTIMIZOR_

namespace math::Optimization
{
	template<typename T>
	class Objective
	{
	public:
		virtual void evaluate(const T* decisions, T* objectives, T* voilations) = 0;
		virtual ~Objective() = default;
	};


	class Configuration
	{
	public:
		template<typename T>
		auto at(const std::string& name) const -> const T&
		{
			return std::any_cast<T>(configurations_.at(name));
		}
	
		auto operator [] (const std::string& name) -> std::any&
		{
			return configurations_[name];
		}

		template<typename T, typename O, typename ...S>	
		std::unique_ptr<Objective<T>> factory(S&&... args)
		{
			return std::make_unique<O>(std::forward<S&&>(args)...);
		}

	private:
		std::map<std::string, std::any> configurations_;
	};

	template<typename T>
	class Result
	{
	public:
		virtual auto dump(const char* path) const -> int = 0;
		virtual auto candidates()           const -> std::vector<T*> = 0;

		virtual ~Result() = default;
	};

	template<typename T>
	class Optimizor
	{
	public:
		virtual const Result& optimize(const Configuration& configuration) = 0;
	};
}


namespace math
{
	class Optimizor
	{
	public:
		class Objective;
		class Configuration;
		class Result;

	public:
		virtual Result& optimize(Configuration& configuration) = 0;
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
		virtual void write(const char* path, char mode) = 0;
		virtual std::list<std::shared_ptr<const double[]>> results() = 0;

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
