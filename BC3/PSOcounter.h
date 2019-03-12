#pragma once
#include "Object.h"
#include "Dataset.h"

class PSOcounter
{
private:
	int m;
	int K;
	double c1;
	double c2;
	double r1;
	double r2;
	double w;
	int numberOfObjects;
	int numberOfClusters;
	int numberOfCoordinates;
	int maxIterationNumber;
	double minChange;
	int P;

	const Dataset* data;

	Particle* X;
	double*** pbest;
	double* fitnessPbest;
	double** gbest;
	double fitnessGbest;
	double*** d;
	Object*** centers;
	Object*** oldCenters;

	void init(const Dataset* pdata = nullptr);

	void dInit();
	void dPrint() const;
	void computeD();

	void XInit();
	void computeX();
	void normalizeX();	

	void VInit();
	void computeV();

	void printMatrix(double** matrix, const char* text) const;
	
	void centersInit();
	void centersPrint() const;
	void computeCenters();	

	void fitnessInit();
	void checkFitness();
	double getOnesFitness(const double** current) const;

	void pbestInit();
	
	void gbestInit();
	void gbestPrint() const;
	void fitnessGbestPrint() const;


public:
	PSOcounter();
	~PSOcounter();
	PSOcounter(const Dataset &pdata);

	void count(const Dataset* pdata = nullptr);
};

