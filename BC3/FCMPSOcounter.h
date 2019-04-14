#pragma once
#include "Dataset.h"
#include "FCMcounter.h"
#include "PSOcounter.h"

class FCMPSOcounter : public FCMcounter, public PSOcounter
{
private:
	FuzzyData* hybridGbest = nullptr;
	int iterationNumber = 5;

	void clear();

public:
	FCMPSOcounter() {};
	~FCMPSOcounter();

	void count(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int P);
	
	void setAlgorithmName(string name);
	void setNumberOfIterations(int number);

	const FuzzyData* getBest() const;
};

