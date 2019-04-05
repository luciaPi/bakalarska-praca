#pragma once
#include "Object.h"
#include "Dataset.h"
#include "CounterData.h"

using namespace std;

enum class FinalCriterion { minChange, maxIteration, both };

class FCMcounter
{
private:
	CounterData* counterData = nullptr;

	double minChange = 0.0001;
	int maxIteration = 20;
	FinalCriterion finalCriterion = FinalCriterion::both;

	void setCounter(Dataset data, int numberOfClusters, int m);
	void clear();

	void muPrint() const;
	void computeMu();

	void centersPrint() const;
	//void computeCenters();

	void dPrint() const;
	//void computeD();

	bool isMetFinalCriterion(int actualIterationNumber) const;

	//void objectsPrintWithType() const;	
	//int whichCenter(const Object &object) const;
	//int whichNumberOfObject(const Object &object) const;

public:
	FCMcounter() {};
	~FCMcounter();
	
	void count(const Dataset data, int numberOfClusters, int m);
	void setMaxIterations(int parmaxIteration);
	void setMinChange(double minChange);
	void setFinalCriterion(FinalCriterion fc);

	double getJm() const;
	void printJm() const;	
};

