#include "ParticleCounterData.h"
#include <iostream>

using namespace std;

void ParticleCounterData::Vprint() const
{
	printAsMatrix(V, "V");
}

void ParticleCounterData::pbestPrint() const
{
	printAsMatrix(pbest, "PBest");
	pbestCentersPrint();
}

void ParticleCounterData::pbestJmPrint() const
{
	cout << nameAlg << " - " << "Pbest Jm " << name << ":" << K/pbestFitness << endl;
}

void ParticleCounterData::Xprint() const
{
	muPrint();
}

bool ParticleCounterData::setX()
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

string ParticleCounterData::getName() const
{
	return name;
}

string ParticleCounterData::getAlgorithmName() const
{
	return nameAlg;
}

void ParticleCounterData::pbestCentersPrint() const
{
	cout << "Pbest centra:" << endl;
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

double ParticleCounterData::getFitness() const
{
	return CounterData::getFitness();
}

void ParticleCounterData::setK(int parK)
{
	if (parK > 0) {
		pbestFitness = pbestFitness * parK / K;
		CounterData::setK(parK);
	}
}

void ParticleCounterData::setM(int parm)
{
	if (parm > 0) {
		CounterData::setM(parm);
		Particle::init();
	}
}

void ParticleCounterData::setNumberOfClusters(int number)
{
	if (numberOfClusters > 0) {
		CounterData::setNumberOfClusters(number);
		setSize(data.getSize()*numberOfObjects);
	}
}

void ParticleCounterData::setData(Dataset pardata)
{
	CounterData::setData(pardata);
	setSize(data.getSize()*numberOfObjects);
}

//nie nula!!!
void ParticleCounterData::normalize()
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
					mu[i][j] = rand() / RAND_MAX;
					count++;
				}
			}
		}
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = mu[i][j] / rowSum;
		}
	}
	recalculateFromMu();
}

