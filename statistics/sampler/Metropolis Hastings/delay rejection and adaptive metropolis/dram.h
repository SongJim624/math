#include <random>
#include <mkl.h>
#include <functional>

class DRAM
{
private:
    class Proposal;
// delay rejection proposor
    class Rejection;

private:
    std::random_device device_;
    std::mt19937_64 generator_;
    std::uniform_real_distribution<double> uniform_;
    std::gamma_distribution<double> uniform_;

    VSLStreamPtr stream_;
    double frequency_, adpatation_, delay_, update_;

    size_t dimension_;
    double * covariance_;

    std::unique_ptr<Rejection> proposor_;

private:
    double acceptance(const Proposal& first, const Proposal& second) const;

public:
    double * sample(size_t size, Distribution * distribution, Distribution * proposal);
};

class DRAM::Proposal
{
private:
    size_t dimesion_;

private:
    static double * upper_, * lower_;
    static std::function<double(size_t, const double *)> * likely_, * prior_;

private:
    bool check();

public:
    double * proposal, likely, prior;
    bool Propose(const Proposal& origin, const double * covariance);

public:
    Proposal(size_t dimension);
    Proposal(size_t dimension, double * point);
    ~Proposal();

public:
    static void Initialize(double * upper, double * lower, );
};

class DRAM::Rejection
{
public:
    Proposal * next(std::vector<Proposal>& proposals);
};