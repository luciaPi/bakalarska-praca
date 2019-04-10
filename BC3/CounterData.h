#pragma once
#include "Object.h"
#include "Dataset.h"
#include <string>

class CounterData
{
private:
	double minChange = 0.0001;
	string name = "Data";
	
	Object** centers = nullptr;
	Object** oldCenters = nullptr;
	double** d = nullptr;

	void init();
	void clear();

	void muInit();
	void centersInit();
	void dInit();
	
	void computeD();
	void computeCenters();
	void normalizeMu();
	
protected:
	int m = 1;
	int K = 1;
	Dataset data;

	int numberOfObjects = 0;
	int numberOfCoordinates = 0;
	int numberOfClusters = 0;

	double** mu = nullptr;

	void printAsMatrix(double* matrix, const char* text = "") const;
	void setMatrix(double ** source, double **dest);

	void recalculate();

public:
	CounterData();
	CounterData(int numberOfClusters);
	CounterData(Dataset data);
	CounterData(Dataset data, int numberOfClusters, int m);
	~CounterData();

	void setNumberOfClusters(int number);
	void setData(Dataset data);
	void setM(int m);
	void setK(int K);
	void setMinChange(double minChange);
	void setMu(const double ** mu);
	void setName(string name);

	void muPrint() const;
	void computeMu();

	void centersPrint() const;

	void dPrint() const;

	double getJm() const ;
	void printJm() const;

	double getFitness() const;
	void printFitness() const;

	bool wasSignificantChange() const;
};

