#include <broyden.h>

Broden::Broyden()
{

}

/*
function description :
*/
Broyden::Solve(const std::vector<float>& initial)
{
    //allocate the objectives
    float * secant = (float *) mkl_malloc(length * sizeof(float), 64);
    float * current = (float *) mkl_malloc(length * sizeof(float), 64);
    float * updated = (float *) mkl_malloc(length * sizeof(float), 64);
    //allocate the decisions
    float * solution = (float *) mkl_malloc(length * sizeof(float), 64);
    float * disturbance = (float *) mkl_malloc(length * sizeof(float), 64);

    //allocate the matrices
    float * hessian = (float *) mkl_malloc(length * length * sizeof(float), 64);
    float * increment = (float *) mkl_malloc(length * length * sizeof(float), 64);
    //TODO: temperory should be move to the updater
//    float * temperory = (float *) mkl_malloc(length * length * sizeof(float), 64);

    //assign the values to the decisions
    cblas_scopy(length, &initial[0], 1, solution, 1);

    cblas_scopy(length, &initial[0], 1, disturbance, 1);
    cblas_sscale(length, 0.999f, disturbance, 1);

    //calculating the numerical Jacobi matrix
    function(solution, current);
    function(disturbance, updated);

    for(size_t i = 0; i < length; ++i)
    {
        for(size_t j = 0; j < length; ++j)
        {
            hessian[length * (i - 1) + j] = (current[i] - updated[i]) / (solution[j] - disturbance[j]);
        }
    }

    //get the inverse of the Jacobi matrix, disturbance is used as a temperory storage
    int status = sgetrf(length, length, hessian, length, disturbance);
    status = sgetri(length, hessian, length, disturbance);

    //iteration
    for(size_t i = 0; i <  maiximum; ++i)
    {
        //calculate the secant
        cblas_sgemv (CblasRowMajor, CblasNoTrans, length, length, -1, hessian, 1, current, 1, 0, secant, 1);

        //update the solution
        vsAdd(solution, secant, solution);

        //update the value of the functions
        function(solution, updated);

        //tolerance check
        if()
        {
            break;
        }

        //calculate the difference between the current objectives and the updated objectives
        //and the answer is stored in the current objectives
        vsSub(updated, current, current);

        //caluclate the increment of the approximation of Hessian matrix
        status = updater_(increment);

        /*
        =============================================================================
        this is the only place where different types of Broyden method differs from the other
        status = updater(hessian, secant, difference, increment)
        */

/*
       //calculate the B_{k}y_{k}, result is stored in the variable increment
        cblas_sgemv(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, hessian, 1, current, 1, 0, increment, 1);
        //calculating the \mu
        float factor = 1 + cblas_sdot(length, current, 1, increment, 1) / cblas_dot(length, secant, 1, current, 1);

        //calculate the denominator for the increment
        float denominator = 1 / cblas_sdot(length, secant, 1, current, 1);

        //calculat the first item in the increment
        cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, factor, secant, denominator, secant, 1, 0, increment, 1);

        //calculate the second item in the increment
        cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, 1, secant, 1, current, 1, 0, temperory, 1);
        cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, length, -1, temperory, denominator, hessian, 1, 1, increment, 1);

        //calculate the third item in the increment
        cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, 1, 1, current, 1, secant, 1, 0, temperory, 1);
        cblas_gemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, length, length, length, -1, hessian, denominator, temperory, 1, 1, increment, 1);
*/
        /*
        difference ends
        =======================================================
        */

        //the updated objectives become the current objectives for the next iteration
        cblas_scopy(length, updated, 1, current, 1);

        //update the approximation of Hessian matrix
        vsAdd(hessian, increment, hessian)
    }

    //clear the arrays
    mkl_free();
};