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
	ParticleCounterData* gbest;

	void setCounter(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w);
	void removeParticles();
	void particlesInit();	
	
	void compute();
	
	void Xprint() const;
	void Vprint() const;
	void centersPrint() const;
	void dPrint() const;
	void pbestsPrint() const;
	void gbestPrint() const;
			
	void setV(); //dat prec potom
	
	bool wasSignificantChange() const override;

public:
	PSOcounter();
	~PSOcounter();

	void count(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w);	

	const ParticleCounterData* getBest() const;
		
	double getJm() const override;
	void printJm() const override;
};

