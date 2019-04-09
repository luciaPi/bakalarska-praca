#pragma once
#include "Object.h"
#include "Dataset.h"
#include "Particle.h"
#include "ParticleCounterData.h"
#include "MainCounter.h"

class PSOcounter : public MainCounter
{
private:	
	int P;

	ParticleCounterData** particles;
	double* gbest;
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

	void printbestCentre() const;

	bool wasSignificantChange() const override;

public:
	PSOcounter();
	~PSOcounter();

	void count(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w);
	

	//const double** getBestX() const;
		
	double getJm() const override;
	void printJm() const override;
};

