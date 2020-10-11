#include "Dot.h"

Dot::Dot(double _x, double _y) : x(_x), y(_y) {};

double Dot::getCordX() {
	return x;
}

double Dot::getCordY() {
	return y;
}

double getL2(Dot d1, Dot d2) {
	return (d1.getCordX() - d2.getCordX()) * (d1.getCordX() - d2.getCordX()) +
		(d1.getCordY() - d2.getCordY()) * (d1.getCordY() - d2.getCordY());
}