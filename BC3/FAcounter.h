#pragma once
#include "Dataset.h"
#include "FireflyConterData.h"
#include "MainCounter.h"

class FAcounter : public MainCounter
{
private:
	int P;

	FireflyConterData** fireflies;
	FireflyConterData* gbest;
	
	void setCounter();
	void removeFireflies();
	void firefliesInit();

	void compute();

	void centersPrint() const;
	void XPrint() const;
	void dPrint() const;
	void firefliesJmPrint() const;
	void gbestPrint() const;

	void rankFireflies();
	bool wasSignificantChange() const override;

public:
	FAcounter();
	~FAcounter();

	void count(Dataset pdata, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	const FireflyConterData* getBest() const;

	double getJm() const override;
	void printJm() const override;
};

