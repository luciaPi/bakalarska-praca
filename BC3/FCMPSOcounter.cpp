#include "FCMPSOcounter.h"
#include "PSOcounter.h"
#include <iostream>

using namespace std;

void FCMPSOcounter::clear()
{
	delete hybridGbest;
}

FCMPSOcounter::~FCMPSOcounter()
{
	clear();
}

void FCMPSOcounter::setCounter(Dataset data, int numberOfClusters, int m, double c1, double c2, double r1, double r2, double w, int P, int K, MuInitializationMode muInitMode)
{
	clear();
	PSOcounter::setFinalCriterion(FinalCriterion::maxIteration);
	PSOcounter::setMaxIterations(100);
	
	FCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FCMcounter::setMaxIterations(100);

	hybridGbest = new FuzzyData(data, numberOfClusters, m,K, &(FCMcounter::muGenerator), &(FCMcounter::centersGenerator),muInitMode);
	hybridGbest->setAlgorithmName(Counter::nameAlg);
	hybridGbest->setName("Best");

	PSOcounter::setCounter(data, numberOfClusters, m, c1, c2, r1, r2, w, P,K,muInitMode);
}

void FCMPSOcounter::recount()
{
	if (hybridGbest != nullptr) {
		count();
	}
}

void FCMPSOcounter::count()
{
	PSOcounter::count();
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
			//particles[l]->printJm();

			//PSOcounter::printJm();
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
		/*particles[l]->printJm();

		PSOcounter::printJm();
		printJm();*/
	}
	/*PSOcounter::gbestPrint();
	PSOcounter::printJm();
	PSOcounter::pbestsPrint();
	PSOcounter::particlesPbestJmPrint();*/
	
	/*hybridGbest->printJm();
	hybridGbest->centersPrint();*/
}

void FCMPSOcounter::setAlgorithmName(string name)
{
	PSOcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
	Counter::setAlgorithmName(name);
}

void FCMPSOcounter::setNumberOfIterations(int number)
{
	iterationNumber = number;
}

FuzzyData * FCMPSOcounter::getBest() const
{
	return hybridGbest;
}

bool FCMPSOcounter::wasSignificantChange() const
{
	return FCMcounter::wasSignificantChange() && PSOcounter::wasSignificantChange();
}

double FCMPSOcounter::getJm() const
{
	return hybridGbest->getJm();
}

void FCMPSOcounter::printJm() const
{
	return hybridGbest->printJm();
}
