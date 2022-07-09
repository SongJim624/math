
//std::vector<float> Doolittle(const Matrix& A, std::vector<float>& b)

void Doolittle(float ** &A, float * &b, size_t &num, float * &x)
{
	for (size_t i = 0; i < num; ++i)
	{
		A[0][i] = A[0][i];
		A[i][0] = A[i][0] / A[0][0];
	}

	float sum;
	size_t r = 1;
	for (; r < num - 1; ++r)
	{
		size_t i = r;
		sum = 0.0;
		for (size_t k = 0; k < r; ++k)
		{
			sum += A[r][k] * A[k][i];
		}
		A[r][i] = A[r][i] - sum;
		++i;

		for (; i < num; ++i)
		{
			sum = 0.0;
			for (size_t k = 0; k < r; ++k)
			{
				sum += A[r][k] * A[k][i];
			}

			A[r][i] -=  sum;

			sum = 0;
			for (size_t k = 0; k < r; ++k)
			{
				sum += A[i][k] * A[k][r];
			}

			A[i][r] = (A[i][r] - sum) / A[r][r];
		}
	}

	sum = 0.0;
	for (size_t i = 0; i < r; ++i)
	{
		sum += A[r][i] * A[i][r];
	}
	A[r][r] -= sum;


	x[0] = b[0];
	for (size_t i = 1; i < num; ++i)
	{
		sum = 0;

		for (size_t k = 0; k < i; ++k)
		{
			sum += A[i][k] * x[k];
		}

		x[i] = b[i] - sum;
	}

	x[num - 1] = x[num - 1] / A[num - 1][num - 1];

	for (int i = num - 2; i >= 0; --i)
	{
		sum = 0.0;
		for (size_t k = i + 1; k < num; ++k)
		{
			sum += A[i][k] * x[k];
		}

		x[i] = (x[i] - sum) / A[i][i];
	}
}