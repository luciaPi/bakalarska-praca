#include "FAcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void FAcounter::setCounter(Dataset pdata, int numberOfClusters, int m, double alpha, double beta, double gamma, int parP)
{
	removeFireflies();
	P = parP;

	firefliesInit(pdata, numberOfClusters, m, alpha, beta, gamma);
}

FAcounter::~FAcounter()
{
	removeFireflies();	
}

void FAcounter::count(Dataset pdata, int numberOfClusters, int m, double alpha, double beta, double gamma, int parP)
{
	if (pdata.getSize() > 0) {
		setCounter(pdata, numberOfClusters, m, alpha, beta, gamma, parP);
		
		//fireflies[0]->printMinMax();

		rankFireflies();

		/*muPrint();
		dPrint();*/
		//firefliesJmPrint();
		/*gbestPrint();
		XPrint();
		printJm();*/

		int i = 1;
		do {
			//cout << "Round" << i << endl;
			compute();
			rankFireflies();

			//muPrint();
			//dPrint();
			//XPrint();
			//firefliesJmPrint();
			//gbestPrint();
			//printJm();
		} while (!isMetFinalCriterion(i++));
		/*firefliesJmPrint();
		gbestPrint();
		XPrint();
		muPrint();*/
	}
}

bool FAcounter::wasSignificantChange() const
{
	for (int l = 0; l < P; l++) {
		if (fireflies[l]->wasSignificantChange(minChange)) {
			return true;
		}
	}
	return false;
}

const FuzzyData * FAcounter::getBest() const
{
	return gbest;
}

double FAcounter::getJm() const
{
	return gbest->getJm();
}

void FAcounter::printJm() const
{
	gbest->printJm();
}

void FAcounter::recount()
{
	if (fireflies != nullptr) {
		count(fireflies[0]->getData(), fireflies[0]->getNumberOfClusters(), fireflies[0]->getM(), fireflies[0]->getAlpha(), fireflies[0]->getBeta(), fireflies[0]->getGamma(), P);
	}
}

void FAcounter::firefliesInit(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma)
{
	fireflies = new FireflyFuzzyData*[P];
	for (int l = 0; l < P; l++) {
		fireflies[l] = new FireflyFuzzyData(data, numberOfClusters, m, alpha, beta, gamma);
		char name[8];
		snprintf(name, sizeof(name), "FA%d", (l + 1));
		fireflies[l]->setName(name);
		fireflies[l]->setAlgorithmName(nameAlg);
	}
	gbest = new FireflyFuzzyData(data, numberOfClusters, m, alpha, beta, gamma);
	gbest->setAlgorithmName(nameAlg);
	gbest->setName("FA BEST");
}

void FAcounter::firefliesJmPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->printJm();
	}
}

void FAcounter::gbestPrint() const
{
	cout << gbest->getAlgorithmName() << " - Gbest " << gbest->getName() << ":" << endl;
	//gbest->muPrint();
	gbest->Xprint();	
}

void FAcounter::rankFireflies()
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

void FAcounter::XPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->Xprint();
	}
}

void FAcounter::compute()
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

void FAcounter::muPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->muPrint();
	}
}

//vypis matice euklidovskej vzdialenosti
void FAcounter::dPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->dPrint();
	}
}


void FAcounter::removeFireflies()
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


