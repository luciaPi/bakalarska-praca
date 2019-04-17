#pragma once
#include "FuzzyData.h"
#include "Particle.h"

class ParticleFuzzyData : public FuzzyData, public Particle
{
protected:
	void normalize() override;
	bool setX() override;	

public:
	ParticleFuzzyData() {};
	ParticleFuzzyData(int numberOfClusters) : FuzzyData(numberOfClusters) {};
	ParticleFuzzyData(Dataset data) : FuzzyData(data) {};
	ParticleFuzzyData(Dataset data, int numberOfClusters, int m, int K, RandomGenerator* mugenerator, RandomGenerator* centersGenerator, MuInitializationMode muInitMode) : FuzzyData(data, numberOfClusters, m,K,mugenerator,centersGenerator,muInitMode) {};
	ParticleFuzzyData(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w,int K, RandomGenerator* mugenerator, RandomGenerator* centersGenerator, RandomGenerator* Vgenerator, MuInitializationMode muInitMode) :
		FuzzyData(data, numberOfClusters, m,K,mugenerator, centersGenerator, muInitMode),
		Particle(c1,c2,r1,r2,w,data.getSize()*numberOfClusters, Vgenerator)
	{
		setStartingX();
	};
	~ParticleFuzzyData() {};

	void Vprint() const;
	void pbestPrint() const;
	void pbestJmPrint() const;
	void Xprint() const;	
	void pbestCentersPrint() const;
		
	void setK(int K);
	void setM(int m);
	void setNumberOfClusters(int number);
	void setData(Dataset pardata);
	   
	string getName() const override;
	string getAlgorithmName() const override;
	double getFitness() const override;
};

