#include "dram.h"

double * Proposal::upper_, lower;
std::function<double(size_t, const double*)> * Proposal::likely_, prior_;

bool Proposal::Propose(const Proposal& origin, const double * covariation)
{
    vdRngGaussian(0, stream_, dimension_, proposal, 0, 1);
    cblas_dgemm (CblasRowMajor, CblasNoTrans, CblasNoTrans, 1, dimension_, dimension_, 1, proposal, 1, covariation, 1, 0, proposal, 1);
    vsAdd(dimension_, origin.proposal, proposal, proposal);
}