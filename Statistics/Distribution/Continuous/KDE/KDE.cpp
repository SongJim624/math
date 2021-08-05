#include "KDE.h"
#include <algorithm>
#include "Kernels.h"
#include "time.h"
#define pi 3.1415926

T regEM(std::vector<T>& w, std::vector<T>& mu, std::vector<T>& Sig, T& del, const std::vector<T>& X)
{
	std::vector<std::vector<T>> log_lh(X.size(), std::vector<T>(mu.size()));
	std::vector<std::vector<T>> log_sig(X.size(), std::vector<T>(mu.size()));
	std::vector<std::vector<T>> p(X.size(), std::vector<T>(mu.size()));
	std::vector<std::vector<T>> psig(X.size(), std::vector<T>(mu.size()));

	for (size_t i = 0; i < mu.size(); ++i)
	{
		for (size_t j = 0; j < X.size(); ++j)
		{
			T Xcentered = X[j] - mu[i];
			T XRinv = Xcentered * Xcentered / Sig[i];
			T XSig = XRinv / Sig[i] + std::numeric_limits<T>::epsilon();

			log_lh[j][i] = 0.5 * (-XRinv - log(2.0 * Sig[i] * pi / (w[i] * w[i])) - del * del / Sig[i]);
			log_sig[j][i] = log_lh[j][i] + log(XSig);
		}
	}
	std::vector<T> maxll = Max(log_lh);
	std::vector<T> maxlsig = Max(log_sig);
	
	for (size_t i = 0; i < X.size(); ++i)
	{
		for (size_t j = 0; j < mu.size(); ++j)
		{
			p[i][j] = exp(log_lh[i][j] - maxll[i]);
			psig[i][j] = exp(log_sig[i][j] - maxlsig[i]);
		}
	}

	std::vector<T> density = sum(p);
	std::vector<T> psigd = sum(psig);

	std::vector<T> logpdf(density.size());
	std::vector<T> logpsigd(psigd.size());
	for (size_t i = 0; i < density.size(); ++i)
	{
		logpdf[i] = log(density[i]) + maxll[i];
		logpsigd[i] = log(psigd[i]) + maxlsig[i];
	}

	for (size_t i = 0; i < p.size(); ++i)
	{
		for (size_t j = 0; j < p[0].size(); ++j)
		{
			p[i][j] /= density[i];
		}
	}
	T ent = sum(logpdf); 
	w = sum(p, '2');

	for (size_t i = 0; i < mu.size(); ++i)
	{
		if (w[i] < 0) { continue; }

		std::vector<T> A(X.size());

		for (size_t k = 0; k < X.size(); ++k)
		{
			A[k] = p[k][i];
		}

		mu[i] = cross(A, X) / w[i];
		std::vector<T> Xcentered(X.size());

		for (size_t k = 0; k < X.size(); ++k)
		{
			Xcentered[k] = X[k] - mu[i];
			Xcentered[k] *= Xcentered[k];
		}

		Sig[i] = cross(A, Xcentered) / w[i] + del * del;
	}

	div(w, T(sum(w)));
	T curv = 0;

	for (size_t i = 0; i < logpsigd.size(); ++i)
	{
		curv += exp(logpsigd[i] - logpdf[i]);
	}
	curv /= logpsigd.size();

	del = pow(8 * X.size() * sqrt(pi) * curv, -0.333333);

	return ent;
}

template<typename T>
std::vector<size_t> Permute(const size_t& size)
{
	std::vector<size_t> res(size, 0);
	for(size_t i = 0; i < size; ++i)
	{
		res[i] = i;
	}

	for(size_t i = 0; i < size; ++i)
	{
		size_t index = i + rand() % (size - i);
		std::swap(res[i], res[index]);
	}

	return res;
}

//Only the adaptive method is considered
template<typename T>
KDE<T>::KDE (const std::vector<T>& x) :
	mu(std::vector<T>(ceil(pow(x.size(), 0.3333333)) + 20, 0)), 
	sig(std::vector<T>(ceil(pow(x.size(), 0.3333333)) + 20, 0)),
	w(std::vector<T>(ceil(pow(x.size(), 0.3333333)) + 20, 0))
{
	srand(time(NULL));

	std::vector<T> X = x;
	size_t num = X.size();
	std::sort(X.begin(), X.end());

	MAX = *(X.end() - 1);
	MIN = X[0];
	scaling = MAX - MIN;

	MAX += 0.1 * scaling;
	MIN -= 0.1 * scaling;
	scaling = MAX - MIN;

	for(size_t i = 0; i < X.size(); ++i)
	{
		X[i] -= MIN;
		X[i] /= scaling;
	}

//	size_t gam = ceil(pow(num, 0.3333333)) + 20;

	T del= 0.2 * pow(X.size(), -0.2);
	std::vector<size_t> perm = Permute<T> (X.size());

	for (size_t i = 0; i < mu.size(); ++i)
	{
		mu[i] = X[perm[i]];
	}

	for (size_t i = 0; i < w.size(); ++i)
	{
		w[i] = (T) rand() / RAND_MAX;
	}

	div(w, sum(w));

	for (size_t i = 0; i < sig.size(); ++i)
	{
		sig[i] = del * del * rand() / (T)RAND_MAX;
	}

//end of pre-process
	T ent = -INFINITY;

	for (size_t i = 0; i < 1500; ++i)
	{
		T Eold = ent;
		ent = regEM(w, mu, sig, del, X);// update parameters;
		
		T err = abs((ent - Eold) / ent);
		if (err < 1e-6) { break; }
	}
}

void KDE::density(float * X, float * Y, long size)
{
	float * x = new float [length];

	float * temp = new float [length];
	vsSqrt(length, sig, temp);

	for(long i = 0;  i < size; ++i)
	{
		for(long j = 0; j < length; ++j)
		{
			x[j] = (X[i] - MIN);
		}
		cblas_sscal(length, 1 / scaling, x);

		vsSub(length, x, mu, temp);
		vsMul(length, x, temp, temp);
		vsDiv(length, x, sig, temp);
		cblas_sscal(legnth, -0.5, x);
		vsExp(length, x, x);
		vsDiv(length, x, temp, x);
		cblas_sscal(length, 1 / sqrtf(2 * pi), x, 1);

		vsMul(legnth, x, w, x);
		Y[i] = cblas_sasum(length, x, 1);
	}

	cblas_sscal(size, 1 / scaling, Y);
	delete[] temp;
	return;
}