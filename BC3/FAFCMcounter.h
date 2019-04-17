#pragma once
#include "FCMcounter.h"
#include "FAcounter.h"

class FAFCMcounter : public FCMcounter, public FAcounter, public Counter
{
private:
	void count();

public:
	FAFCMcounter() {};
	FAFCMcounter(int seed1, int seed2, int seed3, int seed4) :
		FAcounter(seed2, seed3, seed4),
		FCMcounter(seed1) {};
	~FAFCMcounter() {};

	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode);
		
	void recount();

	void setAlgorithmName(string name);

	bool wasSignificantChange() const override;

	double getJm() const override;

	void printJm() const override;

};

