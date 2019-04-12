#pragma once
#include "CounterData.h"
#include "Particle.h"

class ParticleCounterData : public CounterData, public Particle
{
protected:
	void normalize() override;
	bool setX() override;

public:
	ParticleCounterData() {};
	ParticleCounterData(int numberOfClusters) : CounterData(numberOfClusters) {};
	ParticleCounterData(Dataset data) : CounterData(data) {};
	ParticleCounterData(Dataset data, int numberOfClusters, int m) : CounterData(data, numberOfClusters, m) {};
	ParticleCounterData(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w) :
		CounterData(data, numberOfClusters, m),
		Particle(c1,c2,r1,r2,w,data.getSize()*numberOfClusters)
	{
		setStartingX();
	};
	~ParticleCounterData() {};

	void Vprint() const;
	void pbestPrint() const;
	void Xprint() const;	
	void pbestCentersPrint() const;

	double getFitness() const override;
	
	void setK(int K);
	void setM(int m);
	void setNumberOfClusters(int number);
	void setData(Dataset pardata);
	   
	string getName() const override;
	string getAlgorithmName() const override;
};

