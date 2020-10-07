#include"matrix.h"
#include<iostream>
#include <random>
#include <ctime>
#include <chrono>


int main(int argc, char* argv[]) {
	size_t iterations = 3;
	srand(unsigned(time(0)));
	Matrix a(atoi(argv[1]), atoi(argv[2]));
	Matrix b(atoi(argv[3]), atoi(argv[4]));
	a.initializeWithRandomNumbers();
	b.initializeWithRandomNumbers();
	Matrix c(a.getRowsNumber(), b.getColumnsNumber());
	auto start = std::chrono::steady_clock::now();
	for (int it = 0; it < iterations; ++it) {
		c.multiplicationNaive(a, b);
	}
	auto end = std::chrono::steady_clock::now();
	auto average_time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
	average_time /= iterations;
	std::cout << average_time / 1e6 << " s" << '\n';
}
