#include "CVI.h"
#include <math.h>  
#include <iostream>

using namespace std;

auto delimit = [](ostream& output) { output << "|"; };

double CVI::getMinCentersDistance() const
{
	double min = INT_MAX;
	for (int i = 0; i < numberOfClusters; i++) {
		for (int j = i+1; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = centers[i]->getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			double distance = sqrt(sum);
			distance = pow(distance, 2);
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
		for (int j = i+1; j < numberOfClusters; j++) {
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

Object* CVI::getMainCenter()
{		
	fcmCounter.count(data,1, fuzzydata->getM(), fuzzydata->getK(), fuzzydata->getMuInitMode());
	FuzzyData* forOneCluster = fcmCounter.getBest();
	return *(forOneCluster->getCenters());
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
	for (int j = 0; j < numberOfClusters; j++) {
		double sum = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			sum = sum + pow(mu[i][j], 2);
		}
		//fuzzydata->muPrint();
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
	delete[]meanCenters;
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
	if (number == 0) {
		number++;
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
		sum1 += pow(mu[i][which], m);
	}

	for (int k = 0; k < numberOfCoordinates; k++) {
		for (int kk = 0; kk < numberOfCoordinates; kk++) {
			F[k][kk] = 0;
		}
	}
	
	for (int i = 0; i < numberOfObjects; i++) {
		vector<double> distance;
		for (int k = 0; k < numberOfCoordinates; k++) {
			distance.push_back(data[i].getValue(k)- centers[which]->getValue(k));
		}
		for (int k = 0; k < numberOfCoordinates; k++) {
			double coordinate1 = distance[k];
			for (int kk = 0; kk < numberOfCoordinates; kk++) {
				double coordinate2 = distance[kk];
				F[k][kk] += coordinate1 * coordinate2*pow(mu[i][which],m);
			}
		}
	}
	
	for (int k = 0; k < numberOfCoordinates; k++) {
		for (int kk = 0; kk < numberOfCoordinates; kk++) {
			F[k][kk] = F[k][kk] / sum1;
		}
	}
	//vypis
	/*for (int k = 0; k < numberOfCoordinates; k++) {
		for (int kk = 0; kk < numberOfCoordinates; kk++) {
			cout << F[k][kk] << " ";
		}
		cout << endl;
	}*/
	return F;
}

double CVI::getDeterminant(double ** F)
{
	int signum = 1;
	for (int i = 0; i < numberOfCoordinates; i++) {
		double max = F[i][i];
		int index = i;
		for (int x = i; x < numberOfCoordinates; x++) {
			if (F[x][i] > max) {
				max = F[x][i];
				index = x;
			}
		}
		if (index != i) {
			signum *= -1;
		}
		for (int x = 0; x < numberOfCoordinates; x++) {
			double pom = F[index][x];
			F[index][x] = F[i][x];
			F[i][x] = pom;
		}
		for (int j = i+1; j < numberOfCoordinates; j++) {
			double xx = F[j][i]/F[i][i];
			for (int k = 0; k < numberOfCoordinates; k++) {
				F[j][k] = F[j][k]-(xx*F[i][k]);
			}
		}
	}
	double det = signum;
	for (int i = 0; i < numberOfCoordinates; i++) {
		det *= F[i][i];
	}
	return det;
}

CVI::CVI()
{	
	setIndexNames();
}

CVI::CVI(int seed1, int seed2)
{
	setIndexNames();
	fcmCounter = FCMcounter(seed1, seed2);
}

bool CVI::count(FuzzyData* pdata, Dataset pdataset)
{
	if (pdata != nullptr) {
		fuzzydata = pdata;

		numberOfObjects = fuzzydata->getNumberOfObjects();
		numberOfClusters = fuzzydata->getNumberOfClusters();
		numberOfCoordinates = fuzzydata->getNumberOfCoordinates();
		centers = fuzzydata->getCenters();
		mu = fuzzydata->getMu();
		data = pdataset;

		int i = 0;
		indices[i++] ? countPC() : -1;
		indices[i++] ? countMPC() : -1;
		indices[i++] ? countPE() : -1;
		indices[i++] ? countFS() : -1;
		indices[i++] ? countXIEBENI() : -1;
		indices[i++] ? countKWON() : -1;
		indices[i++] ? countFHV() : -1;
		indices[i++] ? countPBMF() : -1;
		indices[i++] ? countPCAES() : -1;
		indices[i++] ? countSILHOUETTE() : -1;
		return true;
	}
	return false;
}

void CVI::resetCVI()
{
	for (Index& actual : indices) {
		actual.setToCount();
	}
}

void CVI::setIndexToCount(IndexEnum index)
{
	indices[index] = true;
}

void CVI::unsetIndexToCount(IndexEnum index)
{
	indices[index] = false;
}

void CVI::setAllIndecesToCount()
{
	for (int i = 0; i < number; i++) {
		indices[i] = true;
	}
}

void CVI::unsetAllIndecesToCount()
{
	for (int i = 0; i < number; i++) {
		indices[i] = false;
	}
}

//header
void CVI::printResultsHeader(ostream& output) const
{	
	bool isConsole = &output == &cout ? true : false;
	int widthCVIactual = isConsole ? widthCVIConsole : widthCVIFile;

	output << internal << "   Dataset   ";
	delimit(output);
	output << internal << "Pocet zhlukov";
	delimit(output);
	output << internal << "Algoritmus";
	delimit(output);
	delimit(output);
	for (int i = 0; i < number; i++) {
		if (indices[i]) {
			output.width(widthCVIactual);
			string name = indices[i];
			output << name;
			delimit(output);
		}
	}
	delimit(output);
	//output << "  Zhluk" << endl;
	output << endl;
}

//data
void CVI::printResults(ostream & output, string title, double t) const
{
	bool isConsole = &output == &cout ? true : false; 
	int widthCVIactual = isConsole ? widthCVIConsole : widthCVIFile;

	output.width(titleSize);
	output << title;
	delimit(output);

	output.width(titleSize);
	output << numberOfClusters;
	delimit(output);

	output.width(algoritmusSize);
	output << fuzzydata->getAlgorithmName();
	delimit(output);
	delimit(output);

	for (int i = 0; i < number; i++) {
		if (indices[i]) {			
			output.width(widthCVIactual);
			output << indices[i].getResult(t);
			delimit(output);
		}
	}
	delimit(output);
	output <<  endl;
}

void CVI::setAssignedClusters(vector<Attribute*> pclusters)
{
	clusters = pclusters;
}

void CVI::setIndexNames()
{
	indices.clear();
	indices.push_back(Index("PC"));
	indices.push_back(Index("MPC"));
	indices.push_back(Index("PE"));
	indices.push_back(Index("FS"));
	indices.push_back(Index("XIEBENI"));
	indices.push_back(Index("KWON"));
	indices.push_back(Index("FHV"));
	indices.push_back(Index("PBMF"));
	indices.push_back(Index("PCAES"));
	indices.push_back(Index("SILHOUETTE"));
}

double CVI::countPC()
{
	if (fuzzydata != nullptr) {
		double sum = 0;
		const double** mu = fuzzydata->getMu();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				sum += pow(mu[i][j], 2);
			}
		}
		indices[PC] = sum / numberOfObjects;		
	}
	else {
		indices[PC] = -1;
	}
	return indices[PC];
}

double CVI::countPE()
{
	if (fuzzydata != nullptr) {
		double sum = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				sum += mu[i][j] * log(mu[i][j]);
			}
		}
		indices[PE] = (-sum) / numberOfObjects;
	}
	else {
		indices[PE] = -1;
	}
	return indices[PE];
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
		indices[FS] = Jm - Km;
	}
	else {
		indices[FS] = -1;
	}
	return indices[FS];
}

double CVI::countXIEBENI()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();		
		double min = getMinCentersDistance();

		indices[XIEBENI] = Jm/(numberOfObjects*min);
	}
	else {
		indices[XIEBENI] = -1;
	}
	return indices[XIEBENI];
}

double CVI::countKWON()
{
	if (fuzzydata != nullptr) {
		double Jm = 0;
		double** d = fuzzydata->getDistances();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int j = 0; j < numberOfClusters; j++) {
				Jm += pow(mu[i][j], 2) * pow(d[i][j], 2);
			}
		}
		
		double min = getMinCentersDistance();

		double* meanObjects = new double[numberOfCoordinates];
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum = 0;
			for (int i = 0; i < numberOfClusters; i++) {
				sum += centers[i]->getValue(k);
			}
			meanObjects[k] = sum / numberOfClusters;
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
		double xx = sum / numberOfClusters;
		delete[] meanObjects;

		indices[KWON] = (Jm + xx) / min;
	}
	else {
		indices[KWON] = -1;
	}
	return indices[KWON];
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
		indices[FHV] = sum;

		for (int k = 0; k < numberOfCoordinates; k++) {
			delete[] F[k];
		}
		delete[] F;
	}
	else {
		indices[FHV] = -1;
	}
	return indices[FHV];
}

double CVI::countPBMF()
{
	if (fuzzydata != nullptr) {
		double Jm = fuzzydata->getJm();

		Object* mainCenter = getMainCenter();
		double E1 = 0;
		for (int i = 0; i < numberOfObjects; i++) {
			double distance = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = data[i].getValue(k);
				double coordinate2 = mainCenter->getValue(k);
				distance += pow(coordinate1 - coordinate2, 2);
			}
			distance = sqrt(distance);
			E1 += distance;
		}
		double Dc = getMaxCentersDistance();
		//double JmDif = getJmDifferent();

		indices[PBMF] = pow((E1*Dc)/(Jm* numberOfClusters),2);
	}
	else {
		indices[PBMF] = -1;
	}
	return indices[PBMF];
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
		indices[PCAES] = sum1 - sum2;;
	}
	else {
		indices[PCAES] = -1;
	}
	return indices[PCAES];
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
		indices[SILHOUETTE] = sum / numberOfObjects;
	}
	else {
		indices[SILHOUETTE] = -1;
	}
	return indices[SILHOUETTE];
}

double CVI::countMPC()
{
	if (fuzzydata != nullptr) {
		double PCval = indices[PC];
		if (PCval == -1) {
			PCval = countPC();
		}
		double xx = numberOfClusters - 1;
		indices[MPC] = 1-((numberOfClusters/(xx))*(1-PCval));
	}
	else {
		indices[MPC] = -1;
	}
	return indices[MPC];
}

