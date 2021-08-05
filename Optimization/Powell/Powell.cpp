#include "Matrix.h"

Powell::Powell(Function* function, const long& decisions, float * initial) :
	function(function), size(decisions), decision(new float[size]), directions(std::vector<float *>(size, new float *[size]))
{
	memcpy(decision, initial, size * sizeof(float));

	for(long i = 0; i < size; ++i)
	{
		directions[i] = 1.0f;
	}
}

/*
* One dimesion search
* return the optimal step of the search direction
* NOTICE: this method is only suitable for single peak functions
*/
float Powell::Search(Function* function, float* X0, float* direction)
{
	//to settle the intial search interval
	float left = 0, step = 1, right = 1, l = 0, r = 0;
	float* X_temp = new float [size] { 0 };

	memcpy(X_temp, direction, size * sizeof(float));
	cblas_saxpby(size, 1, X0, 1, left, X_temp, 1);
	function->function(X_temp, size, &l, 1);

	memcpy(X_temp, direction, size * sizeof(float));
	cblas_saxpby(size, 1, X0, 1, right, X_temp, 1);
	function->function(X_temp, size, &r, 1);

	if (r != l)
	{
		bool status = r < l;
		float side = status ? left : right;

		while (status == (r < l))
		{
			float temp = left;

			left = r < l ? right: left - step;
			right = r < l ? right + step : temp;

			memcpy(X_temp, direction, size * sizeof(float));
			cblas_saxpby(size, 1, X0, 1, left, X_temp, 1);
			function->function(X_temp, size, &l, 1);

			memcpy(X_temp, direction, size * sizeof(float));
			cblas_saxpby(size, 1, X0, 1, right, X_temp, 1);
			function->function(X_temp, size, &r, 1);
		}

		status ? left = side : right = side;
	}

	float left_temp = left + 0.382f * (right - left);
	float right_temp = left + 0.618f * (right - left);

	memcpy(X_temp, direction, size * sizeof(float));
	cblas_saxpby(size, 1, X0, 1, left_temp, X_temp, 1);
	function->function(X_temp, size, &l, 1);

	memcpy(X_temp, direction, size * sizeof(float));
	cblas_saxpby(size, 1, X0, 1, right_temp, X_temp, 1);
	function->function(X_temp, size, &r, 1);

	while ((right - left) > 1e-6f)
	{
		bool status = l < r;

		status ? 
			right = right_temp, right_temp = left_temp, left_temp = left + 0.382f * (right - left),r = l :
			left = left_temp, left_temp = right_temp, right_temp = left + 0.618f * (right -left), l = r;

		memcpy(X_temp, direction, size * sizeof(float));
		cblas_saxpby(size, 1, X0, 1, status ? left_temp : right_temp, X_temp, 1);
		function->function(X_temp, size, status ? &l : &r, 1);
	}

	delete[] X_temp;
	X_temp = nullptr;
	return 0.5f * (left + right);
}


//traditional powell
void Powell::Optimize(const char* results)
{
	long count = 0; 
	while()
	{
		for(auto& iter : directions)
		{
			float lamda = Search(function, decision, *iter);
			cblas_saxpby(size, lambdas[i], directions[i], 1, 1, *iter, 1);
			function->function(decision, size, &values[i], 1);
		}

	//Update the directions: traditional Powell
		delete[] *directions.begin();
		directions.pop_front();

		directions.push_back(new float [size]);
		vsSub(size, decision, initial, *directions.rbegin());

		float lambda = Search(funtion, initial, *directions.rbegin());
		count ++;

	//Sargen Powell
		if(lambda < values[size] -  values[0])
		{

		}


	}
}