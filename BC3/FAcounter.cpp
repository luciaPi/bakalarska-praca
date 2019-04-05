#include "FAcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

FAcounter::FAcounter()
{
}

FAcounter::FAcounter(const Dataset& pdata)
{
	init(&pdata);
}

FAcounter::~FAcounter()
{
	removeFireflies();
	
}

void FAcounter::init(const Dataset* pdata)
{
	srand(time(NULL));

	m = 2;
	atractiveness = 1;
	LAC = 1;

	maxIterationNumber = 100;
	if (pdata) {
		data = pdata;
	}
	minChange = 0.1;
	P = 10;
	numberOfClusters = 3;
	fireflies = nullptr;
	best = nullptr;
}

void FAcounter::count(const Dataset * pdata)
{
	if (pdata) {
		init(pdata);
	}
	if (data) {
		if (data->getSize() > 0) {
			int numberOfIterations = 0;

			removeFireflies();
			firefliesInit();
			centersPrint();
			computeD();
			//dPrint();
			computeMu();
			//muPrint();
			setJm();
			//firefliesJmPrint();	
			rankFireflies();
			printJmBest();
						
			int i = 1;
			do {
				//cout << "Round" << i++ << endl;	
								
				//rankFireflies();
				//centersPrint();
				/*bestPrint();
				firefliesJmPrint();*/
				//printJmBest();

				for (int first = 0; first < P; first++) {
					double firstJm = fireflies[first]->getJm();
					for (int second = 0; second < P; second++) {
						double secondJm = fireflies[second]->getJm();
						if (secondJm < firstJm) {
							fireflies[first]->move(fireflies[second]->getCenters(), fireflies[second]->getJm());

							//centersPrint();
							fireflies[first]->computeD();
							//dPrint();
							fireflies[first]->computeMu();
							//muPrint();
							//fireflies[first]->setJm();
							//firefliesJmPrint();
						}
					}
				}
				/*computeD();
				computeMu();*/
				setJm();

				rankFireflies();
				//centersPrint();
				/*bestPrint();*/
				//firefliesJmPrint();
				//printJmBest();

			} while (numberOfIterations++ < maxIterationNumber);
			
			centersPrint();
			firefliesJmPrint();
			printJmBest();
			//bestPrint();
		}
	}
}

void FAcounter::bestPrint() const
{
	best->muPrint();
}

void FAcounter::firefliesInit()
{
	fireflies = new Firefly*[P];
	for (int l = 0; l < P; l++) {
		fireflies[l] = new Firefly(m, LAC, atractiveness, numberOfClusters,minChange,data);
	}
	best = new Firefly(m, LAC, atractiveness, numberOfClusters, minChange, data);
}

void FAcounter::firefliesJmPrint() const
{
	for (int l = 0; l < P; l++) {
		//cout << "Jm " << l + 1 << ": " << endl;
		fireflies[l]->JmPrint();
	}
	best->JmPrint();
}

void FAcounter::rankFireflies()
{
	for (int l = 0; l < P; l++) {
		for (int ll = l+1; ll < P; ll++) {
			if (fireflies[l]->getJm() > fireflies[ll]->getJm()) {
				Firefly* f = fireflies[l];
				fireflies[l] = fireflies[ll];
				fireflies[ll] = f;
			}
		}
	}
	best->setCenters(fireflies[0]->getCenters());
}

void FAcounter::computeMu()
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->computeMu();
	}
}

void FAcounter::normalizeMu()
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->normalizeMu();
	}
}

void FAcounter::muPrint() const
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->muPrint();
	}
}

void FAcounter::centersPrint() const
{
	cout << "Centra:" << endl;
	for (int l = 0; l < P; l++) {
		cout << "centra " << l + 1 << ": " << endl;
		fireflies[l]->centersPrint();
	}
}

//vypocet matice euklidovskej vzdialenosti
void FAcounter::computeD()
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->computeD();
	}
}

//vypis matice euklidovskej vzdialenosti
void FAcounter::dPrint() const
{
	cout << "Matica euklidovskej vzdialenosti:" << endl;
	for (int l = 0; l < P; l++) {
		cout << "Matica " << l + 1 << ": " << endl;
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
	if (best != nullptr) {
		delete best;
		best = nullptr;
	}
}

void FAcounter::printJmBest() const
{
	cout << "Jm FA = ";
	best->JmPrint();
}

void FAcounter::setJm()
{
	for (int l = 0; l < P; l++) {
		fireflies[l]->setJm();
	}
}
