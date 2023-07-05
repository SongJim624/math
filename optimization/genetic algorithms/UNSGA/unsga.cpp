#include "unsga.h"

const math::Optimizor::Result& UNSGA::Optimize(Optimizor::Configuration& configuration)
{
    population_ = std::make_unique<Population>(configuration);
    return population_->evolve(std::get<size_t>(configuration["maximum"]));
}
