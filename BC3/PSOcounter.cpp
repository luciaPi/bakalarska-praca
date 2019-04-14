#include "PSOcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void PSOcounter::setCounter(Dataset pdata, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int parP)
{
	removeParticles();
	P = parP;

	particlesInit(pdata, numberOfClusters, m, c1, c2, r1, r2, w);
	if (P > 0) {
		gbest = particles[0];
	}
}

PSOcounter::~PSOcounter()
{
	removeParticles();	
}

void PSOcounter::count(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int parP)
{
	if (data.getSize() > 0) {
		setCounter(data, numberOfClusters, m, c1, c2, r1, r2, w, parP);
		count();
	}
}

void PSOcounter::count()
{
	//setV();

		/*Vprint();
		Xprint();
		//dPrint();
		pbestsPrint();
		gbestPrint();
		centersPrint();
		printJm();
		particlesJmPrint();*/

	int i = 1;
	do {
		//cout << "Round" << i << endl;	
		compute();
		/*Vprint();
		Xprint();
		dPrint();
		pbestsPrint();
		gbestPrint();
		centersPrint();*/
		//printJm();
		//particlesPbestJmPrint();
		//particlesJmPrint();
	} while (!isMetFinalCriterion(i++));
	//Vprint();
	//Xprint();
	//dPrint();			
	/*gbestPrint();
	printJm();
	pbestsPrint();
	particlesPbestJmPrint();*/
}

void PSOcounter::particlesInit(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w)
{
	particles = new ParticleFuzzyData*[P];
	for (int l = 0; l < P; l++) {
		particles[l] = new ParticleFuzzyData(data, numberOfClusters, m, c1, c2, r1, r2, w);
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

void PSOcounter::setV()
{
	double v1[] = {0.96,-0.4,-0.75,0.34,-0.19,-0.42,-0.42,-0.89,0.33,0.83,-0.79,0.47,0.28,0.61,-0.12};
	particles[0]->setV(v1);
	double v2[] = {0.93,-0.81,-0.22,-0.69,-0.2,-0.85,-0.61,0.6,-0.1,0.03,-0.05,-0.83,-0.27,0.81,-0.03};
	particles[1]->setV(v2);
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

bool PSOcounter::wasSignificantChange() const
{
	for (int l = 0; l < P; l++) {
		if (particles[l]->wasSignificantChange()) {
			return true;
		}
	}
	return false;
}

const FuzzyData* PSOcounter::getBest() const
{
	return gbest;
}

double PSOcounter::getJm() const
{
	return gbest->getJm();
}
