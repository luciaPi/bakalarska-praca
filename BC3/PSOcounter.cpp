#include "PSOcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void PSOcounter::setCounter(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int parP, int K, MuInitializationMode muInitMode)
{
	removeParticles();
	P = parP;

	particlesInit(pdata, numberOfClusters, m, c1, c2, r1, r2, w, K, muInitMode);
	if (P > 0) {
		gbest = particles[0];
	}
	if (maxV > 0) {
		setMaxV();
	}
}

PSOcounter::PSOcounter()
{
	Vgenerator = RandomGenerator(rand(), -1, 1);
	muGenerator = RandomGenerator(rand(), 0, 1);
	centersGenerator = RandomGenerator(rand(), 0, INT_MAX);
}

PSOcounter::PSOcounter(int seed1, int seed2, int seed3)
{
	Vgenerator = RandomGenerator(seed1,-1,1);
	muGenerator = RandomGenerator(seed2,0,1);
	centersGenerator = RandomGenerator(seed3,0,INT_MAX);
}

PSOcounter::~PSOcounter()
{
	removeParticles();	
}

void PSOcounter::count(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int parP, int K, MuInitializationMode muInitMode)
{
	if (data.getSize() > 0) {
		setCounter(data, numberOfClusters, m, c1, c2, r1, r2, w, parP, K,muInitMode);
		count();
	}
}

void PSOcounter::recount()
{
	if (gbest) {
		setCounter(gbest->getData(), gbest->getNumberOfClusters(), gbest->getM(), gbest->getC1(), gbest->getC2(), gbest->getR1(), gbest->getR2(), gbest->getW(), P,gbest->getK(),gbest->getMuInitMode());
		count();
	}
}

void PSOcounter::count()
{

	int i = 1;
	do {
		compute();
	} while (!isMetFinalCriterion(i++));
}

void PSOcounter::particlesInit(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int K, MuInitializationMode muInitMode)
{
	particles = new ParticleFuzzyData*[P];
	for (int l = 0; l < P; l++) {
		particles[l] = new ParticleFuzzyData(data, numberOfClusters, m, c1, c2, r1, r2, w,K,&muGenerator, &centersGenerator, &Vgenerator,muInitMode);
		char name[8];
		snprintf(name,sizeof(name),"PSO%d", (l + 1));
		particles[l]->setName(name);
		particles[l]->setAlgorithmName(nameAlg);
	}	
}

void PSOcounter::compute()
{
	for (int l = 0; l < P; l++) {
		bool changeGbest = particles[l]->compute(gbest);
		if (changeGbest) {
			gbest = particles[l];
		}
	}
}

void PSOcounter::Vprint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->Vprint();
	}
}

void PSOcounter::pbestsPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->pbestPrint();
	}
}

void PSOcounter::gbestPrint() const
{
	cout << gbest->getAlgorithmName() << " - Gbest " << gbest->getName() << ":" << endl;
	gbest->pbestPrint();
}

void PSOcounter::particlesJmPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->printJm();
	}
}

void PSOcounter::particlesPbestJmPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->pbestJmPrint();
	}
}

void PSOcounter::Xprint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->Xprint();
	}
}

void PSOcounter::centersPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->centersPrint();
	}
}

//vypis matice euklidovskej vzdialenosti
void PSOcounter::dPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->dPrint();
	}
}

void PSOcounter::removeParticles()
{
	if (particles != nullptr) {
		for (int l = 0; l < P; l++) {
			delete particles[l];

		}
		delete[] particles;
		particles = nullptr;
	}
	gbest = nullptr;
}

void PSOcounter::printJm() const
{
	gbest->pbestJmPrint();
}

void PSOcounter::setK(int K)
{
	for (int l = 0; l < P; l++) {
		particles[l]->setK(K);
	}
}

void PSOcounter::setMaxV(double pmaxV)
{
	maxV = pmaxV;
}

void PSOcounter::setMaxV()
{
	for (int l = 0; l < P; l++) {
		particles[l]->setMaxV(maxV);
	}
}

bool PSOcounter::wasSignificantChange() const
{
	for (int l = 0; l < P; l++) {
		if (particles[l]->wasSignificantChange(minChange)) {
			return true;
		}
	}
	return false;
}

FuzzyData* PSOcounter::getBest() const
{
	return gbest;
}

double PSOcounter::getJm() const
{
	return gbest->getJm();
}
