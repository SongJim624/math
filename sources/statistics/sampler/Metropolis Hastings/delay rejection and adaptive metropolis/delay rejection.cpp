#include "distribution.h"

enum class Acceptance
{
    rejected = 0,
    first = 1,
    second = 2
};


DRAM::Proposal * next(std::vector<DRAM::Proposal>& proposals)
{




}

Acceptance Rejection(std::vector<Proposal>& proposals, double * covariation, double& variance)
{
    auto alpha = [&sigma2](const Proposal& proposed, const Proposal& current)
    {
        return fexp(-0.5 * fmax(0.0, (proposed.likely - current.likely) / sigma2 - (proposed.prior - current.prior)));
    };

    double *

    std::gamma_distribution<double> gamma((n0 + n) ./2, 2. / (n0 * s20 + proposals[0].likely));
    gamma()


    std::random_device random_;
    std::mt19937_64 generator_(random_);
    variance = 1./ gamma(generator_);
}

Acceptance Rejection(std::vector<Proposal>& proposals, double * covariation, double variance)
{
    auto alpha = [&sigma2](const Proposal& proposed, const Proposal& current)
    {
        return fexp(-0.5 * fmax(0.0, (proposed.likely - current.likely) / sigma2 - (proposed.prior - current.prior)));
    };




}

{
        bool reasonable = proposal.first.Propose();

        double alpha12 = alpha(proposals[1], proposals[0]);


        Acceptance staus = Acceptance::rejected;

        if(randoms[0] < alpha12)
        {
            accepted++;
            acceptance = first;
        }
        else if(delay_rejection)
        {
            proposals[2].Propose(proposals[0].proposal, covarinace);

            double alpha32 = alpha(proposals[1], proposals[2]);
            double l2 = exp(-0.5 * (newss2-oldss) / sigma2 - 0.5 * (newprior2 - oldprior));
            double q1 = exp(-0.5*(norm((newpar2-newpar)*iR)^2-norm((oldpar-newpar)*iR)^2));
            double alpha13 = l2*q1*(1-alpha32)/(1-alpha12);
        }

        cblas_dcopy(dimension, proposals[(size_t) status].proposal, 1, samples + r * dimension, 1);
}