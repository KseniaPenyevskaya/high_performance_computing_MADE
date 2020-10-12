#pragma once
#include"omp.h"
#include<cassert>
#include<cmath>


class EquationSystem {
	//Ax + b = 0
public:
	EquationSystem(size_t dim);
	~EquationSystem();
	void initialize();
	void solve();
private:
	size_t dim;
	double* A;
	double* b;
};