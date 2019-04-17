#include "ParticleFuzzyData.h"
#include <iostream>

using namespace std;

void ParticleFuzzyData::Vprint() const
{
	printAsMatrix(V, "V");
}

void ParticleFuzzyData::pbestPrint() const
{
	//printAsMatrix(pbest, "PBest");
	pbestCentersPrint();
}

void ParticleFuzzyData::pbestJmPrint() const
{
	cout << nameAlg << " - Jm " << name << ":" << K/pbestFitness << endl;
}

void ParticleFuzzyData::Xprint() const
{
	muPrint();
}

bool ParticleFuzzyData::setX()
{
	if (numberOfClusters * numberOfObjects == size) {
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				*(X + i*numberOfClusters+j) = &mu[i][j];
			}
		}
		recalculateFromMu();
		return true;
	}
	return false;
}

string ParticleFuzzyData::getName() const
{
	return name;
}

string ParticleFuzzyData::getAlgorithmName() const
{
	return nameAlg;
}

void ParticleFuzzyData::pbestCentersPrint() const
{
	cout << nameAlg << " - Pbest centra "<< name << ":" << endl;
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum " << j + 1 << ":";
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum1 = 0;
			double sum2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				double product2 = pow(*(pbest+i*numberOfClusters+j),m);				
				double product1 = product2 * data[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			cout << sum1 / sum2 << " ";
		}
		cout << endl;
	}
	cout << endl;
}

double ParticleFuzzyData::getFitness() const
{
	return FuzzyData::getFitness();
}

void ParticleFuzzyData::setK(int parK)
{
	if (parK > 0) {
		pbestFitness = pbestFitness * parK / K;
		FuzzyData::setK(parK);
	}
}

void ParticleFuzzyData::setM(int parm)
{
	if (parm > 0) {
		FuzzyData::setM(parm);
		Particle::init();
	}
}

void ParticleFuzzyData::setNumberOfClusters(int number)
{
	if (numberOfClusters > 0) {
		FuzzyData::setNumberOfClusters(number);
		setSize(data.getSize()*numberOfObjects);
	}
}

void ParticleFuzzyData::setData(Dataset pardata)
{
	FuzzyData::setData(pardata);
	setSize(data.getSize()*numberOfObjects);
}

//nie nula!!!
void ParticleFuzzyData::normalize()
{
	for (int i = 0; i < numberOfObjects; i++) {
		int count = 0;
		double rowSum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			if (mu[i][j] < 0) {
				mu[i][j] = 0.001;
				count++;
			}
			rowSum += mu[i][j];
		}
		if (count == numberOfClusters) {
			for (int j = 0; j < numberOfClusters; j++) {
				if (mu[i][j] < 0) {
					mu[i][j] = muGenerator->nextRandom();
				}
			}
		}
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = mu[i][j] / rowSum;
		}
	}
	recalculateFromMu();
}

