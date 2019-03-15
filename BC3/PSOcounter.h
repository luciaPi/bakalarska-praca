#pragma once
#include "Object.h"
#include "Dataset.h"
#include "Particle.h"

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
	
	int maxIterationNumber;
	double minChange;
	int P;
	int numberOfClusters;

	const Dataset* data;

	Particle** particles;
	double** gbestX;
	double gbestFitness;

	void init(const Dataset* pdata = nullptr);

		
	void particlesInit();
	void gbestInit();
	
	void computeV();
	void Vprint() const;

	void checkFitness();
	void bestsPrint() const;
	
	void gbestPrint() const;

	void fitnessPrint() const;
	
	void computeX();
	void normalizeX();
	void Xprint() const;

	void computeCenters();
	void centersPrint() const;

	void computeD();
	void dPrint() const;

	void removeParticles();

	void setV();

public:
	PSOcounter();
	~PSOcounter();
	PSOcounter(const Dataset &pdata);

	void count(const Dataset* pdata = nullptr);
	void printbestCentre() const;
	void printJm() const;

	const double** getBestX() const;
};

