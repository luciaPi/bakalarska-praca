#include "FAFCMcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void FAFCMcounter::setCounter(Dataset pdata, int numberOfClusters, int m, double alpha, double beta, double gamma, int parP, int K, MuInitializationMode muInitMode)
{
	removeFireflies();
	P = parP;

	firefliesInit(pdata, numberOfClusters, m, alpha, beta, gamma, K, muInitMode);
}

FAFCMcounter::FAFCMcounter()
{
	muGenerator = RandomGenerator(rand(), 0, 1);
	randMovementGenerator = RandomGenerator(rand(), 0, 1);
	randMovementFirstGenerator = RandomGenerator(rand(), 0, INT_MAX);
	centersGenerator = RandomGenerator(rand(), 0, INT_MAX);
}

FAFCMcounter::FAFCMcounter(int seed1, int seed2, int seed3, int seed4)
{
	muGenerator = RandomGenerator(seed1, 0, 1);	
	randMovementGenerator = RandomGenerator(seed2, 0, 1);
	randMovementFirstGenerator = RandomGenerator(seed3, 0, INT_MAX);
	centersGenerator = RandomGenerator(seed4, 0, INT_MAX);
}

FAFCMcounter::~FAFCMcounter()
{
	removeFireflies();	
}

void FAFCMcounter::count(Dataset pdata, int numberOfClusters, int m, double alpha, double beta, double gamma, int parP, int K, MuInitializationMode muInitMode)
{
	if (pdata.getSize() > 0) {
		setCounter(pdata, numberOfClusters, m, alpha, beta, gamma, parP, K, muInitMode);
		
		rankFireflies();

		int i = 1;
		do {
			compute();
			rankFireflies();
		} while (!isMetFinalCriterion(i++));
	}
}

bool FAFCMcounter::wasSignificantChange() const
{
	for (int l = 0; l < P; l++) {
		if (fireflies[l]->wasSignificantChange(minChange)) {
			return true;
		}
	}
	return false;
}

FuzzyData * FAFCMcounter::getBest() const
{
	return gbest;
}

double FAFCMcounter::getJm() const
{
	return gbest->getJm();
}

void FAFCMcounter::printJm() const
{
	gbest->printJm();
}

void FAFCMcounter::recount()
{
	if (fireflies != nullptr) {
		count(fireflies[0]->getData(), fireflies[0]->getNumberOfClusters(), fireflies[0]->getM(), fireflies[0]->getAlpha(), fireflies[0]->getBeta(), fireflies[0]->getGamma(), P, fireflies[0]->getK(), fireflies[0]->getMuInitMode());
	}
}

void FAFCMcounter::firefliesInit(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int K, MuInitializationMode muInitMode)
{
	fireflies = new FireflyFuzzyData*[P];
	for (int l = 0; l < P; l++) {
		fireflies[l] = new FireflyFuzzyData(data, numberOfClusters, m, alpha, beta, gamma, K,&muGenerator, &centersGenerator, &randMovementGenerator, &randMovementFirstGenerator, muInitMode);
		char name[8];
		snprintf(name, sizeof(name), "FA%d", (l + 1));
		fireflies[l]->setName(name);
		fireflies[l]->setAlgorithmName(nameAlg);
	}
	gbest = new FireflyFuzzyData(data, numberOfClusters, m, alpha, beta, gamma,K, &muGenerator,&centersGenerator, &randMovementGenerator, &randMovementFirstGenerator, muInitMode);
	gbest->setAlgorithmName(nameAlg);
	gbest->setName("FA BEST");
}

void FAFCMcounter::firefliesJmPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->printJm();
	}
}

void FAFCMcounter::gbestPrint() const
{
	cout << gbest->getAlgorithmName() << " - Gbest " << gbest->getName() << ":" << endl;
	gbest->Xprint();	
}

void FAFCMcounter::rankFireflies()
{
	for (int l = 0; l < P; l++) {
		for (int ll = l+1; ll < P; ll++) {
			if (fireflies[l]->getFireflyFitness() < fireflies[ll]->getFireflyFitness()) {
				FireflyFuzzyData* f = fireflies[l];
				fireflies[l] = fireflies[ll];
				fireflies[ll] = f;
			}
		}
	}
	if (gbest->getFireflyFitness() < fireflies[0]->getFireflyFitness()) {
		gbest->setX(*(fireflies[0]));
	}
}

void FAFCMcounter::XPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->Xprint();
	}
}

void FAFCMcounter::compute()
{
	for (int first = 0; first < P; first++) {
		for (int second = 0; second < P; second++) {
			if (fireflies[first]->getFireflyFitness() < fireflies[second]->getFireflyFitness()) {
				fireflies[first]->move(fireflies[second]);
			}
		}
	}
	fireflies[0]->moveRandomly();
}

void FAFCMcounter::muPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->muPrint();
	}
}

//vypis matice euklidovskej vzdialenosti
void FAFCMcounter::dPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->dPrint();
	}
}


void FAFCMcounter::removeFireflies()
{
	if (fireflies!= nullptr) {
		for (int l = 0; l < P; l++) {
			delete fireflies[l];
		}
		delete[] fireflies;
		fireflies = nullptr;
	}
	delete gbest;
	gbest = nullptr;
}


