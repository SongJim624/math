#include <mkl.h>

template<typename T>
class Matrix
{
public:
	virtual T& operator() (const long& r, const long& c) = 0;
};