#pragma once
#include "FCMcounter.h"
#include "FAcounter.h"

class FAFCM : public FCMcounter, public FAcounter
{
public:
	FAFCM() {};
	~FAFCM() {};

	void count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	void setAlgorithmName(string name);
};

