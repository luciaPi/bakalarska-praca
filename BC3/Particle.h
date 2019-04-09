#pragma once
#include "Object.h"
#include "Dataset.h"
#include "CounterData.h"

class Particle : public CounterData
{
private:
	double c1 = 1;
	double c2 = 1;
	double r1 = -1;
	double r2 = -1;
	double w = 1;

	const double maxV = 1;

	double** V;
	double** pbest;
	double pbestFitness = 0;

	double** gbest;
	double* gbestFitness;

	void pbestInit();
	void VInit();
	void initParticle();

	void checkFitness();

	void normalizeMu();

	void clearParticle();

public:
	Particle();
	Particle(int numberOfClusters);
	Particle(Dataset data);
	Particle(Dataset data, int numberOfClusters, int m);
	Particle(Dataset pdata, int numberOfClusters, int m, double c1,double c2, double r1, double r2, double w);
	~Particle();

	void Vprint() const;
	void computeV();
	
	void pbestPrint() const;

	void computeMu();
	
	void setc1c2(double c1, double c2);
	void setr1r2(double r1, double r2);
	void setw(double w);
		
	//double getXValue(int i, int j) const;
	//void setV(double par[]);
};

