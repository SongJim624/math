#include "-metropolis hastings.h"

std::variant<double, std::vector<double>, std::string, bool> MetropolisHasting::Option::operator [] (const std::string& name)
{
    return options_[name];
}

std::variant<double, std::vector<double>, std::string, bool> MetropolisHasting::Option::operator [] (const std::string& name) const
{
    auto iter = options_.find(name);
    assert(());
    return iter->second;
}

MetropolisHasting::MetropolisHasting(const std::string& name, const Option& options)
{
    Strategies(name, options);
}

void MetropolisHasting::Strategies(const std::string& name, const Option& options)
{
    if(name == "delay rejection adaptive metropolis")
    {

    }

    if(name == "independent")
    {

    }

    if(name == "gamma")
    {

    }
}

double *MetropolisHasting::sample(size_t length, double * samples)
{
    return sampler_(length, samples);
}
