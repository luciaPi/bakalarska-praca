#include "ParticleCounterData.h"
#include <iostream>

using namespace std;

void ParticleCounterData::Vprint() const
{
	printAsMatrix(V, "V: ");
}

void ParticleCounterData::pbestPrint() const
{
	printAsMatrix(pbest, "PBest: ");
	pbestCentersPrint();
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
				*(X + i*numberOfClusters+j) = *(&mu[i][j]);
			}
		}
		recalculate();
		return true;
	}
	return false;
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
				double product2 = pow(*(pbest+i*numberOfObjects+j),m);				
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
	if (K > 0) {
		CounterData::setK(K);
		pbestFitness = pbestFitness * parK / K;
	}
}

void ParticleCounterData::setM(int m)
{
	if (m > 0) {
		CounterData::setM(m);
		Particle::init();
	}
}

void ParticleCounterData::setNumberOfClusters(int number)
{
	if (numberOfClusters > 0) {
		CounterData::setNumberOfClusters(number);
		setSize(number*numberOfObjects);
	}
}

void ParticleCounterData::setData(Dataset data)
{
	CounterData::setData(data);
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
				mu[i][j] = 0;
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
	recalculate();
}

