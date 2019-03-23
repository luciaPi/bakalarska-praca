#pragma once
#include "Object.h"
#include "Dataset.h"

class Firefly
{
private:
	const Dataset* data;
	Object** centers;
	Object** oldCenters;
	double** d;
	double** mu;

	double LAC;
	int m;
	double atractiveness;
	int numberOfClusters = 0;
	int numberOfObjects = 0;
	int numberOfCoordinates = 0;
	double minChange;


	void init(int parm, double parLAC, double paratractivenes, int parnumberOfClusters, double parminchange, const Dataset* pdata = nullptr);

	void centersInit();
	void computeCenters();
	void centersPrint() const;

	void muInit();
	void muPrint() const;
	void normalizeMu();

	void dInit();
	void computeD();
	void dPrint() const;

	void setMatrix(double** source, double **dest);
	void printMatrix(double** matrix, const char* text) const;

public:
	Firefly();
	~Firefly();
	void JmPrint() const;
	void setMu(double ** newMu);
	double getJm() const;
};

