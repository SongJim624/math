#include "unsga.h"

const math::Optimizor::Result& math::UNSGA::Optimize(Optimizor::Configuration& configuration)
{
    population_ = std::make_unique<Population>(configuration);
    return reinterpret_cast<Optimizor::Result*>((*population_)(std::get<size_t>(configuration["maximum"]));
}
