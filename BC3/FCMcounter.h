#pragma once
#include "Object.h"
#include "Dataset.h"
#include "FuzzyData.h"
#include "Counter.h"

using namespace std;

class FCMcounter : public Counter
{
private:
	FuzzyData* fuzzyData = nullptr;
	
	void clear();

	void muPrint() const;
	void computeMu();

	void centersPrint() const;
	void dPrint() const;

	void count();

protected:
	RandomGenerator muGenerator;
	RandomGenerator centersGenerator;

	bool wasSignificantChange() const override;

public:
	FCMcounter();
	FCMcounter(int seed1, int seed2);
	~FCMcounter();

	void setCounter(Dataset data, int numberOfClusters, int m, int K, MuInitializationMode muInitMode);
	
	void count(Dataset data, int numberOfClusters, int m, int K, MuInitializationMode muInitMode);
	void count(FuzzyData* other);

	FuzzyData* getBest() const override;
	double getJm() const override;
	void printJm() const override;
	virtual void recount() override;
};

