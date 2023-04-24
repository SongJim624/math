#include <map>
#include <string>
#include <vector>
#include <variant>
#include <memory>
#include <cassert>

#ifndef _SJML_Optimization_Optimizor_
#define _SJML_Optimization_Optimizor_
namespace Optimization
{
	template<typename T>
	class Objective
	{
	public:
		virtual void operator() (const T* decisions, T* objectives) = 0;
		virtual ~Objective() {}
	};

	template<typename T>
	class Constraint
	{
	public:
		virtual void operator() (const T* decisions, const T* objectives, T* voilations = nullptr) = 0;
		virtual ~Constraint() {}
	};

	template<typename T>
	class Result
	{
	public:
		virtual std::vector<std::vector<T>> decisions() const = 0;
		virtual std::vector<std::vector<T>> objectives() const = 0;
		virtual void Write(const char*) const = 0;
		virtual ~Result() {}
	};

	template<typename T>
	class Configuration
	{
	private:
		std::map<std::string, std::variant<size_t, T, std::vector<T>>> dictionary_;

	public:
		std::variant<size_t, T, std::vector<T>> operator [] (const std::string& name) const {
			auto iter = dictionary_.find(name);
			assert(iter != dictionary_.end(), "item " + name + " not found!");
			return iter->second;
		}

		std::variant<size_t, T, std::vector<T>>& operator [] (const std::string& name) {
			return dictionary_[name];
		}

	public:
		virtual size_t dimensions() const = 0;
		virtual size_t scales() const = 0;
		virtual size_t constraints() const = 0;

		virtual const T* uppers() const = 0;
		virtual const T* lowers() const = 0;
		virtual const bool* integers() const = 0;
		virtual ~Configuration(){}
	public:

		std::unique_ptr<Objective<T>> objective;
		std::unique_ptr<Constraint<T>> constraint;
	};

	template<typename T>
	class Optimizer
	{
	public:
		virtual const Result<T>* Optimize(Configuration<T>*) = 0;
		virtual ~Optimizer() {};
	};
}
#endif // !_SJML_Optimization_Optimizor_