#pragma once
#include "Dataset.h"
#include "FireflyFuzzyData.h"
#include "Counter.h"
#include "RandomGenerator.h"

class FAcounter : public Counter
{
private:
	RandomGenerator muGenerator;
	RandomGenerator randMovementGenerator;
	RandomGenerator randMovementFirstGenerator;
	RandomGenerator centersGenerator;

	FireflyFuzzyData** fireflies = nullptr;
	
	void removeFireflies();
	void firefliesInit(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int K, MuInitializationMode muInitMode);
	
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
	FAcounter();
	FAcounter(int seed1, int seed2, int seed3, int seed4);
	~FAcounter();

	FireflyFuzzyData* gbest = nullptr;

	void setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode);

	void count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode);

	const FuzzyData* getBest() const override;

	double getJm() const override;
	void printJm() const override;
	void recount() override;
};

