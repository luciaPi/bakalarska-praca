#pragma once
#include "Object.h"
#include "Dataset.h"

class Particle
{
private:
	int m;
	int K;
	double c1;
	double c2;
	double r1;
	double r2;
	double w;
	double minChange;
	double** X;
	double** V;
	double** best;
	double fitnessBest;
	double** d;
	Object** centers;
	Object** oldCenters;

	int numberOfObjects = 0;
	int numberOfClusters = 0;
	int numberOfCoordinates = 0;

	const Dataset* data;

	void init(int parm, int parK, double parminchange, double parc1, double parc2, double parr1, double parr2, double parw,int numberOfClusters,const Dataset* pdata);
	void printMatrix(double** matrix, const char* text) const;

	void dInit();
	void XInit();
	void bestInit();
	void VInit();
	void centersInit();

	double getFitness() const;
	
	double getJm() const;
public:
	Particle();
	Particle(const Dataset &pdata,int parm, int parK, double parminchange, double parc1,double parc2, double parr1, double parr2, double parw, int numberOfClusters);
	~Particle();

	void dPrint() const;
	void computeD();

	void Xprint();
	void computeX();
	void normalizeX();

	void fitnessPrint() const;

	void checkFitness(double ** gbest, double &gbestFitness);

	void Vprint() const;
	void computeV(double** gbest);

	void centersPrint() const;
	void computeCenters();

	void bestPrint() const;

	void setMatrix(double ** source, double **dest);
	void setX(double** newX);
	double getXValue(int i, int j) const;


	void setV(double par[]);
};

