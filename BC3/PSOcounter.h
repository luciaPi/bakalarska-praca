#pragma once
#include "Object.h"

class PSOcounter
{
private:
	double c1;
	double c2;
	double w;
	int numberOfObjects;
	int maxIterationNumber;

	double** mu;
	Object** centers;

	void init();

public:
	PSOcounter();
	~PSOcounter();
};

