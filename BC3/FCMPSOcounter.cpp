#include "FCMPSOcounter.h"
#include "FCMcounter.h"
#include "PSOcounter.h"
#include <iostream>

using namespace std;

void FCMPSOcounter::count(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int P)
{
	PSOcounter::setFinalCriterion(FinalCriterion::maxIteration);
	PSOcounter::setMaxIterations(100);

	FCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FCMcounter::setMaxIterations(100);

	FuzzyData* hybridGbest = new FuzzyData(data, numberOfClusters, m);

	PSOcounter::count(data, numberOfClusters, m, c1, c2, r1, r2, w, P);
	hybridGbest->setMu(gbest->getMu());
	int i = 0;
	while (i++ < (iterationNumber - 1)) {
		for (int l = 0; l < P; l++) {
			FCMcounter::count(particles[l]);
			particles[l]->reinit();
			if (particles[l]->getFitness() > gbest->getFitness()) {
				gbest = particles[l];
			}
			if (hybridGbest->getFitness() < particles[l]->getFitness()) {
				hybridGbest->setMu(particles[l]->getMu());
			}
			particles[l]->muPrint();
		}
		PSOcounter::count();
		if (hybridGbest->getFitness() < gbest->getFitness()) {
			hybridGbest->setMu(gbest->getMu());
		}
		/*
		PSOcounter::gbestPrint();
		PSOcounter::printJm();
		PSOcounter::pbestsPrint();
		PSOcounter::particlesPbestJmPrint();*/
	}
	for (int l = 0; l < P; l++) {
		FCMcounter::count(particles[l]);
		particles[l]->reinit();
		if (particles[l]->getFitness() > gbest->getFitness()) {
			gbest = particles[l];
		}	
		if (hybridGbest->getFitness() < particles[l]->getFitness()) {
			hybridGbest->setMu(particles[l]->getMu());
		}
	}
	/*PSOcounter::gbestPrint();
	PSOcounter::printJm();
	PSOcounter::pbestsPrint();
	PSOcounter::particlesPbestJmPrint();*/
	
	hybridGbest->printJm();
	hybridGbest->centersPrint();

	delete hybridGbest;
}

void FCMPSOcounter::setAlgorithmName(string name)
{
	PSOcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
}

void FCMPSOcounter::setNumberOfIterations(int number)
{
	iterationNumber = number;
}
