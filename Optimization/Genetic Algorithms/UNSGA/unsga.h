#ifndef _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
#define _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_

#include "../Genetic Algorithms.h"
#include "Genetic Operation.h"
#include <list>
#include <vector>
#include <map>
#include <time.h>
#include <iostream>
#include <fstream>
#include <string>
#include <memory>
#include <mkl.h>

struct Information
{
	size_t decisions, objectives;
	std::function<void(size_t, const float *, size_t, float *)> objective;
	std::function<float(const float *)> constraints;
};

class UNSGA : public Genetic_Algorithm
{
private:
	const long generation;
	long total_size;

//Reference Plane
	std::list<Point*> Zr;
//Population
	std::vector<Individual_UNSGA*> individuals;

	double mu, mum;

	std::list<Individual_UNSGA*> solution, extend;

//Matrix used when getting the hyperplane
	float** A;
	float* ideal, * interception;

private:
	void Write(const char* results_path);

private:
	std::list<std::list<float>> Reference(const long& M, const long& N);
	void Normalize(std::list<Individual_UNSGA*>& selected);

private:
	void Ideal_Point(std::list<Individual_UNSGA*>& top);
	void Interception(std::list<Individual_UNSGA*>& bottom);
	void Associate(std::list<Individual_UNSGA*>& selected);
	void Niche(std::list<Individual_UNSGA*>& critical);
private:
//Non-dominate Sort
	std::list<std::list<Individual_UNSGA*>> Sort();

//Select from the critical layer
	void Select(std::list<Individual_UNSGA*>& critical);

//The whole select operation
	void Select();
	void Genetic();
public:
	UNSGA(Function* function, Constraints* constraints, 
		const long& generation, const long decision_size, const long& objective_size, const long& nDiv, 
		float* upper, float* lower, bool* integer);
	~UNSGA();

	virtual void Optimize(const char* results_path = nullptr);
};

class UNSGA
{
private:
	class Individual;
	class Reference;

	std::unique_ptr<Reference> references_;

	std::list<std::unque_ptr<Individual>> population_;

private:
	void Select(std::list<Individual*> population);
	void Gentic(std::list<Individual*> population);

public:
	virtual void Optimize();

public:
	UNSGA(size_t maximum, size_t division);
	~UNSGA();
};

class UNSGA::Reference
{
private:
	class Point;
	std::list<Point *> points_;

public:
	Reference(size_t objective, size_t division);
	void Associate(std::list<Individual*>& solution, std::list<Individual*>& critical);
};

class UNSGA::Reference::Point
{
private:
	static size;

private:
	size_t count_;
	float * location_;
	std::list<Individual*> associated_;

public:
	Point(const float * location);
	~Point();

	size_t count() const;
	//perpendicular distance for a point to the reference line
	float distance(const float * point);

	void AddReference();
	void Attach(Individual *);
	Individual* Detach();
};

class UNSGA::Individual
{
private:
	size_t decisions_, objectives_;
	static std::function<void(size_t, const float *, size_t, float)> function_;
	static float * upper_, * lower_;
	static bool * integer_;

private:
	float * decision_, * objective_;

public:
	void Update(const float * decision);
	const float * objective() const;

public:
	static Initialize(size_t, size_t);
	static Finalize();
};
#endif // !_Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
