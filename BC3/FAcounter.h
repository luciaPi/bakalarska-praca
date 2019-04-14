#pragma once
#include "Dataset.h"
#include "FireflyConterData.h"
#include "MainCounter.h"

class FAcounter : public MainCounter
{
private:
	int P;

	FireflyCounterData** fireflies = nullptr;
	
	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);
	void removeFireflies();
	void firefliesInit(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma);

	void compute();

	void muPrint() const;
	void XPrint() const;
	void dPrint() const;
	void firefliesJmPrint() const;
	void gbestPrint() const;

	void rankFireflies();
	bool wasSignificantChange() const override;

public:
	FAcounter() {};
	~FAcounter();

	FireflyCounterData* gbest = nullptr;

	void count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	const FireflyCounterData* getBest() const;

	double getJm() const override;
	void printJm() const override;
};

