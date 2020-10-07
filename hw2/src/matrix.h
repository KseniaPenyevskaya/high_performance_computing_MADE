#pragma once
#include <iostream>
#include <random>
#include <cassert>

class Matrix {
public:
	Matrix(size_t rowsNumber, size_t columnsNumber);
	~Matrix();
	size_t getRowsNumber();
	size_t getColumnsNumber();
	void initializeWithRandomNumbers();
	void printMatrix();
	void sum(const Matrix& a, const Matrix& b);
	void sub(const Matrix& a, const Matrix& b);
	void multiplicationNaive(const Matrix& a, const Matrix& b);
private:
	size_t rowsNumber;
	size_t columnsNumber;
	size_t **matrix;
};
