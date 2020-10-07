#include"matrix.h"

Matrix::Matrix(size_t _rowsNumber, size_t _columnsNumber) : 
	rowsNumber(_rowsNumber),
	columnsNumber(_columnsNumber)
{
	matrix = new size_t * [rowsNumber];
	for (int i = 0; i < rowsNumber; ++i)
	{
		matrix[i] = new size_t[columnsNumber];
	}
	for (int i = 0; i < rowsNumber; ++i)
	{
		for (int j = 0; j < columnsNumber; ++j)
		{
			matrix[i][j] = 0;
		}
	}
}

Matrix::~Matrix()
{
	for (int i = 0; i < rowsNumber; i++)
	{
		delete matrix[i];

	}
	delete matrix;
}

size_t Matrix::getRowsNumber()
{
	return rowsNumber;
}

size_t Matrix::getColumnsNumber()
{
	return columnsNumber;
}

void Matrix::initializeWithRandomNumbers()
{
	for (int i = 0; i < rowsNumber; ++i)
	{
		for (int j = 0; j < columnsNumber; ++j)
		{
			this->matrix[i][j] = rand() / 1000;
		}
	}
}

void Matrix::printMatrix()
{
	for (int i = 0; i < rowsNumber; ++i) {
		for (int j = 0; j < columnsNumber; ++j) {
			std::cout << matrix[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}


void Matrix::sum(const Matrix& a, const Matrix& b)
{
	assert(a.rowsNumber == b.rowsNumber);
	assert(a.columnsNumber == b.columnsNumber);
	for (size_t i = 0; i < rowsNumber; ++i) {
		for (size_t j = 0; j < columnsNumber; ++j) {
			matrix[i][j] = a.matrix[i][j] + b.matrix[i][j];
		}
	}
}

void Matrix::sub(const Matrix& a, const Matrix& b)
{
	assert(a.rowsNumber == b.rowsNumber);
	assert(a.columnsNumber == b.columnsNumber);
	for (size_t i = 0; i < rowsNumber; ++i) {
		for (size_t j = 0; j < columnsNumber; ++j) {
			matrix[i][j] = a.matrix[i][j] - b.matrix[i][j];
		}
	}
}

void Matrix::multiplicationNaive(const Matrix& a, const Matrix& b)
{
	assert(a.columnsNumber == b.rowsNumber);
	for (size_t k = 0; k < a.columnsNumber; ++k) {
		for (size_t i = 0; i < a.rowsNumber; ++i) {
			for (size_t j = 0; j < columnsNumber; ++j) {
				matrix[i][j] += a.matrix[i][k] * b.matrix[k][j];
			}
		}
	}
}
