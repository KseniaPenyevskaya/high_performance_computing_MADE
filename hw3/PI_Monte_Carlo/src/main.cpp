#include <omp.h>
#include <random>

#include "Dot.h"
#include "utils.h"

double computePiMonteCarlo() {
	int N = 1000000;
	int R2 = 100;
	int inCircle = 0;
	int total = 0;
	int nThreads = 8;
	unsigned int seeds[nThreads];
	seedThreads(nThreads, seeds);
#pragma omp parallel reduction(+: inCircle, total)
	{
		int tid = omp_get_thread_num();
		srand(seeds[tid]);
		Dot center(0.0, 0.0);
		double x;
		double y;
		for (int i = 0; i < N; ++i) {
			x = (double)(rand()) / RAND_MAX * 10;
			y = (double)(rand()) / RAND_MAX * 10;
			total += 1;
			if (getL2(Dot(x, y), center) <= R2) {
				inCircle += 1;
			}
		}
	}
	return 4 * (double)inCircle / double(total);
}


int main(int argc, char* argv[])
{
	double res = computePiMonteCarlo();
	printf("pi = %f\n", res);
	return 0;
}
