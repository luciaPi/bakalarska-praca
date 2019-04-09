#include "Particle.h"
#include <iostream>
#include <time.h>

using namespace std;

Particle::Particle()
{
	srand(time(NULL));

	initParticle();
}

Particle::Particle(int pnumberOfClusters)
	: CounterData(pnumberOfClusters)
{
	srand(time(NULL));

	initParticle();
}

Particle::Particle(Dataset pdata)
	: CounterData(pdata)
{
	srand(time(NULL));

	initParticle();
}

Particle::Particle(Dataset pdata, int pnumberOfClusters, int pm)
	: CounterData(pdata,pnumberOfClusters,pm)
{
	srand(time(NULL));

	initParticle();
}

Particle::Particle(Dataset pdata, int parnumberOfClusters, int parm, double parc1, double parc2, double parr1, double parr2, double parw)
	: CounterData(pdata,parnumberOfClusters, parm)
{
	srand(time(NULL));

	if (parw >= 0) {
		w = parw;
	}
	if (parc1 >= 0) {
		c1 = parc1;
	}
	if (parc2 >= 0) {
		c2= parc2;
	}
	if (parr1 <= 1) {
		r1 = parr1;
	}
	if (parr2 <= 1) {
		r2 = parr2;
	}

	initParticle();
}

Particle::~Particle()
{
	clearParticle();
}

void Particle::initParticle()
{
	clearParticle();

	VInit();
	pbestInit();

	if (r1 < 0) {
		r1 = (double)rand() / RAND_MAX;
	}
	if (r2 < 0) {
		r2 = (double)rand() / RAND_MAX;
	}	
}

void Particle::pbestInit()
{
	pbest = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		pbest[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			pbest[i][j] = mu[i][j];
		}
	}
	pbestFitness = getJm();
}

void Particle::VInit()
{
	V = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		V[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = (double)rand() / RAND_MAX * 2.0 - 1.0;
		}
	}
}

void Particle::checkFitness()
{
	double newFitness = getFitness();
	if (newFitness > pbestFitness) {
		pbestFitness = newFitness;
		setMatrix(mu, pbest);
		
		if (newFitness > *gbestFitness) {
			*gbestFitness = newFitness;
			setMatrix(mu, gbest);
		}
	}
}

void Particle::computeV()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = w * V[i][j] + c1 * r1*(pbest[i][j] - mu[i][j]) + c2 * r2*(gbest[i][j] - mu[i][j]);
			if (V[i][j] > maxV) {
				V[i][j] = maxV;
			}
			else if (V[i][j] < -maxV) {
				V[i][j] = -maxV;
			}
		}
	}
}

void Particle::computeMu()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = mu[i][j] + V[i][j];
		}
	}
	normalizeMu();
	recalculate();
	checkFitness();
}

void Particle::setc1c2(double parc1, double parc2)
{

	if (parc1 >= 0) {
		c1 = parc1;
	}
	if (parc2 >= 0) {
		c2 = parc2;
	}
}

void Particle::setr1r2(double parr1, double parr2)
{

	if (parr1 <= 1 && parr1 >= 0) {
		r1 = parr1;
	}
	if (parr2 <= 1 && parr2 >= 0) {
		r2 = parr2;
	}
}

void Particle::setw(double parw)
{
	if (parw >= 0) {
		w = parw;
	}
}

void Particle::Vprint() const
{
	printMatrix(V, "V: ");
}

void Particle::pbestPrint() const
{
	printMatrix(pbest, "PBest: ");
}

//nie nula!!!
void Particle::normalizeMu()
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
}

void Particle::clearParticle()
{
	if (V) {
		for (int i = 0; i < numberOfObjects; i++) {
			delete[] V[i];
			V[i] = nullptr;
		}

		delete[] V;
		V = nullptr;
	}

	if (pbest) {
		for (int i = 0; i < numberOfObjects; i++) {
			delete[] pbest[i];
			pbest[i] = nullptr;
		}
		delete[] pbest;
		pbest = nullptr;
		pbestFitness = 0;
	}
}

/*
double Particle::getXValue(int i, int j) const
{
	return X[i][j];
}*/
/*
void Particle::setV(double par[])
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			int x = i * numberOfClusters + j;
			V[i][j] = par[i*numberOfClusters + j];
		}
	}
}*/