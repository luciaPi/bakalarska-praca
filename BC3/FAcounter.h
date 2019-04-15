#pragma once
#include "Dataset.h"
#include "FireflyFuzzyData.h"
#include "Counter.h"

class FAcounter : public Counter
{
private:
	FireflyFuzzyData** fireflies = nullptr;
	
	void removeFireflies();
	void firefliesInit(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma);
	
	void muPrint() const;
	void XPrint() const;
	void dPrint() const;
	void firefliesJmPrint() const;
	void gbestPrint() const;

	void compute();
	void rankFireflies();

protected:
	int P;
	bool wasSignificantChange() const override;

public:
	FAcounter() {};
	~FAcounter();

	FireflyFuzzyData* gbest = nullptr;

	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	void count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P);

	const FuzzyData* getBest() const;

	double getJm() const override;
	void printJm() const override;
	void recount() override;
};

