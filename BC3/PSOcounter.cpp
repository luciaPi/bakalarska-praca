#include "PSOcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void PSOcounter::init(const Dataset& pdata)
{
	srand(time(NULL));

	K = 2;
	m = 2;
	c1 = 2;
	c2 = 2;
	r1 = (double) rand() / RAND_MAX;
	r2 = (double) rand() / RAND_MAX;
	w = 0.5;
	numberOfObjects = 0;
	numberOfClusters = 3;
	maxIterationNumber = 1;
	data = &pdata;
	minChange = 0.1;

	
}

PSOcounter::PSOcounter()
{
}

PSOcounter::~PSOcounter()
{
	for (int i = 0; i < numberOfObjects; i++) {
		delete[] X[i];
		X[i] = nullptr;
	}
	delete[] X;
	X = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
 		delete[] V[i];
		V[i] = nullptr;
	}
	delete[] V;
	V = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
 		delete[] pbest[i];
		pbest[i] = nullptr;
	}
	delete[] pbest;
	pbest = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] d[i];
		d[i] = nullptr;
	}
	delete[] d;
	d = nullptr;

	delete[] gbest;

	for (int j = 0; j < numberOfClusters; j++) {
		delete centers[j];
		centers[j] = nullptr;
	}
	delete[] centers;
	centers = nullptr;

	for (int j = 0; j < numberOfClusters; j++) {
		delete oldCenters[j];
		oldCenters[j] = nullptr;
	}
	delete[] oldCenters;
	oldCenters = nullptr;
}

PSOcounter::PSOcounter(const Dataset & pdata)
{
	init(pdata);
}

void PSOcounter::count(const Dataset * pdata)
{
	if (pdata) {
		init(*pdata);
	}
	if (data) {
		if ((numberOfObjects = data->getSize()) > 0) {
			numberOfCoordinates = (*data)[0].getNumberOfCoordinates();

			int numberOfIterations = 0;

			XInit();
			printMatrix(X,"Matica pozicii X: ");

			pbestInit();
			printMatrix(pbest,"Matica pbest: ");

			gbestInit();
			gbestPrint();

			dInit();
			fitnessInit();

			VInit();
			printMatrix(V,"Matica rychlosti V: ");

			centersInit();
			computeCenters();
			centersPrint();

			int i = 1;
			do {	
				cout << "Round" << i++ << endl;
				computeD();
				dPrint();

				checkFitness();
				printMatrix(pbest, "Pbest: ");
				gbestPrint();

				computeV();
				computeX();
				printMatrix(V, "Matica rychlosti V: ");
				printMatrix(X, "Matica pozicii X: ");
				normalizeX();
				printMatrix(X, "Matica pozicii X: ");

				computeCenters();
				centersPrint();
			} while (numberOfIterations++ < maxIterationNumber);
		}
	}
}

void PSOcounter::XInit()
{
	int which = 0;
	X = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		X[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			if (which%numberOfClusters == 0)
				X[i][j] = 1;
			else
				X[i][j] = 0;
			which++;
		}
		which++;
	}
}

void PSOcounter::computeX()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = X[i][j] + V[i][j];
		}
	}
}

//nie nula!!!
void PSOcounter::normalizeX()
{
	for (int i = 0; i < numberOfObjects; i++) {
		int count = 0;
		double rowSum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			if (X[i][j] < 0) {
				X[i][j] = 0.001;
				count++;
			}
			rowSum += X[i][j];
		}
		if (count == numberOfClusters) {
			for (int j = 0; j < numberOfClusters; j++) {
				if (X[i][j] < 0) {
					X[i][j] = rand() / RAND_MAX;
					count++;
				}
			}
		}
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = X[i][j] / rowSum;
		}
	}
}

void PSOcounter::pbestInit()
{
	int which = 0;
	pbest = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		pbest[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			if (which%numberOfClusters == 0)
				pbest[i][j] = 1;
			else
				pbest[i][j] = 0;
			which++;
		}
		which++;
	}
}

void PSOcounter::fitnessInit()
{
	fitnessPbest = new double[numberOfClusters];		
	for (int j = 0; j < numberOfClusters; j++) {
		fitnessPbest[j] = 0.00001;
	}
	fitnessGbest = 0.00001;
}

void PSOcounter::VInit()
{
	V = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		V[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = (double)rand() / RAND_MAX * 2.0 - 1.0;
		}
	}
}

void PSOcounter::computeV()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = w * V[i][j] + c1 * r1*(pbest[i][j] - X[i][j]) + c2 * r2*(gbest[j] - X[i][j]);
		}
	}
}

void PSOcounter::printMatrix(double** matrix, const char* text) const
{
	cout << text << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

//inicializacia centier
void PSOcounter::centersInit()
{
	centers = new Object*[numberOfClusters];
	oldCenters = new Object*[numberOfClusters];
	for (int j = 0; j < numberOfClusters; j++) {
		Object *flower = new Object();
		centers[j] = flower;
		flower = new Object();
		oldCenters[j] = flower;
	}
}

//vypis centier
void PSOcounter::centersPrint() const
{
	cout << "Centra:" << endl;
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum" << j + 1 << ": ";
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << centers[j]->getValue(k) << " ";
		}
		cout << endl;
	}
}

//vypocet centier
void PSOcounter::computeCenters()
{
	vector<double> values;
	for (int j = 0; j < numberOfClusters; j++) {
		values.clear();
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum1 = 0;
			double sum2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				double product2 = 1;
				for (int l = 0; l < m; l++) {
					product2 *= X[i][j];
				}
				double product1 = product2 * (*data)[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			values.push_back(sum1 / sum2);
		}
		oldCenters[j]->setValues(centers[j]->getValues());
		centers[j]->setValues(values);
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordinates; k++) {
				double a1 = centers[j]->getValue(k);
				double a2 = (*data)[i].getValue(k);
				if (abs(centers[j]->getValue(k) - (*data)[i].getValue(k)) < 0.00000001) {
					centers[j]->setValue(k, centers[j]->getValue(k) + 0.1*minChange);
				}
			}
		}

	}
}

void PSOcounter::gbestInit()
{
	//gbest = (double)rand() / RAND_MAX;
	gbest = new double[numberOfClusters];
	for (int j = 0; j < numberOfClusters; j++) {
		gbest[j] = 0.000001;
	}
}

void PSOcounter::gbestPrint() const
{
	cout << "Gbest: ";
	cout << *gbest << endl;
}

void PSOcounter::fitnessGbestPrint() const
{
	cout << "Fitness gbest: ";
	cout << fitnessGbest << endl;
}

double PSOcounter::getOnesFitness(const int which) const
{
	double Jm = 0;
	for (int j = 0; j < numberOfClusters; j++) {
		Jm += pow(X[which][j],m) * d[which][j];
	}

	return K / Jm;
}

void PSOcounter::checkFitness()
{
	for (int i = 0; i < numberOfObjects; i++) {
		double newFitness = getOnesFitness(i);
		if (newFitness > fitnessPbest[i]) {
			fitnessPbest[i] = newFitness;
			*pbest[i] = *X[i];
			if (newFitness > fitnessGbest) {
				fitnessGbest = newFitness;
				*gbest = *X[i];
			}
		}
	}
}

//vypocet matice euklidovskej vzdialenosti
void PSOcounter::computeD()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = (*data)[i].getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			d[i][j] = sqrt(sum);
		}
	}
}

//inicializacia matice euklidovskych vzdialenosti
void PSOcounter::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
	}
}

//vypis matice euklidovskej vzdialenosti
void PSOcounter::dPrint() const
{
	cout << "Matica euklidovskej vzdialenosti:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}
