#include "omp.h"
#include <ctime>
#include <iostream>

const size_t N = 10;
double EPS = 1e-5;


void matrixMult(double* matrix1, double* matrix2) {
	double result[N * N];
#pragma omp parallel for private(i, j)
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			result[i * N + j] = 0;
		}
	}
#pragma omp parallel for private(i, j, k)
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			for (size_t k = 0; k < N; ++k) {
				result[i * N + j] += matrix1[i * N + k] * matrix2[k * N + j];
			}
		}
	}
#pragma omp parallel for private(i, j)
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			matrix1[i * N + j] = result[i * N + j];
		}
	}
}

void matrixVectorMul(const double* matrix, double* vector)
{
	double result[N];
#pragma omp parallel for private(i)
	for (size_t i = 0; i < N; ++i)
		result[i] = 0;
#pragma omp parallel for private(i, j)
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			result[i] += matrix[i * N + j] * vector[j];
#pragma omp parallel for private(i)
	for (size_t i = 0; i < N; ++i)
		vector[i] = result[i];
}


void matrixPow(double* matrix, size_t POW) {
	double result[N * N];
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			result[i * N + j] = 0;
			if (i == j) {
				result[i * N + j] = 1;
			}
		}
	}
	size_t k = POW;
	while (k)
		if (k & 1) {
			matrixMult(result, matrix);
			--k;
		}
		else {
			matrixMult(matrix, matrix);
			k >>= 1;
		}
	for (size_t i = 0; i < N; ++i) {
		for (size_t j = 0; j < N; ++j) {
			matrix[i * N + j] = result[i * N + j];
		}
	}
}

int main(int argc, char* argv[]) {
		size_t POW = 5;
		double matrix[N * N];
		for (size_t i = 0; i < N; ++i) {
			for (size_t j = 0; j < N; ++j) {
				matrix[i * N + j] = 1 + i % 7 + j % 15;
			}
		}                                                                                                   
		matrixPow(matrix, POW);
}
