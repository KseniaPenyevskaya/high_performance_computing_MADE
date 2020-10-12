#include"EquationSystem.h"


int main() {
	size_t dim = 300;
	EquationSystem ls(dim);
	ls.initialize();
	ls.solve();
	return 0;
}
