#include "utils.h"
void seedThreads(const size_t nThreads, unsigned int* seeds) {

	int tid;
	unsigned int seed;
#pragma omp parallel private(tid, seed)
	{
		int tid = omp_get_thread_num();
		seed = (unsigned)time(NULL);
		seeds[tid] = (seed & 0xFFFFFFF0) | (tid + 1);
	}
}