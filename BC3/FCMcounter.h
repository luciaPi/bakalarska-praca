#pragma once
#include "Object.h"
#include "Dataset.h"

using namespace std;

class FCMcounter
{
private:
	int m;
	const Dataset *data;
	
	int numberOfObjects;
	int numberOfCoordinates;
	int numberOfClusters;
	double** mu;
	Object** centers;
	Object** oldCenters;
	double** d;
	double minChange;
	
	void objectsPrintWithType() const;

	void muInit();
	void muPrint() const;
	void computeMu();

	void centersInit();
	void centersPrint() const;
	void computeCenters();	

	void dInit();
	void dPrint() const;
	void computeD();

	bool isSignificantChange() const;
	int whichCenter(const Object &object) const;
	int whichNumberOfObject(const Object &object) const;

	void init(const Dataset& pdata);

public:
	FCMcounter() {};
	FCMcounter(const Dataset &pdata);
	~FCMcounter();	
	void count(const Dataset* pdata = nullptr);
	double getFitness() const;
	void printJm() const;
};

