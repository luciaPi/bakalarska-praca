#include "FCMcounter.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include "Attribute.h"

FCMcounter::FCMcounter(const Dataset &pdata)
{	
	init(pdata);
}

void FCMcounter::init(const Dataset& pdata)
{
	m = 2;
	numberOfClusters = 3;
	numberOfCoordinates = 0;
	minChange = 0.001;
	data = nullptr;
	centers = nullptr;
	mu = nullptr;
	oldCenters = nullptr;
	d = nullptr;
	data = &pdata;
}

void FCMcounter::setMu(double const ** parmu)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = parmu[i][j];
		}
	}
}

FCMcounter::~FCMcounter()
{
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
}

//kroky algoritmu
void FCMcounter::count(const Dataset* pdata, const double** parmu)
{
	if (pdata) {
		init(*pdata);
	}
	if (data) {
		if ((numberOfObjects = data->getSize()) > 0) {
			numberOfCoordinates = (*data)[0].getNumberOfCoordinates();			
 			muInit();
			if (parmu) {
				setMu(parmu);
			}
			//muPrint();
			dInit();
			centersInit();

			computeCenters();
			//centersPrint();

			int i = 0;
			do {
				//cout << "Round" << i << endl;
				computeD();
				//dPrint();

				computeMu();
				//muPrint();

				computeCenters();
				//centersPrint();
				//computeD();
				//printJm();
				i++;
			} while (isSignificantChange());
			//dPrint();
			//objectsPrintWithType();
			printJm();
			centersPrint();
			//muPrint();

		}
	}
}

double FCMcounter::getFitness() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(mu[i][j], m) * d[i][j];
		}
	}

	return Jm;
}

void FCMcounter::printJm() const
{
	cout << "Jm FCM = ";
	cout << getFitness() << endl;
}

//inicializacia matice mu
void FCMcounter::muInit()
{
	int which = 0;
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
	}
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

//vypis matice mu
void FCMcounter::muPrint() const
{
	cout << "Matica prislusnosti mu:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << mu[i][j] << " ";
		}
		cout << endl;
	}
}

//vypocet matice mu
void FCMcounter::computeMu()
{
	double index = 2 / (m - 1);
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfClusters; k++) {
				double value = d[i][j] / d[i][k];
				sum += pow(value,index);
			}
			mu[i][j] = 1 / sum;
		}
	}
}

//inicializacia centier
void FCMcounter::centersInit()
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
void FCMcounter::centersPrint() const
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
void FCMcounter::computeCenters()
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
					centers[j]->setValue(k, centers[j]->getValue(k)+0.1*minChange);
				}
			}
		}
		
	}
}

//vypocet matice euklidovskej vzdialenosti
void FCMcounter::computeD()
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

//je zmena centier oproti centram v minulom kroku vyznamna
bool FCMcounter::isSignificantChange() const
{
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates;k++) {
			if ((abs(oldCenters[j]->getValue(k) - centers[j]->getValue(k))) > minChange) {
				return true;
			}
		}
	}
	return false;
}

//ku ktoremu centru ma objekt najvyssiu prislusnost
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
}

//zisti poradie daneho objektu v ramci vsetkych objektov
int FCMcounter::whichNumberOfObject(const Object & flower) const
{
	return data->whichNumberOfObject(flower);
}

//inicializacia matice euklidovskych vzdialenosti
void FCMcounter::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
	}
}

//vypis matice euklidovskej vzdialenosti
void FCMcounter::dPrint() const
{
	cout << "Matica euklidovskej vzdialenosti:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}

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
}