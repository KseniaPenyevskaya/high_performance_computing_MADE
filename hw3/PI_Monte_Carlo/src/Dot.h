#pragma once

class Dot {
public:
	Dot(double x, double y);
	double getCordX();
	double getCordY();
private:
	double x;
	double y;
};
double getL2(Dot d1, Dot d2);