#include "FCMcounter.h"
#include <iostream>

FCMcounter::FCMcounter()
{
	m = 2;
	numberOfClusters = 3;
	minChange = 0.00001;	
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
	}
	delete[] centers;

	for (int j = 0; j < numberOfClusters; j++) {
		delete oldCenters[j];
	}
	delete[] oldCenters;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] mu[i];
	}
	delete[] mu;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] d[i];
	}
	delete[] d;
}

void FCMcounter::count(const char* name)
{
	if (readDataFromFile(name)) {
		numberOfObjects = all.size();
		flowersPrint();
		muInit();
		dInit();
		centersInit();

		computeCenters();
		//centersPrint();

		int i = 0;
		do {
			cout << "Round" << i << endl;
			computeD();
			//dPrint();

			computeMu();
			//muPrint();

			computeCenters();
			//centersPrint();

			i++;
		} while (isSignificantChange());
		dPrint();
		flowersPrintWithType();
		centersPrint();
		muPrint();
	}
}

bool FCMcounter::readDataFromFile(const char* fileName)
{
	FILE* datafile;
	if ((datafile = fopen(fileName, "r")) == NULL) {
 		cout << "CHYBA" << endl;
		return false;
	}
	else {
		vector<float> values;
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
					//numberOfCoordinates = values.size();
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

void FCMcounter::flowersPrint()
{
	cout << "Data:" << endl;
	for (Flower *flower : all) {		
		for (int i = 0; i < numberOfCoordinates;i++) {
			cout << flower->getValue(i) << ",";
		}
		cout << flower->getName() << endl;
	}
}

void FCMcounter::muInit()
{
	int which = 0;
	mu = new float*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new float[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			if (which%numberOfClusters == 0)
				mu[i][j] = 1;
			else
				mu[i][j] = 0;
			which++;
		}
		which++;			
	}
	/*float values[] = { 0.15, 0.45, 0.4, 0,0.5,0.5,0.25,0.75,0,1,0,0 ,0.25,0.75,0 };
	int which = 0;
	mu = new float*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		mu[i] = new float[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = values[which];
			which++;
		}
	}*/
}

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

void FCMcounter::flowersPrintWithType()
{
	cout << "Vysledok:" << endl;
	for (Flower *flower : all) {
		for (int i = 0; i < numberOfCoordinates; i++) {
			cout << flower->getValue(i) << (i == numberOfCoordinates - 1 ? " Zhluk: " : ",");
		}
		cout << whichCenter(*flower) << endl;
	}
}

void FCMcounter::computeMu()
{
	float index = 2 / (m - 1);
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			float sucet = 0;
			for (int k = 0; k < numberOfClusters; k++) {
				float podiel = d[i][j] / d[i][k];
				float sucin = 1;
				for (int l = 0; l < index; l++) { // chyba
					sucin *= podiel;
				}
				sucet += sucin;
			}
			mu[i][j] = 1 / sucet;
		}
	}
}

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

void FCMcounter::computeCenters()
{
	vector<float> values;
	for (int j = 0; j < numberOfClusters; j++) {
		values.clear();
		for (int k = 0; k < numberOfCoordinates; k++) {
			float sucet1 = 0;
			float sucet2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				float sucin1;
				float sucin2 = 1;
				for (int k = 0; k < m; k++) {
					sucin2 *= mu[i][j];
				}
				sucin1 = sucin2 * all[i]->getValue(k);
				sucet1 += sucin1;
				sucet2 += sucin2;
			}
			values.push_back(sucet1 / sucet2);
		}
		oldCenters[j]->setValues(centers[j]->getValues());
		centers[j]->setValues(values);
	}
}

void FCMcounter::computeD()
{	
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sucet = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double sur1 = all[i]->getValue(k);
				double sur2 = centers[j]->getValue(k);
				sucet += pow(sur1 - sur2, 2);
			}
			d[i][j] = sqrt(sucet);
		}
	}
}

bool FCMcounter::isSignificantChange()
{
	for (int i = 0; i < numberOfClusters; i++) {
		for (int j = 0; j < numberOfCoordinates;j++) {
			if ((abs(oldCenters[i]->getValue(j) - centers[i]->getValue(j))) > minChange) {
				return true;
			}
		}
	}
	return false;
}

int FCMcounter::whichCenter(Flower &flower)
{
	int whichObject = whichNumberOfObject(flower);

	float max = 0;
	int maxCoordinate = -1;
	if (whichObject >= 0) {
		for (int i = 0; i < numberOfClusters; i++) {
			if (mu[whichObject][i] > max) {
				max = mu[whichObject][i];
				maxCoordinate = i;
			}
		}
	}
	return maxCoordinate;
}

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

void FCMcounter::dInit()
{
	d = new float*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new float[numberOfClusters];
	}
}

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
