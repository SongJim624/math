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



class Individual_UNSGA
{
public:
	Individual individual;

	long dominated;
	std::list<Individual_UNSGA*> dominate;

	float distance;

	Individual_UNSGA();
	~Individual_UNSGA();
};

struct DominationCMP
{
	bool operator()(Individual_UNSGA* A, Individual_UNSGA* B) const;
};

struct DistanceCMP
{
	bool operator()(Individual_UNSGA* A, Individual_UNSGA* B) const;
};

//Reference Point
struct Point
{
	float* coordinate;
	long rho;

	std::list<Individual_UNSGA*> individuals;

	Point(const std::list<float>& coor);
	~Point();
};

struct PointCMP
{
	bool operator()(Point* A, Point* B) const;
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
	struct Individual
	{
		std::vector<float> decisions;
		std::list<Individual*> dominates;
		size_t dominated;
		float distance;
	};

	struct Point
	{
		std::vector<float> decisions;
		std::list<Individual *> appendeds;
	};

private:


public:
	void Optimize();
};

#endif // !_Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
