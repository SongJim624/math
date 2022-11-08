#include "broyden.h"

void Jacobian(std::function<void(const float *, size_t, float *, size_t)>& system, size_t row, size_t column, float * middle, float * jacobian)
{
    float * left = (float *) mkl_malloc(column * sizeof(float), 64);
    float * right = (float *) mkl_malloc(column * sizeof(float), 64);
 
    float* before = (float*)mkl_malloc(row * sizeof(float), 64);
    float* after = (float*)mkl_malloc(row * sizeof(float), 64);

    cblas_scopy(column, middle, 1, left, 1);
    cblas_scopy(column, middle, 1, right, 1);

    for(size_t c = 0; c < column; ++c)
    {
        if (c > 0)
        {
            left[c - 1] += 0.0000005;
            right [c - 1] -= 0.0000005;
        }

        left[c] -= 0.0000005;
        right[c] += 0.0000005;

        system(left, column, before, row);
        system(right, column, after, row);

        for(size_t r = 0; r < row; ++r)
        {
            jacobian[r * column + c] = 1e6 * (after[r] - before[r]);
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

Broyden::Broyden(std::function<void(const float*, size_t, float*, size_t)> target, size_t scale, float secant, float value, size_t maximum, size_t code = 0)
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
    target_(initial, scale_, original, scale_);
    Jacobian(target_, scale_, scale_, result, hessian);

    //get the inverse of the Jacobian as the approximation of Hessian, disturbance is used as a temperory storage
    Inverse(hessian, scale_);

    
    //iteration
    for(size_t i = 0; i <  100; ++i)
    {
        //calculate the secant
        cblas_sgemv (CblasRowMajor, CblasNoTrans, scale_, scale_, -1, hessian, scale_, original, 1, 0, secant, 1);

        //update the result and calculate the function value
        vsAdd(scale_, result, secant, result);
        target_(result, scale_, updated, scale_);

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