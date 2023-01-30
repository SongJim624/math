#ifndef _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
#define _Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_

#include "../genetic-algorithms.h"
#include <time.h>
#include <list>
#include <map>
#include <algorithm>
#include <memory>
#include <mkl.h>

class UNSGA : public Genetic
{
private:
	struct Configuration;
	class Individual;
	class Reference;
	class Reproducor;

private:
	std::unique_ptr<Configuration> configuration_;
	std::list<Individual*> population_;
	std::unique_ptr<Reproducor> reproducor_;

private:
	void Dominate(size_t size, const Individual& lhs, const Individual& rhs);
	std::list<std::list<Individual*>> Sort(std::list<Individual*>& population);
	void Select(Reference& plain, std::list<Individual*>& solution, std::list<Individual*>& population);

public:
	virtual std::vector<std::vector<float>> Optimize(Information * information);

public:
	UNSGA(const Configuration& configuration);
	~UNSGA();
};

class UNSGA::Reference
{
private:
	static Information* information_;
	
private:	
	class Point;
	std::list<Point *> points_;

private:
	void Ideal(const std::list<Individual*>& individuals, float* ideal);
	void Associate(const std::pair<std::map<Individual*, float *>, std::map<Individual*, float*>>& costs);
	std::pair<std::map<Individual*, float*>, std::map<Individual*, float*>> Normalize(const std::list<Individual*> solution, const std::list<Individual*>& critical, float * costs);

public:
	Reference(size_t objective, size_t division);
	~Reference();

public:
	void Niche(size_t needed, std::list<Individual*>& solution, std::list<Individual*>& critical);
};

class UNSGA::Reference::Point
{
private:
	float * location_;
	std::map<float, Individual*> associated_;

public:
	size_t count;
	std::list<Individual*> associated;

public:
	Point(const float * location);
	Point(const std::list<float>& location);
	~Point();

	//perpendicular distance for a point to the reference line
	float distance(const float * point) const;
};

class UNSGA::Individual : public Genetic::Individual
{
public:
	float* decisions();
	float* objectives();

	const float* objectives() const;
	const float* decisions() const;
public:
	size_t dominated;
	std::list<Individual*> dominates;
};

class UNSGA::Reproducor : public Genetic::Reproductor
{
private:
	static VSLStreamStatePtr stream_;

private:
	float cross_, mutation_, threshold_;

private:
	void Cross(const Individual& father, const Individual& mother, Individual& son, Individual& daughter);
	void Mutate(Individual& individual);
	void check(Individual& individual);

public:
	virtual void Reproduce(std::list<Individual*>& solution, std::list<Individual*>& population);

public:
	static void Initialize(VSLStreamStatePtr stream);
	static void Finalize();
};

struct UNSGA::Configuration
{
	size_t maximum;
	size_t division;
};
#endif // !_Mathematical_Tools_Optimization_Genetic_Algorithms_UNSGA_
