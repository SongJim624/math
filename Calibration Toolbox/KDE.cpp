#include "KDE.h"
#include <iostream>
#include <fstream>

float KDE::regEM(float& del, const std::vector<float>& X,
	float* log_lh, float* log_sig, float* maxll, float* maxlsig, float* density, float* psigd, float* ones)
{
	long N = X.size();

	for (long c = 0; c < size; ++c)
	{
		cblas_scopy(N, &X[0], 1, &log_lh[c], size);
		cblas_saxpy(N, -mu[c], &ones[0], 1, &log_lh[c], size);

		vsPowxI(N, &log_lh[c], size, 2, &log_lh[c], size);
		cblas_sscal(N, 1 / sig[c], &log_lh[c], size);

		cblas_scopy(N, &log_lh[c], size, &log_sig[c], size);
		cblas_saxpby(N, std::numeric_limits<float>::epsilon(), &ones[0], 1, 1 / sig[c], &log_sig[c], size);

		cblas_saxpy(N, 1.8379f + log(sig[c] / (w[c] * w[c])) + del * del / sig[c], &ones[0], 1, &log_lh[c], size);
		cblas_sscal(N, -0.5f, &log_lh[c], size);

		vsLnI(N, &log_sig[c], size, &log_sig[c], size);
		vsAddI(N, &log_sig[c], size, &log_lh[c], size, &log_sig[c], size);
	}

	for (long r = 0; r < 2 * N; ++r)
	{
		maxll[r] = -INFINITY;

		for (long c = 0; c < size; ++c)
		{
			maxll[r] = fmax(log_lh[r * size + c], maxll[r]);
		}
	}

	for (long c = 0; c < size; ++c)
	{
		vsSubI(2 * N, &log_lh[c], size, &maxll[0], 1, &log_lh[c], size);
	}

	vsExp(2 * N * size, &log_lh[0], &log_lh[0]);

	for (long r = 0; r < 2 * N; ++r)
	{
		density[r] = cblas_sasum(size, &log_lh[r * size], 1);
	}

	for (long c = 0; c < size; ++c)
	{
		vsDivI(N, &log_lh[c], size, &density[0], 1, &log_lh[c], size);
		w[c] = cblas_sasum(N, &log_lh[c], size);
	}

	vsLn(2 * N, &density[0], &density[0]);
	vsAdd(2 * N, &density[0], &maxll[0], &density[0]);

	float ent = cblas_sasum(N, &density[0], 1);

	for (long c = 0; c < size; ++c)
	{
		if (w[c] <= 0)
		{
			continue;
		}

		mu[c] = cblas_sdot(N, &log_lh[c], size, &X[0], 1) / w[c];

		memcpy(&maxll[0], &X[0], N * sizeof(float));
		cblas_saxpy(N, -mu[c], &ones[0], 1, &maxll[0], 1);
		vsPowx(N, &maxll[0], 2, &maxll[0]);
		sig[c] = cblas_sdot(N, &log_lh[c], size, &maxll[0], 1) / w[c] + del * del;
	}

	cblas_sscal(size, 1 / cblas_sasum(size, &w[0], 1), &w[0], 1);
	vsSub(N, &psigd[0], &density[0], &density[0]);
	vsExp(N, &density[0], &density[0]);

	del = pow(14.17963 * cblas_sasum(N, &density[0], 1), -0.3);

	return ent;
}
//Only the adaptive method is considered
KDE::KDE(std::vector<float>& X, const float& min, const float& max)
	: size(ceil(powf(X.size(), 0.3f)) + 20), min(min), max(max), 
	scaling(0.1f / (max - min)), mu(new float[size]), 
	sig(new float[size]), w(new float[size])
{
	long N = X.size();

	std::vector<float> ones(N, 1);
	cblas_saxpby(N, -min * scaling, &ones[0], 1, scaling, &X[0], 1);

	memcpy(&mu[0], &X[0], size * sizeof(float));

	vslNewStream(&stream, VSL_BRNG_MCG31, (unsigned long)time(nullptr));

	vsRngUniform(0, stream, size, &w[0], 0, 1);
	cblas_sscal(size, 1 / cblas_sasum(size, &w[0], 1), &w[0], 1);

	float del = 0.2 * powf(N, -0.3);
	vsRngUniform(0, stream, size, &sig[0], 0, 1);
	cblas_sscal(size, del * del, &sig[0], 1);

	std::vector<float> log_lh(2 * N * size, 0);
	std::vector<float> maxll(2 * N, 0);
	std::vector<float> density(2 * N, 0);

	float ent = -INFINITY;
	while(true)
	{
		float Eold = ent;
		ent = regEM(del, X, &log_lh[0], &log_lh[N * size], 
			&maxll[0], &maxll[N], &density[0], &density[N], &ones[0]);

		float err = abs((ent - Eold) / ent);
		if (err < 1e-6) break;
	}

	Build(size);
}

KDE::~KDE()
{
	delete[] mu;
	delete[] sig;
	delete[] w;
	vslDeleteStream(&stream);
}

void KDE::density(float * X, float * Y, long N)
{
//	vsPowx(N, X, 2, Y);
//	cblas_sscal(N, -0.5f, Y, 1);
//	vsExp(N, Y, Y);
//	cblas_sscal(N, 1 / sqrtf(6.28f), Y, 1);

	std::vector<float>ones(N, 1);
	std::vector<float>Xc(N);
	memcpy(&Xc[0], X, N * sizeof(float));
	cblas_saxpby(N, -min * scaling, &ones[0], 1, scaling, &Xc[0], 1);

	std::vector<float> XX(N);
	for (long i = 0; i < size; ++i)
	{
		memcpy(&XX[0], &Xc[0], N * sizeof(float));

		cblas_saxpby(N, -mu[i] / sqrtf(sig[i]), &ones[0], 1, 1 / sqrtf(sig[i]), &XX[0], 1);
		vsPowx(N, &XX[0], 2, &XX[0]);

		cblas_saxpby(N, -0.5f * logf(sig[i] * 6.2832f / (w[i] * w[i])), &ones[0], 1, -0.5f, &XX[0], 1);
		vsExp(N, &XX[0], &XX[0]);
		vsAdd(N, Y, &XX[0], Y);
	}

	cblas_sscal(N, scaling, Y, 1);
	return;
}