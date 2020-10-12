#include"EquationSystem.h"


EquationSystem::EquationSystem(size_t _dim) : dim(_dim) {
	A = new double[dim * dim];
	b = new double[dim];
}
EquationSystem::~EquationSystem()
{
	delete A;
	delete b;
};

void EquationSystem::initialize() {
#pragma omp parallel for
	for (size_t i = 0; i < dim; ++i) {
		double diagEl = 0;
		for (size_t j = 0; j < dim; ++j) {
			if (i != j) {
				double value = rand() % 607;
				diagEl += value;
				A[i * dim + j] = value;
			}
		}
		A[i * dim + i] = diagEl * rand() / 94;
		b[i] = rand() % 607;
	}
}

template<typename T>
T computeResidual(const T* vec1, const T* vec2) {
	T residual = 0;
	double diff;
	size_t dim = sizeof(*vec1) / sizeof(vec1[0]);
	for (size_t i = 0; i < dim; ++i) {
		diff = vec1[i] - vec2[i];
		diff = abs(diff);
		if (diff > residual) {
			residual = diff;
		}
	}
	return residual;
}

void EquationSystem::solve()
{
	double* xPrevious = new double[dim];
	double* x = new double[dim];
	const double eps = 1e-7;
	double residual = 100;
#pragma omp parallel for
	for (size_t i = 0; i < dim; ++i) {
		xPrevious[i] = b[i] / A[i * dim + i];
	}
	size_t maxIters = 1000;
	size_t iter = 0;
	while (residual > eps) {
		if (iter > 1) {
#pragma omp parallel for
			for (size_t i = 0; i < dim; ++i) {
				xPrevious[i] = x[i];
			}
		}
#pragma omp parallel for
		for (size_t i = 0; i < dim; ++i) {
			x[i] = b[i];
			for (size_t j = 0; j < dim; ++j) {
				if (i != j) {
					x[i] -= A[i * dim + j] * xPrevious[j];
				}
			}
			x[i] /= A[i * dim + i];
		}
		residual = computeResidual(x, xPrevious);
		//printf("%f\n", residual);
		iter += 1;
		assert(iter < 1e6);
	}
}

