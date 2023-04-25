#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include "../../optimizer.h"

#ifndef _MATH_OPTIMIZTION_UNSGA_RESULTS_
#define _MATH_OPTIMIZTION_UNSGA_RESULTS_
template<typename T>
class Result : public Optimization::Result<T>
{

};
#endif //! _MATH_OPTIMIZTION_UNSGA_RESULTS_