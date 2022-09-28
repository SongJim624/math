#include "broyden.h"

void Jacobian(std::function<void(const float *, float *, size_t)>& system, float * location, float * jacobian, size_t scale)
{
    float * current = (float *) mkl_malloc(scale * sizeof(float), 64);
    system(location, current, scale);

    float * disturbed = (float *) mkl_malloc(scale * sizeof(float), 64);
    float * disturbance = (float *) mkl_malloc(scale * sizeof(float), 64);
    cblas_scopy(scale, location, 1, disturbance, 1);

    for(size_t column = 0; column < scale; ++column)
    {
        if (column > 0)
        {
            disturbance[column - 1] -= 0.00001;
        }

        disturbance[column] += 0.00001;
        system(disturbance, disturbed, scale);

        for(size_t row = 0; row < scale; ++row)
        {
            jacobian[row * scale + column] = 100000 * (disturbed[row] - current[row]);
        }
    }
}

/*
function description :
*/

int Inverse(float* matrix, int scale)
{
    int status = 0;
    int size = scale * scale;

    int* ipv = new int[scale];
    float* workspace = (float*)mkl_malloc(scale * scale * sizeof(float), 64);
    
    sgetrf(&scale, &scale, matrix, &scale, ipv, &status);
    sgetri(&scale, matrix, &scale, ipv, workspace, &size, &status);

    delete[] ipv;
    ipv = nullptr;
    mkl_free(workspace);

    return status;
}


Broyden::Broyden(std::function<void(const float*, float*, size_t)>& target, size_t scale, float secant, float value, size_t maximum, size_t code = 0)
    : target_(target), scale_(scale), maximum_(maximum), secant_(secant), value_(value)
{
    Updator(code);
}

void Broyden::Solve(const float * initial, float * result)
{
    //assign the initial point to the result
    cblas_scopy(scale_, initial, 1, result, 1);

    //allocate variables
    float* secant = (float*)mkl_malloc(scale_ *sizeof(float), 64);
    float * original = (float *) mkl_malloc(scale_ * sizeof(float), 64);
    float * updated = (float *) mkl_malloc(scale_ * sizeof(float), 64);
    float * hessian = (float *) mkl_malloc(scale_ * scale_ * sizeof(float), 64);

    //calculate the values and Jacobian at the initial point
    target_(initial, original, scale_);
    Jacobian(target_, result, hessian, scale_);

    //get the inverse of the Jacobian as the approximation of Hessian, disturbance is used as a temperory storage
    Inverse(hessian, scale_);

    
    //iteration
    for(size_t i = 0; i <  100; ++i)
    {
        //calculate the secant
        cblas_sgemv (CblasRowMajor, CblasNoTrans, scale_, scale_, -1, hessian, scale_, original, 1, 0, secant, 1);

        //update the result and calculate the function value
        vsAdd(scale_, result, secant, result);
        target_(result, updated, scale_);

        const int incr = 1;
        //tolerance check
        if(snrm2(&scale_, secant, &incr) <  secant_ && snrm2(&scale_, updated, &incr) < value_)
        {
            std::cout << "found!" << std::endl;
            std::cout << "result[0] : " << result[0] << std::endl;
            std::cout << "result[1] : " << result[1]  << std::endl;

            break;
        }

        //update the Hessian and assign the new function value to the old one
        updator_(scale_, hessian, secant, original, updated);
    }

    mkl_free(secant);
    mkl_free(hessian);
    mkl_free(original);
    mkl_free(updated);
};