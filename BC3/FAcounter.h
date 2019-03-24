#pragma once
#include "Dataset.h"
#include "Firefly.h"

class FAcounter
{
private:
	const Dataset *data;
	Firefly** fireflies;
	Firefly* best;

	double LAC;
	int m;
	double atractiveness;
	int maxIterationNumber;
	double minChange;
	int P;
	int numberOfClusters;

	void init(const Dataset* pdata = nullptr);

	void firefliesInit();
	void removeFireflies();

	void centersPrint() const;

	void muPrint() const;
	void normalizeMu();
	void computeMu();

	void computeD();
	void dPrint() const;
	
	void setJm();

	void firefliesJmPrint() const;

	void rankFireflies();

public:
	FAcounter();
	FAcounter(const Dataset&pdata);
	~FAcounter();

	void count(const Dataset* pdata = nullptr);
	void bestPrint() const;

	void printJmBest() const;
};

