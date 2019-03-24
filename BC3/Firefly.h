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
	double alpha;

	double LAC;
	int m;
	double atractiveness;
	int numberOfClusters = 0;
	int numberOfObjects = 0;
	int numberOfCoordinates = 0;
	double minChange;
	double Jm;

	double* minCoordinate;
	double* maxCoordinate;

	void init(int parm, double parLAC, double paratractivenes, int parnumberOfClusters, double parminchange, const Dataset* pdata = nullptr);

	void centersInit();

	void muInit();

	void dInit();

	void setMatrix(double** source, double **dest);
	void printMatrix(double** matrix, const char* text) const;

	double getMinCoordinate(int which) const;
	double getMaxCoordinate(int which) const;
	void setMinMaxCoordinates();

	double computeDistanceOfCenters(Object first, Object second) const;
	double computeDistanceOfAllCenters(vector<Object> otherCenters) const;

public:
	Firefly();
	Firefly(int parm, double parLAC, double paratractivenes, int parnumberOfClusters, double parminchange, const Dataset* pdata);
	~Firefly();
	void JmPrint() const;
	void setMu(double ** newMu);
	double getJm() const;
	double setJm();

	void muPrint() const;
	void normalizeMu();
	void computeMu();

	void centersPrint() const;
	
	void computeD();
	void dPrint() const;
	
	vector<Object> getCenters();
	void move(vector<Object> otherCenters);

	void setCenters(vector<Object> newCenters);
};

