#ifndef _SJML_Optimization_Optimizor_
#define _SJML_Optimization_Optimizor_
namespace SJML
{
	namespace Optimization
	{
		namespace Optimizor
		{
			class Objective
			{
			public:
				virtual void objectives(float * decisions,  long decision_size, float * objectives, loong objective_size) = 0;
				virtual ~Objective(){};
			};

			class Constraint
			{
			public:
				virtual float constraints(float * decisions, long decision_size, float * objectives, long objective_size) = 0;
				virtual ~Constraint(){};
			};

			class Optimizor
			{
			protected:
				Objective* objective_function;
				Constraint* constraint_function;

			public:
				Optimizor() : objective_function(nullptr), constraint_function(nullptr){}
				virtual ~Optimizor(){objective_function = nullptr; constraint_function = nullptr;}
				virtual void Optimize(const char * resutls) = 0;
			};
		}
	}
}
#endif // !_SJML_Optimization_Optimizor_