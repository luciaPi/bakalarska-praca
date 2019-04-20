#include "FuzzyData.h"
#include <iostream>

using namespace std;

FuzzyData::FuzzyData()
{
	init();
}

FuzzyData::FuzzyData(int parnumberOfClusters)
{
	if (parnumberOfClusters > 0) {
		numberOfClusters = parnumberOfClusters;
	}
	init();	
}

FuzzyData::FuzzyData(Dataset pardata)
{
	data = pardata;
	init();
}

FuzzyData::FuzzyData(Dataset pardata, int parnumberOfClusters, int parM, int parK, RandomGenerator* parmugenerator, RandomGenerator* parcentersGenerator,MuInitializationMode parmuInitMode)
{
	K = parK;
	muGenerator = parmugenerator;
	centersGenerator = parcentersGenerator;
	muInitMode = parmuInitMode;
	data = pardata;
	if (parnumberOfClusters > 0) {
		numberOfClusters = parnumberOfClusters;
	}
	if (m > 0) {
		m = parM;
	}
	init();
}

FuzzyData::~FuzzyData()
{
	clear();
}

void FuzzyData::setNumberOfClusters(int number)
{
	if (number > 0) {
		numberOfClusters = number;
	}
	init();
}

void FuzzyData::setData(Dataset pardata)
{
	data = pardata;
	init();
}

void FuzzyData::setM(int parM)
{
	if (m > 0) {
		m = parM;
	}
	init();
}

void FuzzyData::setK(int parK)
{
	if (K > 0) {
		K = parK;
	}
}

void FuzzyData::setMinChange(double parminChange)
{
	if (parminChange > 0) {
		minChange = parminChange;
	}
}

void FuzzyData::init()
{
	if (muGenerator == nullptr) {
		throw exception();
	}
	clear();
	numberOfObjects = data.getSize();
	if (numberOfObjects > 0) {
		numberOfCoordinates = data[0].getNumberOfCoordinates();
	}
	else {
		numberOfCoordinates = 0;
	}

	centersInit();
	dInit();
	muInit();

	recalculateFromMu();
}

void FuzzyData::clear()
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
void FuzzyData::muInit()
{	
	/*mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
	}
	if (muInitMode == MuInitializationMode::fcmPlusPlus) {
		plusplusInit();
	}
	else {
		randomMuInit();
	}*/
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
	//test
	double values[] = { 0.15, 0.45, 0.4, 0,0.5,0.5,0.25,0.75,0,1,0,0 ,0.25,0.75,0 };
	int which = 0;
	mu = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = values[which];
			which++;
		}
	}
	/*double values[] = { 0.8, 0.2, 0.9, 0.1,0.7,0.3,0.3,0.7,0.5,0.5,0.2,0.8 };
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

void FuzzyData::muPrint() const
{
	cout << nameAlg << " - Matica prislusnosti mu " << name << ":" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << mu[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void FuzzyData::computeFromMu()
{
	computeMu();
	recalculateFromMu();
}

/*void FuzzyData::computeFromCenters()
{
	computeCenters();
	recalculateFromCenters();
}*/

void FuzzyData::normalizeMu()
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

void FuzzyData::randomMuInit()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = muGenerator->nextRandom();
		}
	}
	normalizeMu();
}

void FuzzyData::plusplusInit()
{	
	if (numberOfClusters > 0) {
		int which = ((centersGenerator->nextRandom() / centersGenerator->getMax()) * numberOfObjects);
		centers[0]->setValues(data[which].getValues());
		centers[0]->addToValues(minChange*0.1);
		int initCenters = 1;
		int i = 0;
		computeDFor(initCenters - 1, initCenters);
		double sum = sumDFor(initCenters);
		//centersPrint();
		while (initCenters < numberOfClusters) {	
			//dPrint();
			double prob = pow(getLowestDistance(i++ % numberOfObjects,initCenters),p)/sum;
			double randomProb = ((centersGenerator->nextRandom() / centersGenerator->getMax()));
			//cout << randomProb << "prob randomprob " << prob << endl;
			if (randomProb < prob) {
				centers[initCenters]->setValues(data[(i - 1) % numberOfObjects].getValues());
				centers[initCenters]->addToValues(minChange*0.1);
				initCenters++;
				computeDFor(initCenters - 1, initCenters);
				sum = sumDFor(initCenters);				
			}
			//centersPrint();
			//dPrint();
		}
		computeD();
		computeMu();
		//dPrint();
	}
}

void FuzzyData::recalculateFromMu()
{
	computeCenters();
	computeD();
	//centersPrint();
	//dPrint();
}

void FuzzyData::recalculateFromCenters()
{
	computeD();
	computeMu();
	recalculateFromMu();
}

void FuzzyData::printAsMatrix(double * matrix, const char * text) const
{
	cout << nameAlg << " - " << text << " " << name << ":" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << matrix[i*numberOfClusters+j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void FuzzyData::setMatrix(double ** source, double ** dest)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			dest[i][j] = source[i][j];
		}
	}
}

void FuzzyData::setMu(const double ** parmu)
{
	if (parmu) {
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				mu[i][j] = parmu[i][j];
			}
		}
	}
	recalculateFromMu();
}

void FuzzyData::setName(string pname)
{
	name = pname;
}

void FuzzyData::setAlgorithmName(string pname)
{
	nameAlg = pname;
}

void FuzzyData::centersInit()
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

void FuzzyData::centersPrint() const
{
	cout << nameAlg <<  " - Centra " << name << ":" << endl;
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum" << j + 1 << ": ";
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << centers[j]->getValue(k) << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void FuzzyData::computeCenters()
{
	vector<double> values;
	for (int j = 0; j < numberOfClusters; j++) {
		values.clear();
		for (int k = 0; k < numberOfCoordinates; k++) {
		//for (int k = 0; k < 1; k++) {
			double sum1 = 0;
			double sum2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				double product2 = 1;				
				product2 *= pow(mu[i][j],m);
				double product1 = product2 * data[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			values.push_back(sum1 / sum2);
		}
		*oldCenters[j]=*centers[j];
		centers[j]->setValues(values);
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordinates; k++) {
				if (abs(centers[j]->getValue(k) - data[i].getValue(k)) < 0.00000001) {
					centers[j]->setValue(k, centers[j]->getValue(k) + 0.1*minChange);
				}
			}
		}
	}
}

void FuzzyData::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			d[i][j] = INT_MAX;
		}
	}
}

void FuzzyData::dPrint() const
{
	cout << nameAlg << " - Matica euklidovskej vzdialenosti " << name << ":" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
	cout << endl;
}

void FuzzyData::computeMu()
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

//vypocet matice euklidovskej vzdialenosti
void FuzzyData::computeD()
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

void FuzzyData::computeDFor(int start, int end)
{
	if (start >= 0 && end <= numberOfClusters) {
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = start; j < end; j++) {
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
}

double FuzzyData::sumDFor(int end) const
{
	double sum = 0;
	for (int j = 0; j < end; j++) {
		for (int i = 0; i < numberOfObjects; i++) {
			if (d[i][j] > 0) {
				sum += pow(d[i][j], p);
			}
		}
	}
	return sum;
}

double FuzzyData::getLowestDistance(int whichObject, int whichMax) const
{
	double min = INT_MAX;
	for (int j = 0; j < whichMax; j++) {
		if (min>d[whichObject][j]) {
			min = d[whichObject][j];
		}
	}
	//cout << min << endl;
	return min;
}

double FuzzyData::getJm() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(mu[i][j], m) * pow(d[i][j], 2);
		}
	}
	return Jm;
}

void FuzzyData::printJm() const
{
	cout << nameAlg << " - Jm " << name << " = ";
	cout << getJm() << endl;
}

double FuzzyData::getFitness() const
{
	return K/getJm();
}

void FuzzyData::printFitness() const
{
	cout << nameAlg << " - Fitness " << name << " = ";
	cout << getFitness() << endl;
	cout << endl;
}

Dataset FuzzyData::getData() const
{
	return data;
}

int FuzzyData::getM() const
{
	return m;
}

int FuzzyData::getNumberOfClusters() const
{
	return numberOfClusters;
}

int FuzzyData::getNumberOfObjects() const
{
	return numberOfObjects;
}

int FuzzyData::getNumberOfCoordinates() const
{
	return numberOfCoordinates;
}

const double ** FuzzyData::getMu() const
{
	return (const double **)mu;
}

MuInitializationMode FuzzyData::getMuInitMode() const
{
	return muInitMode;
}

int FuzzyData::getK() const
{
	return K;
}

Object** FuzzyData::getCenters() const
{
	return centers;
}

double ** FuzzyData::getDistances() const
{
	return d;
}

//je zmena centier oproti centram v minulom kroku vyznamna
bool FuzzyData::wasSignificantChange(double pminchange) const
{
	if (pminchange <= 0) {
		pminchange = minChange;
	}
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			if ((abs(oldCenters[j]->getValue(k) - centers[j]->getValue(k))) > pminchange) {
				return true;
			}
		}
	}
	return false;
}

string FuzzyData::getAlgorithmName() const
{
	return nameAlg;
}
