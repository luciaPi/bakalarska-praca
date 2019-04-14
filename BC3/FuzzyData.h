#pragma once
#include "Object.h"
#include "Dataset.h"
#include <string>

class FuzzyData
{
private:
	double minChange = 0.0001;
	
	Object** oldCenters = nullptr;
	double** d = nullptr;

	void init();
	void clear();

	void muInit();
	void centersInit();
	void dInit();

	void computeCenters();
	void computeMu();
	void computeD();
	void normalizeMu();
	
protected:
	int m = 1;
	int K = 1;
	Dataset data;
	string name = "Data";
	string nameAlg = "Algorithm";

	int numberOfObjects = 0;
	int numberOfCoordinates = 0;
	int numberOfClusters = 0;

	double** mu = nullptr;
	Object** centers = nullptr;

	void printAsMatrix(double* matrix, const char* text = "") const;
	void setMatrix(double ** source, double **dest);

	void recalculateFromMu();
	void recalculateFromCenters();
	
public:
	FuzzyData();
	FuzzyData(int numberOfClusters);
	FuzzyData(Dataset data);
	FuzzyData(Dataset data, int numberOfClusters, int m);
	~FuzzyData();

	void setNumberOfClusters(int number);
	void setData(Dataset data);
	void setM(int m);
	void setK(int K);
	void setMinChange(double minChange);
	void setMu(const double ** mu);
	void setName(string name);
	void setAlgorithmName(string name);

	void computeFromMu();
	//void computeFromCenters();

	void muPrint() const;
	void centersPrint() const;
	void dPrint() const;

	double getJm() const ;
	void printJm() const;

	double getFitness() const;
	void printFitness() const;

	Dataset getData() const;
	int getM() const;
	int getNumberOfClusters() const;
	const double** getMu() const;

	bool wasSignificantChange() const;
};

