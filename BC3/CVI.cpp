#include "CVI.h"

double CVI::getMinCentersDistance() const
{
	const Object** centers = fuzzydata->getCenters();
	int numberOfClusters = fuzzydata->getNumberOfClusters();
	int numberOfCoordinates = fuzzydata->getNumberOfCoordinates();

	double min = INT_MAX;
	for (int i = 0; i < numberOfClusters; i++) {
		for (int j = i; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = centers[i]->getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			double distance = sqrt(sum);
			if (distance < min) {
				min = distance;
			}
		}
	}
	return min;
}

CVI::CVI()
{
	whichCount = new bool[number];
	for (int i = 0; i < number; i++) {
		whichCount[i] = false;
	}
}


CVI::~CVI()
{
	delete whichCount;
	whichCount = nullptr;
}

bool CVI::count(FuzzyData* data)
{
	if (data != nullptr) {
		data = fuzzydata;
		int i = 0;
		whichCount[i++] ? countPC() : -1;
		whichCount[i++] ? countMPC() : -1;
		whichCount[i++] ? countPE() : -1;
		whichCount[i++] ? countFS() : -1;
		whichCount[i++] ? countXIEBENI() : -1;
		whichCount[i++] ? countKWON() : -1;
		whichCount[i++] ? countFHV() : -1;
		whichCount[i++] ? countPBMF() : -1;
		whichCount[i++] ? countPCAES() : -1;
		whichCount[i++] ? countSILHOUETTE() : -1;
		return true;
	}
	return false;
}

double CVI::countPC()
{
	if (fuzzydata != nullptr) {
		double sum = 0;
		int numberOfClusters = fuzzydata->getNumberOfClusters();
		int numberOfObjects = fuzzydata->getNumberOfObjects();
		const double** mu = fuzzydata->getMu();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfObjects; j++) {
				sum += pow(mu[i][j], 2);
			}
		}
		PCvalue = sum / numberOfObjects;
	}
	else {
		PCvalue = -1;
	}
	return PCvalue;
}

double CVI::countPE()
{
	if (fuzzydata != nullptr) {
		double sum = 0;
		int numberOfClusters = fuzzydata->getNumberOfClusters();
		int numberOfObjects = fuzzydata->getNumberOfObjects();
		const double** mu = fuzzydata->getMu();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfObjects; j++) {
				sum += mu[i][j] * log(mu[i][j]);
			}
		}
		PEvalue = (-sum) / numberOfObjects;
	}
	else {
		PEvalue = -1;
	}
	return PEvalue;
}

double CVI::countFS()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();

		int m = fuzzydata->getM();
		int numberOfClusters = fuzzydata->getNumberOfClusters();
		int numberOfObjects = fuzzydata->getNumberOfObjects();
		const double** mu = fuzzydata->getMu();
		const Object** centers = fuzzydata->getCenters();
		Dataset data = fuzzydata->getData();
		int numberOfCoordinates = data[0].getNumberOfCoordinates();

		double* meanCenters = new double[numberOfCoordinates];
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum = 0;
			for (int j = 0; j < numberOfClusters; j++) {
				sum += centers[j]->getValue(k);
			}
			meanCenters[k] = sum / numberOfClusters;
		}

		double Km = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				double sum = 0;
				for (int k = 0; k < numberOfCoordinates; k++) {
					double center = centers[j]->getValue(k);
					sum += pow(center - meanCenters[k], 2);
				}
				double distance = sqrt(sum);
				Km += pow(mu[i][j], m)*pow(distance, 2);
			}
		}
		delete[] meanCenters;
		FSvalue = Jm - Km;
	}
	else {
		FSvalue = -1;
	}
	return FSvalue;
}

double CVI::countXIEBENI()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();
		int numberOfObjects = fuzzydata->getNumberOfObjects();		
		double min = getMinCentersDistance();

		XIEBENIvalue = Jm/(numberOfObjects*min);
	}
	else {
		XIEBENIvalue = -1;
	}
	return XIEBENIvalue;
}

double CVI::countKWON()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();
		int numberOfObjects = fuzzydata->getNumberOfObjects();
		const Object** centers = fuzzydata->getCenters();
		int numberOfObjects = fuzzydata->getNumberOfObjects();
		int numberOfClusters = fuzzydata->getNumberOfClusters();
		int numberOfCoordinates = fuzzydata->getNumberOfCoordinates();
		double min = getMinCentersDistance();

		double* meanObjects = new double[numberOfCoordinates];
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				sum += centers[i]->getValue(k);
			}
			meanObjects[k] = sum / numberOfObjects;
		}

		double sum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			double distance = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = centers[j]->getValue(k);
				double coordinate2 = meanObjects[k];
				distance += pow(coordinate1 - coordinate2, 2);
			}
			distance = sqrt(distance);
			sum += pow(distance, 2);
		}
		double xx = (1 / numberOfClusters)*sum;

		KWONvalue = (Jm + xx) / min;
	}
	else {
		KWONvalue = -1;
	}
	return KWONvalue;
}

double CVI::countMPC()
{
	if (fuzzydata != nullptr) {
		double PC = countPC();
		int numberOfClusters = fuzzydata->getNumberOfClusters();
		MPCvalue = 1-(numberOfClusters/(numberOfClusters-1))*(1-PC);
	}
	else {
		MPCvalue = -1;
	}
	return MPCvalue;
}

