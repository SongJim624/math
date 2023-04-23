#include "../optimizer.h"
#include <functional>

#ifndef _SJML_Optimization_Gentic Algorithms_
#define _SJML_Optimization_Gentic_Algorithms_
namespace Optimization
{
    template<typename T>
    class Genetic : public Optimizer<T>
    {
    protected:
        class Individual;

    public:
        virtual const Optimizer::Result* Optimize() = 0;
        virtual ~Genetic() {};
    };

    template<typename T>
    class Genetic<T>::Individual
    {
    protected:
        static std::pair<size_t, size_t> numbers_;

    public:
        T* decisions, * objectives;

    public:
        //the reason of using the int type but not bool
        //when applying the non dorminant sort, if bool return only, the operator > needed be called again.
        virtual int operator < (const Individual& individual) const = 0;
    };
}
#endif // !_SJML_Optimization_Gentic Algorithms_