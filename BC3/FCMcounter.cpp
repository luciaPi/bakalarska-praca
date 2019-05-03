#include "FCMcounter.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include "Attribute.h"

void FCMcounter::setCounter(Dataset parData, int parNumberOfClusters, int parM, int K, MuInitializationMode muInitMode)
{
	clear();
	fuzzyData = new FuzzyData(parData, parNumberOfClusters, parM, K, &muGenerator,&centersGenerator, muInitMode);
	fuzzyData->setName("FCM");
	fuzzyData->setAlgorithmName(nameAlg);
}

FCMcounter::FCMcounter()
{
	muGenerator = RandomGenerator(rand(), 0, 1);
	centersGenerator = RandomGenerator(rand(), 0, INT_MAX);
}

FCMcounter::FCMcounter(int seed1, int seed2)
{
	muGenerator = RandomGenerator(seed1, 0, 1);
	centersGenerator = RandomGenerator(seed2, 0, INT_MAX);
}

FCMcounter::~FCMcounter() {
	clear();
}

//kroky algoritmu
void FCMcounter::count(Dataset data, int numberOfClusters, int m, int K, MuInitializationMode muInitMode)
{	
	if (data.getSize() > 0) {
		setCounter(data, numberOfClusters, m, K, muInitMode);		
		count();
	}	
}

void FCMcounter::count(FuzzyData * other)
{
	if (other != nullptr) {
		setCounter(other->getData(),other->getNumberOfClusters(),other->getM(), other->getK(),other->getMuInitMode());
		fuzzyData->setMu(other->getMu());
		count();
	}
	other->setMu(fuzzyData->getMu());
}

FuzzyData * FCMcounter::getBest() const
{
	return fuzzyData;
}

void FCMcounter::count()
{
	int i = 0;
	do {
		computeMu();
	} while (!isMetFinalCriterion(i++));
}

void FCMcounter::clear()
{
	delete fuzzyData;
	fuzzyData = nullptr;
}

double FCMcounter::getJm() const
{
	if (fuzzyData) {
		return fuzzyData->getJm();
	}
	return -1;
}

void FCMcounter::printJm() const
{
	if (fuzzyData) {
		fuzzyData->printJm();
	}
}

void FCMcounter::recount()
{
	if (fuzzyData != nullptr) {
		if ((fuzzyData->getData()).getSize() > 0) {
			setCounter(fuzzyData->getData(), fuzzyData->getNumberOfClusters(), fuzzyData->getM(), fuzzyData->getK(),fuzzyData->getMuInitMode());
			count();
		}
	}
}

bool FCMcounter::wasSignificantChange() const
{
	return fuzzyData->wasSignificantChange(minChange);
}

//vypis matice mu
void FCMcounter::muPrint() const
{
	if (fuzzyData) {
		fuzzyData->muPrint();
	}
}

//vypocet matice mu
void FCMcounter::computeMu()
{
	if (fuzzyData) {
		fuzzyData->computeFromMu();
	}
}

//vypis centier
void FCMcounter::centersPrint() const
{
	if (fuzzyData) {
		fuzzyData->centersPrint();
	}
}

//vypis matice euklidovskej vzdialenosti
void FCMcounter::dPrint() const
{
	if (fuzzyData) {
		fuzzyData->dPrint();
	}
}