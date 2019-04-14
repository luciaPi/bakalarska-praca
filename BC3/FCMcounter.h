#pragma once
#include "Object.h"
#include "Dataset.h"
#include "CounterData.h"
#include "MainCounter.h"

using namespace std;

class FCMcounter : public MainCounter
{
private:
	CounterData* counterData = nullptr;
	
	void setCounter(Dataset data, int numberOfClusters, int m);
	void clear();

	void muPrint() const;
	void computeMu();

	void centersPrint() const;
	void dPrint() const;

	bool wasSignificantChange() const override;
	void count();

	//void computeD();	
	//void computeCenters();
	//void objectsPrintWithType() const;	
	//int whichCenter(const Object &object) const;
	//int whichNumberOfObject(const Object &object) const;

public:
	FCMcounter() {};
	~FCMcounter();
	
	void count(const Dataset data, int numberOfClusters, int m);
	void count(CounterData* other);

	double getJm() const override;
	void printJm() const override;
};

