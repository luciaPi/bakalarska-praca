#include "Firefly.h"
#include <iostream>
#include <math.h> 

using namespace std;

void Firefly::init(int parm, double parLAC, double paratractiveness, int parnumberOfClusters, double parminchange, const Dataset* pdata)
{
	m = parm;
	atractiveness = paratractiveness;
	LAC = parLAC;
	numberOfClusters = parnumberOfClusters;
	minChange = parminchange;
	Jm = LONG_MAX;
	alpha = (double)rand() / RAND_MAX;

	if (pdata) {
		data = pdata;
		numberOfObjects = data->getSize();
		numberOfCoordinates = (*data)[0].getNumberOfCoordinates();
		//numberOfCoordinates = 1;
		muInit();
		dInit();
		setMinMaxCoordinates();

		centersInit();
	}
}

Firefly::Firefly()
{
}

Firefly::Firefly(int parm, double parLAC, double paratractivenes, int parnumberOfClusters, double parminchange, const Dataset * pdata)
{
	init(parm, parLAC, paratractivenes, parnumberOfClusters, parminchange, pdata);
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

	delete[] minCoordinate;
	minCoordinate = nullptr;

	delete[] maxCoordinate;
	maxCoordinate = nullptr;
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

		vector<double> values;
		for (int k = 0; k < numberOfCoordinates; k++) {
			double min = getMinCoordinate(k);
			double max = getMaxCoordinate(k);
			double value = (double)rand() / RAND_MAX * (maxCoordinate[k]-minCoordinate[k]) + minCoordinate[k];
			values.push_back(value);
		}
		centers[j]->setValues(values);
		
		object = new Object();
		oldCenters[j] = object;
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

vector<Object> Firefly::getCenters()
{
	vector<Object> centersVector;
	for (int j = 0; j < numberOfClusters; j++) {
		centersVector.push_back(*centers[j]);
	}
	return centersVector;
}

double Firefly::computeDistanceOfCenters(Object first,Object second) const
{
	double sum = 0;
	for (int k = 0; k < numberOfCoordinates; k++) {
		double coordinate1 = first.getValue(k);
		double coordinate2 = second.getValue(k);
		sum += pow(coordinate1 - coordinate2, 2);
	}
	return sqrt(sum);
}

double Firefly::computeDistanceOfAllCenters(vector<Object> otherCenters) const
{
	double allSum = 0;
	for (int j = 0; j < numberOfClusters; j++) {
		double sum = 0;
		for (int k = 0; k < numberOfCoordinates; k++) {
			double coordinate1 = centers[j]->getValue(k);
			double coordinate2 = otherCenters[j].getValue(k);
			sum += pow(coordinate1 - coordinate2, 2);
		}
		allSum += sqrt(sum);
	}
	return allSum;
}

void Firefly::move(vector<Object> otherCenters)
{
	double r = computeDistanceOfAllCenters(otherCenters);
	double beta = atractiveness * exp(-LAC * pow(r, 2));

	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			double randNumber = (double)rand() / RAND_MAX;
			double rCenter = computeDistanceOfCenters(*centers[j], otherCenters[j]);
			
			double newValue = centers[j]->getValue(k) + beta*r+alpha*(randNumber-0.5);
			if (newValue < minCoordinate[k]) {
				newValue = minCoordinate[k];
			}
			if (newValue > maxCoordinate[k]) {
				newValue = maxCoordinate[k];
			}			
			centers[j]->setValue(k, newValue);
		}
	}
}

void Firefly::setCenters(vector<Object> newCenters)
{
	if (newCenters.size() != numberOfClusters) {
		return;
	}
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			centers[j]->setValue(k,newCenters[j].getValue(k));
		}
	}
	computeD();
	computeMu();
	setJm();
}

void Firefly::muPrint() const
{
	printMatrix(mu, "MU: ");
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

double Firefly::getMinCoordinate(int which) const
{
	double min = LONG_MAX;
	for (int i = 0; i < numberOfObjects; i++) {
		Object current = (*data)[i];
		double value = current.getValue(which);
		if (value < min) {
			min = value;
		}
	}
	return min-0.01;
}

double Firefly::getMaxCoordinate(int which) const
{
	double max = LONG_MIN;
	for (int i = 0; i < numberOfObjects;i++) {
		Object current = (*data)[i];
		double value = current.getValue(which);
		if (value > max) {
			max = value;
		}
	}
	return max+0.01;
}

void Firefly::setMinMaxCoordinates()
{
	minCoordinate = new double[numberOfCoordinates];
	maxCoordinate = new double[numberOfCoordinates];
	for (int k = 0; k < numberOfCoordinates; k++) {
		minCoordinate[k] = getMinCoordinate(k);
		maxCoordinate[k] = getMaxCoordinate(k);
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

void Firefly::computeMu()
{
	double index = 2 / (m - 1);
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfClusters; k++) {
				double value = d[i][j] / d[i][k];
				sum += pow(value, index);
			}
			mu[i][j] = 1 / sum;
		}
	}
}

void Firefly::JmPrint() const
{
	cout << "Jm: " << getJm() << endl;
}

double Firefly::getJm() const
{
	return Jm;
}

double Firefly::setJm()
{
	double JmNew = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			JmNew += pow(mu[i][j], m) * d[i][j];
		}
	}
	Jm = JmNew;
	return JmNew;
}
