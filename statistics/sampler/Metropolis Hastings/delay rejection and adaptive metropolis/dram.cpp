#include "Sampling.h"
#include <limits>
#include <functional>
#include <random>

enum Acceptance
{
    rejected = 0,
    first = 1;
    second = 2;
};


double DRAM::acceptance(double variance, const Proposal& first, const Proposal& second) const
{
    return expf(-0.5 * min(0, (first.likely - second.likely) / variance - (first.prior - second.prior)));
}

double * DRAM::sample(size_t simulations, double * samples)
{
    std::array<Proposal, 3> proposals;

    for(size_t s = 2; s < simulations + 1; ++s)
    {
        proposor.Propose(proposals);
        cblas_dcopy(dimension_, proposals[0].proposal, 1, samples + s * dimension_, 1);

    }

    return samples;
}


/*
for isimu=2:nsimu

  if isimu/printint == fix(isimu/printint) % info on every printint iteration
    fprintf('isimu=%d, %d%% done, accepted: %d%%\n',...
            isimu,fix(isimu/nsimu*100),fix((acce/isimu)*100));
  end
  
  newpar = oldpar+randn(1,npar)*R;     % a new proposal

  accept = 0;
  % check bounds
  if any(newpar<bounds(1,:)) | any(newpar>bounds(2,:))
    newss = Inf;
    newprior = 0;
    alpha12 = 0;
  else % inside bounds, check if accepted
    newss  = feval(ssfun,newpar,data);   % sum-of-squares
    newprior = feval(priorfun,newpar,params); % prior ss
    alpha12 = min(1,exp(-0.5*(newss-oldss)/sigma2 -0.5*(newprior-oldprior)));
    if rand < alpha12 % we accept
      accept   = 1;
      acce     = acce+1;
      oldpar   = newpar;
      oldss    = newss;
      oldprior = newprior;
    end
  end
  if accept == 0 & dodr % we reject, but make a new try (DR)
    newpar2 = oldpar+randn(1,npar)*R2;  % a new try

    if any(newpar2<bounds(1,:)) | any(newpar2>bounds(2,:))
      newss2 = Inf;
      newprior2 = 0;
    else % inside bounds
      newss2    = feval(ssfun,newpar2,data);
      newprior2 = feval(priorfun,newpar2,params);
      alpha32 = min(1,exp(-0.5*(newss-newss2)/sigma2 -0.5*(newprior-newprior2)));
      l2 = exp(-0.5*(newss2-oldss)/sigma2 - 0.5*(newprior2-oldprior));
      q1 = exp(-0.5*(norm((newpar2-newpar)*iR)^2-norm((oldpar-newpar)*iR)^2));
      alpha13 = l2*q1*(1-alpha32)/(1-alpha12);
      if rand < alpha13 % we accept
        accept = 1;
        acce     = acce+1;
        oldpar   = newpar2;
        oldss    = newss2;
        oldprior = newprior2;
      end
    end
  end
  chain(isimu,:) = oldpar; 
  % update the error variance sigma2
  if s20 > 0
    sigma2  = 1./gammar_mt(1,1,(n0+n)./2,2./(n0*s20+oldss));
    s2chain(isimu,:) = sigma2;
  end
  
  if adaptint>0 & fix(isimu/adaptint) == isimu/adaptint
    % adapt the proposal covariances
    if verbosity, fprintf('adapting\n'); end
    % update covariance and mean of the chain
    [chaincov,chainmean,wsum] = covupd(chain((lasti+1):isimu,:),1, ...
                                       chaincov,chainmean,wsum);
    lasti = isimu;
    [Ra,is] = chol(chaincov + eye(npar)*qcovadj);
    if is % singular cmat
      fprintf('Warning cmat singular, not adapting\n');
    else
      R = Ra*adascale;
      if dodr  
        R2 = R./drscale;     % second proposal for DR try
        iR = inv(R);
      end
    end
  end
  
end

% calculate covariance and mean of the chain
[chaincov,chainmean,wsum] = covupd(chain((lasti+1):isimu,:),1, chaincov,chainmean,wsum);


results.class = 'MCMC results';
results.accepted=acce./nsimu;              % acceptance ratio
results.mean = chainmean;
results.cov  = chaincov;
results.qcov = R'*R;
results.R = R;
results.nsimu = nsimu;
results.drscale = drscale;
results.adascale = adascale;
results.adaptint = adaptint;

%%%%%%%%
function y=getpar(options,par,default)
%GETPAR get parameter value from a struct
% options   options struct
% par       parameter value to extract from the struct
% default   default value if par is not a member of the options struct

if isfield(options,par)
  y = getfield(options,par);
elseif nargin>2
  y = default;
else
  error(sprintf('Need value for option: %s',par));
end
*/