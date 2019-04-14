#pragma once
#include "FCMcounter.h"
#include "FAcounter.h"

class FAFCMcounter : public FCMcounter, public FAcounter
{
public:
	FAFCMcounter() {};
	~FAFCMcounter() {};

	void count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	void setAlgorithmName(string name);
};

