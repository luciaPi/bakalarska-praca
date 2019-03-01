#include "FCMcounter.h"
#include <iostream>
#include <time.h>
#include <ctime>

FCMcounter::FCMcounter()
{
	m = 2;
	numberOfClusters = 3;
	minChange = 0.1;	
}


FCMcounter::~FCMcounter()
{
	while (all.size() > 0) {
		Flower* last = all.back();
		delete last;
		all.pop_back();
	}

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
void FCMcounter::count(const char* name)
{
	if (readDataFromFile(name)) {
		numberOfObjects = all.size();
		flowersPrint();
		muInit();
		muPrint();
		dInit();
		centersInit();

		computeCenters();
		centersPrint();

		int i = 0;
		do {
			cout << "Round" << i << endl;
			computeD();
			dPrint();

			computeMu();
			muPrint();

			computeCenters();
			centersPrint();

			i++;
		} while (isSignificantChange());
		dPrint();
		flowersPrintWithType();
		centersPrint();
		//muPrint();

	}
}

//nacitanie vstupnych dat
bool FCMcounter::readDataFromFile(const char* fileName)
{
	FILE* datafile;
	if ((datafile = fopen(fileName, "r")) == NULL) {
 		cout << "CHYBA" << endl;
		return false;
	}
	else {
		vector<double> values;
		float value;
		int returnVal;
		Flower *flower = new Flower();
		int lastPosition;
		char name[100];
		int counChars = 0;

		do {
			//lastPosition = ftell(datafile);
			if ((returnVal = fscanf(datafile, "%f", &value)) == EOF) {
				break;
			}			
			if (returnVal == 0) {
				//fseek(datafile, lastPosition, SEEK_SET);				
				/*fscanf(datafile, "%s", &name);
				flower->setName(name);
				flower->setValues(values);
				numberOfCoordinates = values.size();
				all.push_back(flower);
				flower = new Flower();
				values.clear();*/
				fscanf(datafile, "%c", &name);
				counChars++;				
			}
			else {
				//fscanf(datafile, "%c",&name);
				if (counChars > 2) {
					flower->setValues(values);
					all.push_back(flower);
					flower = new Flower();
					values.clear();
				}
				values.push_back(value);
				counChars = 0;
			}
		} while (true);
		flower->setValues(values);
		numberOfCoordinates = values.size();
		all.push_back(flower);

		fclose(datafile);
	}
	return true;
}

//vypis vstupnych dat
void FCMcounter::flowersPrint()
{
	cout << "Data:" << endl;
	for (Flower *flower : all) {		
		for (int k = 0; k < numberOfCoordinates;k++) {
			cout << flower->getValue(k) << (k < numberOfCoordinates-1 ? "," : " ");
		}
		cout << flower->getName() << endl;
	}
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
void FCMcounter::muPrint()
{
	cout << "Matica prislusnosti mu:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << mu[i][j] << " ";
		}
		cout << endl;
	}
}

//vypis dat spolu s priradenym zhlukom
void FCMcounter::flowersPrintWithType()
{
	cout << "Vysledok:" << endl;
	for (Flower *flower : all) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << flower->getValue(k) << (k == numberOfCoordinates - 1 ? " Zhluk: " : ",");
		}
		cout << whichCenter(*flower) << endl;
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
	centers = new Flower*[numberOfClusters];
	oldCenters = new Flower*[numberOfClusters];
	for (int j = 0; j < numberOfClusters; j++) {		
		Flower *flower = new Flower();
		centers[j] = flower;
		flower = new Flower();
		oldCenters[j] = flower;
	}
}

//vypis centier
void FCMcounter::centersPrint()
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
				double product1 = product2 * all[i]->getValue(k);
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
				double a2 = all[i]->getValue(k);
				if (abs(centers[j]->getValue(k) - all[i]->getValue(k)) < 0.00000001) {
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
				double coordinate1 = all[i]->getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			d[i][j] = sqrt(sum);
		}
	}
}

//je zmena centier oproti centram v minulom kroku vyznamna
bool FCMcounter::isSignificantChange()
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
int FCMcounter::whichCenter(Flower &flower)
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
int FCMcounter::whichNumberOfObject(Flower & flower)
{
	int which = 0;
	for (Flower *actual : all) {
		if (actual == &flower) {
			return which;
		}
		which++;
	}
}

void FCMcounter::saveOutputToArff(const char * filename, char* title, char* creator, char* donor, char* relation)
{
	FILE* datafile;
	if ((datafile = fopen(filename, "w")) == NULL) {
		cout << "CHYBA" << endl;
	}
	else {
		char* titles[] = {
		"1. Title",
		"2. Sources",
		"(a) Creator",
		"(b) Donor",
		"(c) Date",
		"@RELATION",
		"@ATTRIBUTE",
		"@DATA"
		};
		char shape = '%';
		time_t now = time(0);
		tm* date = localtime(&now);

		int which = 0;
		fprintf(datafile, "%c %s: ", shape,titles[which++]);
		fprintf(datafile, "%s\n%c\n", title, shape);
		fprintf(datafile, "%c %s:\n", shape, titles[which++]);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++], creator);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++],donor);
		fprintf(datafile, "%c\t%s: %d\-%d-%d\n", shape, titles[which++],date->tm_mday,date->tm_mon,date->tm_year);
		fprintf(datafile, "%c\n\n", shape);
		fprintf(datafile, "%s %s\n", titles[which++],relation);

		/*vector<double> values;
		float value;
		int returnVal;
		Flower *flower = new Flower();
		int lastPosition;
		char name[100];
		int counChars = 0;

		do {
			//lastPosition = ftell(datafile);
			if ((returnVal = fscanf(datafile, "%f", &value)) == EOF) {
				break;
			}
			if (returnVal == 0) {
				//fseek(datafile, lastPosition, SEEK_SET);				
				/*fscanf(datafile, "%s", &name);
				flower->setName(name);
				flower->setValues(values);
				numberOfCoordinates = values.size();
				all.push_back(flower);
				flower = new Flower();
				values.clear();*//*
				fscanf(datafile, "%c", &name);
				counChars++;
			}
			else {
				//fscanf(datafile, "%c",&name);
				if (counChars > 2) {
					flower->setValues(values);
					all.push_back(flower);
					flower = new Flower();
					values.clear();
				}
				values.push_back(value);
				counChars = 0;
			}
		} while (true);/*
		flower->setValues(values);
		numberOfCoordinates = values.size();
		all.push_back(flower);*/

		fclose(datafile);
	}
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
void FCMcounter::dPrint()
{
	cout << "Matica euklidovskej vzdialenosti:" << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}
