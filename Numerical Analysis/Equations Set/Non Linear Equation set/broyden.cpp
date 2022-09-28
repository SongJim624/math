#include "broyden.h"

void Jacobian(std::function<void(float *, float *, size_t)>& system, float * location, float * jacobian, size_ t scale)
{
    float * current = (float *) mkl_malloc(scale * sizeof(float), 64);
    system(location, current, scale);

    float * disturbed = (float *) mkl_malloc(scale * sizeof(float), 64);
    float disturbance = (float *) mkl_malloc(scale * sizeof(float), 64);
    cblas_sscopy(scale, current, 1, disturbance, 1);

    for(auto& function : functions)

    for(size_t i = 0; i < scale; ++i)
    {
        if (i > 0)
        {
            disturbance[i - 1] -= 0.001;
        }

        disturbance[i] += 0.001;
        system(disturbance, disturbed, scale);

        for(size_t j = 0; j < scale; ++j)
        {
            jacobian[i * size + j] = 1000 * (disturbed[j] - current[j]);
        }
    }
}

/*
function description :
*/
void Broyden::Solve(const float * initial, float * result)
{
    //assign the initial point to the result
    cblas_sscopy(initial, result, scale_);

    //allocate variables
    float * secant = (float *) mkl_malloc(length * sizeof(float), 64);
    float * original = (float *) mkl_malloc(length * sizeof(float), 64);
    float * updated = (float *) mkl_malloc(length * sizeof(float), 64);
    float * hessian = (float *) mkl_malloc(length * length * sizeof(float), 64);

    //calculate the values and Jacobian at the initial point
    target_(initial, original, scale_);
    Jacobian(target_, initial, hessian, scale_);

    //get the inverse of the Jacobian as the approximation of Hessian, disturbance is used as a temperory storage
    int status = sgetrf(length, length, hessian, length, disturbance);
    status = sgetri(length, hessian, length, disturbance);

    //iteration
    for(size_t i = 0; i <  maiximum; ++i)
    {
        //calculate the secant
        cblas_sgemv (CblasRowMajor, CblasNoTrans, length, length, -1, hessian, 1, original, 1, 0, secant, 1);

        //update the result and calculate the function value
        vsAdd(scale_, result, secant, result);
        target_(result, updated, scale_);

        //tolerance check
        if(norm(sceant) <  relative_ || norm(updated) < absolute_)
        {
            break;
        }

        //update the Hessian and assign the new function value to the old one
        updater_(hessian, secant, original, updated);
    }

    mkl_free(secant);
    mkl_free(hessian);
    mkl_free(original);
    mkl_free(update);
};