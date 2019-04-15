#pragma once
#include "FCMcounter.h"
#include "FAcounter.h"

class FAFCMcounter : public FCMcounter, public FAcounter, public Counter
{
private:
	void count();

public:
	FAFCMcounter() {};
	~FAFCMcounter() {};

	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);
		
	void recount();

	void setAlgorithmName(string name);

	bool wasSignificantChange() const override;

	double getJm() const override;

	void printJm() const override;

};

