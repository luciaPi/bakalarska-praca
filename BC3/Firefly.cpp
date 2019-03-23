#include "Firefly.h"
#include <iostream>

using namespace std;

void Firefly::init(int parm, double parLAC, double paratractiveness, int parnumberOfClusters, double parminchange, const Dataset* pdata)
{
	m = parm;
	atractiveness = paratractiveness;
	LAC = parLAC;
	numberOfClusters = parnumberOfClusters;
	minChange = parminchange;

	if (pdata) {
		data = pdata;
		numberOfObjects = data->getSize();
		numberOfCoordinates = (*data)[0].getNumberOfCoordinates();
		muInit();
		dInit();
		centersInit();
	}
}

Firefly::Firefly()
{
}


Firefly::~Firefly()
{
	for (int i = 0; i < numberOfObjects; i++) {
		delete[] mu[i];
		mu[i] = nullptr;
	}
	delete[] mu;
	mu = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] d[i];
		d[i] = nullptr;
	}
	delete[] d;
	d = nullptr;

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


void Firefly::muInit()
{
	mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = (double)rand() / RAND_MAX;
		}
	}
	normalizeMu();
	/*double values[] = { 0.15, 0.45, 0.4, 0,0.5,0.5,0.25,0.75,0,1,0,0 ,0.25,0.75,0 };
	int which = 0;
	X = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		X[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = values[which];
			which++;
		}
	}*/

}

//inicializacia matice euklidovskych vzdialenosti
void Firefly::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
	}
}

//inicializacia centier
void Firefly::centersInit()
{
	centers = new Object*[numberOfClusters];
	oldCenters = new Object*[numberOfClusters];
	for (int j = 0; j < numberOfClusters; j++) {
		Object *object = new Object();
		centers[j] = object;
		object = new Object();
		oldCenters[j] = object;
	}
}

//vypocet centier
void Firefly::computeCenters()
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
					product2 *= mu[i][j];
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

//vypocet matice euklidovskej vzdialenosti
void Firefly::computeD()
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

//vypis centier
void Firefly::centersPrint() const
{
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum" << j + 1 << ": ";
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << centers[j]->getValue(k) << " ";
		}
		cout << endl;
	}

}

//vypis matice euklidovskej vzdialenosti
void Firefly::dPrint() const
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}

void Firefly::muPrint() const
{
	printMatrix(mu, "X: ");
}


void Firefly::setMatrix(double ** source, double **dest)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			dest[i][j] = source[i][j];
		}
	}
}

void Firefly::setMu(double ** newMu)
{
	setMatrix(newMu, mu);
}

void Firefly::printMatrix(double** matrix, const char* text) const
{
	cout << text << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

//nie nula!!!
void Firefly::normalizeMu()
{
	for (int i = 0; i < numberOfObjects; i++) {
		int count = 0;
		double rowSum = 0;
		for (int j = 0; j < numberOfClusters; j++) {			
			rowSum += mu[i][j];
		}
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = mu[i][j] / rowSum;
		}
	}
}

void Firefly::JmPrint() const
{
	cout << "Jm: " << getJm() << endl;
}

double Firefly::getJm() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(mu[i][j], m) * d[i][j];
		}
	}

	return Jm;
}