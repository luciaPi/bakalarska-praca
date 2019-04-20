#include "CVI.h"
#include "FCMcounter.h"
#include <math.h>  

double CVI::getMinCentersDistance() const
{
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

double CVI::getMaxCentersDistance() const
{
	double max = -1;
	for (int i = 0; i < numberOfClusters; i++) {
		for (int j = i; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = centers[i]->getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			double distance = sqrt(sum);
			if (distance > max) {
				max = distance;
			}
		}
	}
	return max;
}

double CVI::getJmDifferent() const
{
	double** d = fuzzydata->getDistances();
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(mu[i][j], fuzzydata->getM()) * pow(d[i][j], 1);
		}
	}
	return Jm;
}

Object** CVI::getMainCenter()
{		
	fcmCounter.count(data,1, fuzzydata->getM(), fuzzydata->getK(), fuzzydata->getMuInitMode());
	FuzzyData* forOneCluster = fcmCounter.getBest();
	return forOneCluster->getCenters();
}

double CVI::getMinBeta(int which) const
{
	double min = INT_MAX;
	double beta = getBeta();
	for (int j = 0; j < numberOfClusters; j++) {
		if (j != which) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = centers[which]->getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			double distance = sqrt(sum);
			double xx = pow(distance, 2)/beta;
			if (xx < min) {
				min = xx;
			}
		}
	}
	return min;
}

double CVI::getMuM() const
{
	double min = INT_MAX;
	for (int i = 0; i < numberOfClusters; i++) {
		double sum = 0;
		for (int j = i; j < numberOfObjects; j++) {
			sum += pow(mu[i][j], 2);
		}
		if (sum < min) {
			min = sum;
		}
	}
	return min;
}

double CVI::getBeta() const
{
	double sum = 0;

	double* meanCenters = new double[numberOfCoordinates];
	for (int k = 0; k < numberOfCoordinates; k++) {
		double sum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			sum += centers[j]->getValue(k);
		}
		meanCenters[k] = sum / numberOfClusters;
	}

	for (int j = 0; j < numberOfClusters; j++) {
		double distance = 0;
		for (int k = 0; k < numberOfCoordinates; k++) {
			double coordinate1 = centers[j]->getValue(k);
			distance += pow(coordinate1 - meanCenters[k], 2);
		}
		distance = sqrt(distance);
		sum += pow(distance, 2);
	}
	double beta = sum / numberOfClusters;
	return beta;
}

double CVI::getA(int which) const
{
	double sum = 0;
	double number = 0;
	Attribute* objectAssignedClass = data[which].getObjectAssignedClass();
	for (int i = 0; i < numberOfObjects; i++) {
		if (i != which) {
			if (data[i].getObjectAssignedClass() == objectAssignedClass) {
				double distance = 0;
				for (int k = 0; k < numberOfCoordinates; k++) {
					double coordinate1 = data[i].getValue(k);
					double coordinate2 = data[which].getValue(k);
					distance += pow(coordinate1 - coordinate2, 2);
				}
				distance = sqrt(distance);
				sum += distance;
				number++;
			}
		}
	}
	return sum / number;
}

double CVI::getB(int which) const
{
	double min = INT_MAX;
	Attribute* objectAssignedClass = data[which].getObjectAssignedClass();
	for (Attribute* actual : clusters) {
		if (actual != objectAssignedClass) {
			double sum = 0;
			double number = 0;			
			for (int i = 0; i < numberOfObjects; i++) {
				if (data[i].getObjectAssignedClass() == actual) {
					double distance = 0;
					for (int k = 0; k < numberOfCoordinates; k++) {
						double coordinate1 = data[i].getValue(k);
						double coordinate2 = data[which].getValue(k);
						distance += pow(coordinate1 - coordinate2, 2);
					}
					distance = sqrt(distance);
					sum += distance;
					number++;					
				}
			}
			double average = sum / number;
			if (average < min) {
				min = average;
			}
		}
	}
	return min;
}

double ** CVI::getF(double** F, int which)
{
	double sum1 = 0;
	int m = fuzzydata->getM();
	for (int i = 0; i < numberOfObjects; i++) {
		sum1 += pow(mu[i][j], m);	
		sum2
	}
	for (int k = 0; k < numberOfCoordinates; k++) {
		for (int l = 0; l < numberOfCoordinates; l++) {
			F[k][l] = F[k][l] / sum1;
		}
	}
	return F;
}

CVI::CVI()
{
	whichCount = new bool[number];
	for (int i = 0; i < number; i++) {
		whichCount[i] = false;
	}
}

CVI::CVI(int seed1, int seed2)
{
	whichCount = new bool[number];
	for (int i = 0; i < number; i++) {
		whichCount[i] = false;
	}
	fcmCounter = FCMcounter(seed1, seed2);
	int x = 0;
}


CVI::~CVI()
{
	delete whichCount;
	whichCount = nullptr;
}

bool CVI::count(FuzzyData* pdata)
{
	if (pdata != nullptr) {
		fuzzydata = pdata;

		numberOfObjects = fuzzydata->getNumberOfObjects();
		numberOfClusters = fuzzydata->getNumberOfClusters();
		numberOfCoordinates = fuzzydata->getNumberOfCoordinates();
		centers = fuzzydata->getCenters();
		mu = fuzzydata->getMu();
		data = fuzzydata->getData();

		int i = 0;
		whichCount[i++] ? countPC() : -1;
		whichCount[i++] ? countMPC() : -1;
		whichCount[i++] ? countPE() : -1;
		whichCount[i++] ? countFS() : -1;
		whichCount[i++] ? countXIEBENI() : -1;
		whichCount[i++] ? countKWON() : -1;
		//whichCount[i++] ? countFHV() : -1;
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
		delete[] meanObjects;

		KWONvalue = (Jm + xx) / min;
	}
	else {
		KWONvalue = -1;
	}
	return KWONvalue;
}

double CVI::countFHV()
{
	if (fuzzydata != nullptr) {
		double** F = new double*[numberOfCoordinates];
		for (int k = 0; k < numberOfCoordinates; k++) {
			F[k] = new double[numberOfCoordinates];
		}

		double sum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			F = getF(F,j);
			double det = getDeterminant(F);
			sum += sqrt(det);
		}
		FHVvalue = sum;

		for (int k = 0; k < numberOfCoordinates; k++) {
			delete[] F[k];
		}
		delete[] F;
	}
	else {
		FHVvalue = -1;
	}
	return FHVvalue;
}

double CVI::countPBMF()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();
		double min = getMinCentersDistance();

		Object** mainCenter = getMainCenter();
		double E1 = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			double distance = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = data[i].getValue(k);
				double coordinate2 = mainCenter[k]->getValue(k);
				distance += pow(coordinate1 - coordinate2, 2);
			}
			distance = sqrt(distance);
			E1 += distance;
		}
		double Dc = getMaxCentersDistance();
		double JmDif = getJmDifferent();

		PBMFvalue = pow((1/numberOfClusters)*(E1/JmDif)*Dc,2);
	}
	else {
		PBMFvalue = -1;
	}
	return PBMFvalue;
}

double CVI::countPCAES()
{
	if (fuzzydata != nullptr) {
		double muM = getMuM();
		double sum1 = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			for (int i = 0; i < numberOfObjects; i++) {
				sum1 += pow(mu[i][j],2) / muM;
			}
		}
		double sum2 = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			double min = getMinBeta(j);
			sum2 += exp(-min);
		}
		PCAESvalue = sum1 - sum2;;
	}
	else {
		PCAESvalue = -1;
	}
	return PCAESvalue;
}

double CVI::countSILHOUETTE()
{
	if (fuzzydata != nullptr && clusters.size() == numberOfClusters) {
		double sum = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			double a = getA(i);
			double b = getB(i);
			double max = a > b ? a : b;
			sum += (b-a) / max;
		}
		SILHOUETTEvalue = sum / numberOfClusters;
	}
	else {
		SILHOUETTEvalue = -1;
	}
	return SILHOUETTEvalue;
}

double CVI::countMPC()
{
	if (fuzzydata != nullptr) {
		double PC = countPC();
		MPCvalue = 1-(numberOfClusters/(numberOfClusters-1))*(1-PC);
	}
	else {
		MPCvalue = -1;
	}
	return MPCvalue;
}

