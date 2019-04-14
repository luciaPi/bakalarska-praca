#include "Application.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include <string>
#include <vector>
#include "Object.h"
#include "Attribute.h"
#include "FCMcounter.h"
#include "PSOcounter.h"
#include "FCMPSOcounter.h"
#include "FAcounter.h"
#include "FAFCMcounter.h"

using namespace std;

Application::Application()
{
	srand(time(NULL));
}

Application::~Application()
{
	delete data;
	data = nullptr;
}

void Application::count(Algorithm alg, int numberOfItertion)
{
	if (data != nullptr && numberOfClusters > 0 && numberOfItertion > 0) {
		r1 = (double)rand() / RAND_MAX;
		r2 = (double)rand() / RAND_MAX;		
		alpha = (double)rand() / RAND_MAX;

		switch (alg) {
			case Algorithm::fcm: {
				FCMcounter fcmCounter;
				fcmCounter.setAlgorithmName("FCM");
				fcmCounter.setMaxIterations(numberOfItertion);
				fcmCounter.count(*data, numberOfClusters, m);
				fcmCounter.printJm();
				break;
			}
			case Algorithm::pso: {
				PSOcounter psoCounter;
				psoCounter.setAlgorithmName("PSO");
				psoCounter.setFinalCriterion(FinalCriterion::maxIteration);
				psoCounter.setMaxIterations(numberOfItertion);
				psoCounter.count(*data, numberOfClusters, m, c1, c2, r1, r2, w, Ppso);
				psoCounter.printJm();
				break;
			}	
			case Algorithm::fcmpso: {
				FCMPSOcounter fcmpso;
				fcmpso.setAlgorithmName("FCM-PSO");
				fcmpso.setNumberOfIterations(numberOfItertion);
				fcmpso.count(*data, numberOfClusters, m, c1, c2, r1, r2, w, Ppso);
				break;
			}
			case Algorithm::fa: {
				FAcounter faCounter;
				faCounter.setAlgorithmName("FA");
				faCounter.setMaxIterations(numberOfItertion);
				faCounter.setFinalCriterion(FinalCriterion::maxIteration);
				faCounter.count(*data, numberOfClusters, m, alpha, beta, gamma, Pfa);
				break;
			}
			case Algorithm::fafcm: {
				FAFCMcounter fafcm;
				fafcm.setAlgorithmName("FAFCM");
				fafcm.count(*data, numberOfClusters, m, alpha, beta, gamma, Pfa);
				break;
			}
		}	
	}
}

void Application::saveOutputToArff(const char * filename, char* title, char* creator, char* donor, char* relation, vector<Attribute*> attributes) const
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
		fprintf(datafile, "%c %s: ", shape, titles[which++]);
		fprintf(datafile, "%s\n%c\n", title, shape);
		fprintf(datafile, "%c %s:\n", shape, titles[which++]);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++], creator);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++], donor);
		fprintf(datafile, "%c\t%s: %d\-%d-%d\n", shape, titles[which++], date->tm_mday, date->tm_mon, date->tm_year + 1900);
		fprintf(datafile, "%c\n\n", shape);
		fprintf(datafile, "%s %s\n\n", titles[which++], relation);


		for (int i = 0; i < attributes.size(); i++) {
			const char* actual = attributes[i]->getName();
			fprintf(datafile, "%s %s ", titles[which], attributes[i]->getName());
			fprintf(datafile, "%s\n", attributes[i]->getTypes());
		}
		fprintf(datafile, "\n%s\n", titles[++which]);

		fclose(datafile);
	}
}

//nacitanie vstupnych dat
bool Application::setData(const char* fileName)
{
	FILE* datafile;
	if ((datafile = fopen(fileName, "r")) == NULL) {
		//cout << "CHYBA" << endl;
		return false;
	}
	else {
		data = new Dataset();

		vector<double> values;
		float value;
		int returnVal;
		Object *flower = new Object();
		int lastPosition;
		char name[100];
		int countChars = 0;
		int countNumbers = 0;
		int numAttributes = 0;

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
				flower = new Object();
				values.clear();*/
				do {
					fscanf(datafile, "%c", &name);
					countChars++;
					countNumbers = 0;
				} while (*name != ',' && *name != '\n');
			}
			else {
				//fscanf(datafile, "%c",&name);
				if ((countChars > 2 && values.size() > 0) || countNumbers == 1) {
					flower->setValues(values);
					data->add(flower);
					flower = new Object();
					values.clear();
					countNumbers = 0;
					numAttributes = 0;
				}
				//if (numAttributes > 0) {
					values.push_back(value);
				//}
				numAttributes++;
				countChars = 0;
				countNumbers = 1;
			}
		} while (true);
		flower->setValues(values);
		data->add(flower);

		fclose(datafile);
	}
	return true;
}

bool Application::setNumberOfClusters(int number)
{
	if (number > 0) {
		numberOfClusters = number;
		return true;
	}
	return false;
}

//vypis vstupnych dat
void Application::dataObjectsPrint() const
{
	cout << "Data objects:" << endl;
	int numberOfCoordiantes;
	int numberOfObjects;
	if ((numberOfObjects = data->getSize()) > 0) {
		numberOfCoordiantes = (*data)[0].getNumberOfCoordinates();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordiantes; k++) {
				cout << (*data)[i].getValue(k) << (k < numberOfCoordiantes - 1 ? "," : " ");
			}
			cout << endl;
		}
	}
}