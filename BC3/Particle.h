#pragma once
#include "Object.h"

class Particle
{
private:
	double** X;
	double** V;
	double** pbest;
	double fitnessPbest;
	double** d;
	Object** centers;
	Object** oldCenters;
public:
	Particle();
	~Particle();
};

