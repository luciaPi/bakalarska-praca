#include "CounterData.h"
#include <iostream>

using namespace std;

CounterData::CounterData()
{
	init();
}

CounterData::CounterData(int parnumberOfClusters)
{
	if (parnumberOfClusters > 0) {
		numberOfClusters = parnumberOfClusters;
	}
	init();	
}

CounterData::CounterData(Dataset pardata)
{
	data = pardata;
	init();
}

CounterData::CounterData(Dataset pardata, int parnumberOfClusters, int parM)
{
	data = pardata;
	if (parnumberOfClusters > 0) {
		numberOfClusters = parnumberOfClusters;
	}
	if (m > 0) {
		m = parM;
	}
	init();
}

CounterData::~CounterData()
{
	clear();
}

void CounterData::setNumberOfClusters(int number)
{
	if (number > 0) {
		numberOfClusters = number;
	}
	init();
}

void CounterData::setData(Dataset pardata)
{
	data = pardata;
	init();
}

void CounterData::setM(int parM)
{
	if (m > 0) {
		m = parM;
	}
	init();
}

void CounterData::setK(int parK)
{
	if (K > 0) {
		K = parK;
	}
}

void CounterData::setMinChange(double parminChange)
{
	if (parminChange > 0) {
		minChange = parminChange;
	}
}

void CounterData::init()
{
	clear();
	numberOfObjects = data.getSize();
	if (numberOfObjects > 0) {
		numberOfCoordinates = data[0].getNumberOfCoordinates();
	}
	else {
		numberOfCoordinates = 0;
	}
	minChange = 0.0001;

	muInit();
	centersInit();
	dInit();

	recalculate();
}

void CounterData::clear()
{
	if (centers) {
		for (int j = 0; j < numberOfClusters; j++) {
			delete centers[j];
			centers[j] = nullptr;
		}
		delete[] centers;
		centers = nullptr;
	}

	if (oldCenters) {
		for (int j = 0; j < numberOfClusters; j++) {
			delete oldCenters[j];
			oldCenters[j] = nullptr;
		}
		delete[] oldCenters;
		oldCenters = nullptr;
	}

	if (mu) {
		for (int i = 0; i < numberOfObjects; i++) {
			delete[] mu[i];
			mu[i] = nullptr;
		}
		delete[] mu;
		mu = nullptr;
	}

	if (d) {
		for (int i = 0; i < numberOfObjects; i++) {
			delete[] d[i];
			d[i] = nullptr;
		}
		delete[] d;
		d = nullptr;
	}
}

//inicializacia matice mu
void CounterData::muInit()
{	
	mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = (double)rand() / RAND_MAX;;
		}
	}
	normalizeMu();

	/*int which = 0;
	mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			if (which%numberOfClusters == 0)
				mu[i][j] = 1;
			else
				mu[i][j] = 0;
			which++;
		}
		which++;
	}*/

	/*double values[] = { 0.15, 0.45, 0.4, 0,0.5,0.5,0.25,0.75,0,1,0,0 ,0.25,0.75,0 };
	int which = 0;
	mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = values[which];
			which++;
		}
	}*/
}

void CounterData::muPrint() const
{
	cout << name << " - Matica prislusnosti mu:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << mu[i][j] << " ";
		}
		cout << endl;
	}
}

void CounterData::computeMu()
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
	recalculate();
}

void CounterData::normalizeMu()
{
	for (int i = 0; i < numberOfObjects; i++) {
		double rowSum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			rowSum += mu[i][j];
		}
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = mu[i][j] / rowSum;
		}
	}
}

void CounterData::recalculate()
{
	computeCenters();
	computeD();
}

void CounterData::printAsMatrix(double * matrix, const char * text) const
{
	cout << text << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << matrix[i*numberOfClusters+j] << " ";
		}
		cout << endl;
	}
}

void CounterData::setMatrix(double ** source, double ** dest)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			dest[i][j] = source[i][j];
		}
	}
}

void CounterData::setMu(const double ** parmu)
{
	if (parmu) {
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				mu[i][j] = parmu[i][j];
			}
		}
	}
}

void CounterData::setName(string pname)
{
	name = pname;
}

void CounterData::centersInit()
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

void CounterData::centersPrint() const
{
	cout << name <<  " - Centra:" << endl;
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum" << j + 1 << ": ";
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << centers[j]->getValue(k) << " ";
		}
		cout << endl;
	}
}

void CounterData::computeCenters()
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
				double product1 = product2 * data[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			values.push_back(sum1 / sum2);
		}
		*(oldCenters[j]) = *(centers[j]);
		centers[j]->setValues(values);
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordinates; k++) {
				double a1 = centers[j]->getValue(k);
				double a2 = data[i].getValue(k);
				if (abs(centers[j]->getValue(k) - data[i].getValue(k)) < 0.00000001) {
					centers[j]->setValue(k, centers[j]->getValue(k) + 0.1*minChange);
				}
			}
		}
	}
}

void CounterData::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
	}
}

void CounterData::dPrint() const
{
	cout << name << " - Matica euklidovskej vzdialenosti:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}

//vypocet matice euklidovskej vzdialenosti
void CounterData::computeD()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = data[i].getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			d[i][j] = sqrt(sum);
		}
	}
}

double CounterData::getJm() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(mu[i][j], m) * pow(d[i][j], 1);
		}
	}
	return Jm;
}

void CounterData::printJm() const
{
	cout << "Jm " << name << " = ";
	cout << getJm() << endl;
}

double CounterData::getFitness() const
{
	return K/getJm();
}

void CounterData::printFitness() const
{
	cout << "Fitness " << name << " = ";
	cout << getFitness() << endl;
}

//je zmena centier oproti centram v minulom kroku vyznamna
bool CounterData::wasSignificantChange() const
{
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			if ((abs(oldCenters[j]->getValue(k) - centers[j]->getValue(k))) > minChange) {
				return true;
			}
		}
	}
	return false;
}