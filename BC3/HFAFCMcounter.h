#pragma once
#include "FCMcounter.h"
#include "FAFCMcounter.h"

class HFAFCMcounter : public FCMcounter, public FAFCMcounter, public Counter
{
private:
	void count();

public:
	HFAFCMcounter() {};
	HFAFCMcounter(int seed1, int seed2, int seed3, int seed4, int seed5,int seed6) :
		FAFCMcounter(seed2, seed3, seed4,seed6),
		FCMcounter(seed1,seed5) {};
	~HFAFCMcounter() {};

	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode);
		
	void recount();

	void setAlgorithmName(string name);

	bool wasSignificantChange() const override;
	double getJm() const override;
	void printJm() const override;
	FuzzyData* getBest() const override;
};

