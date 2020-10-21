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

void normalizeColumns(double* matrix)
{
	double colSum[N];
#pragma omp parallel for private(i)
	for (size_t i = 0; i < N; ++i)
		colSum[i] = 0;
#pragma omp parallel for private(i, j)
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			colSum[i] += matrix[j * N + i];
#pragma omp parallel for private(i, j)
	for (size_t i = 0; i < N; ++i)
		for (size_t j = 0; j < N; ++j)
			if (abs(colSum[j]) > 1e-3)
				matrix[i * N + j] /= colSum[j];
}

double l2(const double* vector)
{
	double sum = 0;
#pragma omp parallel for reduction(+: sum)
	for (size_t i = 0; i < N; ++i)
		sum += vector[i] * vector[i];
	return sqrt(sum);
}

double normalizeVectorL2(double* vector, double l2Norm)
{
	if (abs(l2Norm) > 1e-4)
	{
#pragma omp parallel for private(i)
		for (size_t i = 0; i < N; ++i)
			vector[i] /= l2Norm;
	}
	return l2Norm;
}


void pageRank(double* matrix, double* x)
{
	double previousL2;
	double L2;
	double residual = EPS + 10;
#pragma omp parallel for private(i)
	for (size_t i = 0; i < N; ++i)
		x[i] = 5;

	L2 = l2(x);
	normalizeVectorL2(x, L2);
	normalizeColumns(matrix);
	matrixVectorMul(matrix, x);
	previousL2 = l2(x);
	normalizeVectorL2(x, previousL2);
	
	while(residual >= EPS)
	{
		L2 = previousL2;
		matrixVectorMul(matrix, x);
		previousL2 = l2(x);
		normalizeVectorL2(x, previousL2);
		residual = abs(L2 - previousL2);
		printf("residual = %f\n", residual);
	}
}

void naiveRank(double* matrix, double* x) {
#pragma omp parallel for private(i)
	for (size_t i = 0; i < N; ++i)
	{
		x[i] = 0;
		for (size_t j = 0; j < N; ++j)
			x[i] += matrix[i * N + j];
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
		double pRank[N];
		double nRank[N];
		pageRank(matrix, pRank);
		for (int i = 0; i < N; ++i) {
			std::cout << pRank[i] << " ";
		}
		std::cout << '\n';
		naiveRank(matrix, nRank);
}
