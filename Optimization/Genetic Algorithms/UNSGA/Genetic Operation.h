#include  "../Genetic Algorithms.h"
#include <math.h>

class Genetic_Operation : public Genetic
{
private:
	float mu, mum;

public:
	Genetic_Operation(const float& mu, const float& mum) : mu(mu), mum(mum) {}
	virtual void Cross(Individual& father, Individual& mother, Individual& son, Individual& daughter);
	virtual void Mutation(Individual& individual);
};