#include "FCMcounter.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include "Attribute.h"

void FCMcounter::setCounter(Dataset parData, int parNumberOfClusters, int parM)
{
	clear();
	counterData = new CounterData(parData, parNumberOfClusters, parM);
	counterData->setName("FCM");
	counterData->setAlgorithmName(nameAlg);
}

FCMcounter::~FCMcounter() {
	clear();
}

//kroky algoritmu
void FCMcounter::count(const Dataset data, int numberOfClusters, int m)
{	
	if (data.getSize() > 0) {
		setCounter(data, numberOfClusters, m);		
		count();
	}	
}

void FCMcounter::count(CounterData * other)
{
	if (other != nullptr) {
		setCounter(other->getData(),other->getNumberOfClusters(),other->getM());
		counterData->setMu(other->getMu());
		count();
	}
	other->setMu(counterData->getMu());
}

void FCMcounter::count()
{
	//muPrint();
	//centersPrint();
	//dPrint();

	int i = 0;
	do {
		//cout << "Round" << i << endl;
		computeMu();

		//muPrint();
		//centersPrint();
		//dPrint();
		printJm();
	} while (!isMetFinalCriterion(i++));
	//muPrint();
	//centersPrint();
	//printJm();
}

void FCMcounter::clear()
{
	delete counterData;
	counterData = nullptr;
}

double FCMcounter::getJm() const
{
	if (counterData) {
		return counterData->getJm();
	}
	return -1;
}

void FCMcounter::printJm() const
{
	if (counterData) {
		counterData->printJm();
	}
}

bool FCMcounter::wasSignificantChange() const
{
	return counterData->wasSignificantChange();
}

//vypis matice mu
void FCMcounter::muPrint() const
{
	if (counterData) {
		counterData->muPrint();
	}
}

//vypocet matice mu
void FCMcounter::computeMu()
{
	if (counterData) {
		counterData->computeFromMu();
	}
}

//vypis centier
void FCMcounter::centersPrint() const
{
	if (counterData) {
		counterData->centersPrint();
	}
}

/*//vypocet centier
void FCMcounter::computeCenters()
{
	if (counterData) {
		counterData->computeCenters();
	}
}*/

//vypis matice euklidovskej vzdialenosti
void FCMcounter::dPrint() const
{
	if (counterData) {
		counterData->dPrint();
	}
}

/*void FCMcounter::computeD()
{
	if (counterData) {
		counterData->computeD();
	}
}*/

/*
//vypis dat spolu s priradenym zhlukom
void FCMcounter::objectsPrintWithType() const
{
	cout << "Vysledok:" << endl;
	for (int i = 0; i < data->getSize();i++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << (*data)[i].getValue(k) << (k == numberOfCoordinates - 1 ? " Zhluk: " : ",");
		}
		cout << whichCenter((*data)[i]) << endl;
	}
}*/

/*//ku ktoremu centru ma objekt najvyssiu prislusnost
int FCMcounter::whichCenter(const Object &flower) const
{
	int whichObject = whichNumberOfObject(flower);

	double max = 0;
	int maxCoordinate = -1;
	if (whichObject >= 0) {
		for (int j = 0; j < numberOfClusters; j++) {
			if (mu[whichObject][j] > max) {
				max = mu[whichObject][j];
				maxCoordinate = j;
			}
		}
	}
	return maxCoordinate;
}*/

/*//zisti poradie daneho objektu v ramci vsetkych objektov
int FCMcounter::whichNumberOfObject(const Object & flower) const
{
	return data->whichNumberOfObject(flower);
}*/