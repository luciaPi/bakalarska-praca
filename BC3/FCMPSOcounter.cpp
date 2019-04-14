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
	
	PSOcounter::count(data, numberOfClusters, m, c1, c2, r1, r2, w, P);

	for (int l = 0; l < P; l++) {
		FCMcounter::count(particles[l]);
		particles[l]->Particle::init();
		if (particles[l]->getFitness() > gbest->getFitness()) {
			gbest = particles[l];
		}
	}	
	cout << endl;
	cout << endl;
	cout << endl;
	PSOcounter::count();/*
	PSOcounter::gbestPrint();
	PSOcounter::printJm();
	PSOcounter::pbestsPrint();
	PSOcounter::particlesPbestJmPrint();*/
}

void FCMPSOcounter::setAlgorithmName(string name)
{
	PSOcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
}
