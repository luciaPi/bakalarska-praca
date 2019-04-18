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

	//void computeD();	
	//void computeCenters();
	//void objectsPrintWithType() const;	
	//int whichCenter(const Object &object) const;
	//int whichNumberOfObject(const Object &object) const;

public:
	FCMcounter();
	FCMcounter(int seed1, int seed2);
	~FCMcounter();

	void setCounter(Dataset data, int numberOfClusters, int m, int K, MuInitializationMode muInitMode);
	
	void count(const Dataset data, int numberOfClusters, int m, int K, MuInitializationMode muInitMode);
	void count(FuzzyData* other);

	const FuzzyData* getBest() const override;
	double getJm() const override;
	void printJm() const override;
	virtual void recount() override;
};

