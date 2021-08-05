#include "Sampling.h"
#include <limits>
#include "../../Matrix/Matrix.h"

//The DRAM (Delay rejection Adaptive Metropolis) is for the multi-variable distribution

/*
* where the X is the Matrix to store the samples
* size is the length of the input variables
* N is the size of the samples
*/
void Sampling(float ** X, long size, long N)
{
    float sd = powf(2.4f, 2);

    long accepted = 0;

    Matrix R;
    for(long i = 1; i < N; ++i)
    {


        vsRngUniform(size, X[i], ) * R;
    }

    float ratio =  (float) accepted / size;
}


//This should be a recursive process?
bool Delay_Reject(float * X, long size, long stage, float& D, float& N)
{
    float ratio = 0;

    switch(stage)
    {
    case 0:
    {
        float Xn = new float[size];



        D = ;
        N = ;

//if the Xn still needed in the other recursive process still need to investigate?
        delete[] Xn;
        if(rand() > D / N)
        {
            memcpy(X, Xn, size * sizeof(float));
            return true;
        }
        else
        {
            Delay_Reject(X, size, accepted, stage + 1, D, N);
        }
    }
    case stage:
    {
        return;
    }
    default:
    {
        D = q * (D - N);
        N = ;

        Delay_Reject(Xn, size, accepted, stage + 1, D, N);
    }
    }

    if(stage == 1)
    {

    }

    if(stage > this->stage)
    {
        return;
    }

//remeber to delete it!!
    float * Xn = new float [size];

    /*
    process to form the new sample
    */


    float alpha = fminf(1.0f,expf(-0.5*(newss-oldss)/sigma2 -0.5*(newprior-oldprior)));


    if(!accept)
    {
        Delay_Reject(Xn, size, accept, stage + 1, D, N);
    }
}

void Covariance_Update(float * X, long N, long size, float* w, float * covariance, float * mean, float * wsum)
{
    for(long i = 0; i < N; ++i)
    {
        vsAdd(size, mean)
    }


  for i=1:n
    xi     = x(i,:);
    wsum   = w(i);
    xmeann = xi;
    xmean  = oldmean + wsum/(wsum+oldwsum)*(xmeann-oldmean);

    xcov =  oldcov + ...
            wsum./(wsum+oldwsum-1) ...
            .* (oldwsum/(wsum+oldwsum) ...
                .* ((xi-oldmean)' *(xi-oldmean))  ...
                - oldcov);
    wsum    = wsum+oldwsum;
    oldcov  = xcov;
    oldmean = xmean;
    oldwsum = wsum;
  end
  
else % no update

  wsum  = sum(w);
  xmean = zeros(1,p);
  xcov  = zeros(p,p);
  for i=1:p
    xmean(i) = sum(x(:,i).*w)./wsum;
  end
  if wsum>1
    %%% (wsum-oldwsum/wsum)
    for i=1:p
      for j=1:i
        xcov(i,j) = (x(:,i)-xmean(i))' * ((x(:,j)-xmean(j)).*w)./(wsum-1);
        if (i ~= j)
          xcov(j,i) = xcov(i,j);
        end
      end
    end
  end



}