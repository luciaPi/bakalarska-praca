#pragma once
#include "Object.h"
#include "Dataset.h"
#include "ParticleFuzzyData.h"
#include "Counter.h"

class PSOcounter : public Counter
{
private:	
	int P;
	
	void setCounter(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int P);
	void removeParticles();
	void particlesInit(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w);
	
	void compute();
				
	void setV(); //dat prec potom
	
	bool wasSignificantChange() const override;

protected:
	ParticleFuzzyData** particles = nullptr;
	ParticleFuzzyData* gbest = nullptr;

	void Xprint() const;
	void Vprint() const;
	void centersPrint() const;
	void dPrint() const;
	void pbestsPrint() const;
	void gbestPrint() const;
	void particlesJmPrint() const;
	void particlesPbestJmPrint() const;

public:
	PSOcounter() {};
	~PSOcounter();

	void count(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int P);	
	void count();

	const ParticleFuzzyData* getBest() const;
		
	double getJm() const override;
	void printJm() const override;

	void setK(int K);
};

