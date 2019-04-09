#pragma once
#include "CounterData.h"
#include "Particle.h"

class ParticleCounterData : public CounterData, Particle
{
private:
	void normalize() override;
	bool setX() override;
	

public:
	ParticleCounterData() {};
	ParticleCounterData(int numberOfClusters) : CounterData(numberOfClusters) {};
	ParticleCounterData(Dataset data) : CounterData(data) {};
	ParticleCounterData(Dataset data, int numberOfClusters, int m) : CounterData(data, numberOfClusters, m) {};
	ParticleCounterData(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w) :
		CounterData(pdata, numberOfClusters, m),
		Particle(c1,c2,r1,r2,w,pdata.getSize()*numberOfClusters)
	{};
	~ParticleCounterData() {};

	void Vprint() const;
	void pbestPrint() const;
	void Xprint() const;	

	void pbestCentersPrint() const;
};

