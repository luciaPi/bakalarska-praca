#include "Application.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
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
	srand(time(0));
	parameterGenerator = RandomGenerator(rd(), 0, 1);
}

Application::~Application()
{
	delete data;
	data = nullptr;
}

void Application::count(Algorithm alg, int numberOfItertion)
{
	if (alpha < 0) {
		alpha = parameterGenerator.nextRandom();
	}
	if (r1 < 0) {
		r1 = parameterGenerator.nextRandom();
	}
	if (r2 < 0) {
		r2 = parameterGenerator.nextRandom();
	}
	if (data != nullptr && numberOfClusters > 0 && numberOfItertion > 0) {

		switch (alg) {
			case Algorithm::fcm: {
				FCMcounter fcmCounter(rd(),rd());
				fcmCounter.setFinalCriterion(FinalCriterion::minChange);
				fcmCounter.setAlgorithmName("FCM");
				fcmCounter.setMaxIterations(numberOfItertion);

				fcmCounter.setCounter(*data, numberOfClusters, m, K, muInitMode);
				count(&fcmCounter);
				break;
			}
			case Algorithm::pso: {
				PSOcounter psoCounter(rd(), rd(), rd());
				psoCounter.setAlgorithmName("PSO");
				psoCounter.setFinalCriterion(FinalCriterion::maxIteration);
				psoCounter.setMaxIterations(numberOfItertion);				

				psoCounter.setCounter(*data, numberOfClusters, m,c1, c2, r1, r2, w, Ppso, K, muInitMode);
				count(&psoCounter);
				break;
			}	
			case Algorithm::fcmpso: {
				FCMPSOcounter fcmpsoCounter(rd(), rd(), rd(),rd(), rd());
				fcmpsoCounter.setAlgorithmName("FCM-PSO");
				fcmpsoCounter.setNumberOfIterations(numberOfItertion);

				fcmpsoCounter.setCounter(*data, numberOfClusters, m, c1, c2, r1, r2, w, Ppso, K, muInitMode);
				count(&fcmpsoCounter);
				break;
			}
			case Algorithm::fa: {
				FAcounter faCounter(rd(), rd(), rd(), rd());
				faCounter.setAlgorithmName("FA");
				faCounter.setMaxIterations(numberOfItertion);
				faCounter.setFinalCriterion(FinalCriterion::maxIteration);

				faCounter.setCounter(*data, numberOfClusters, m, alpha, beta, gamma, Pfa, K, muInitMode);
				count(&faCounter);
				break;
			}
			case Algorithm::fafcm: {
				FAFCMcounter fafcmCounter(rd(), rd(), rd(), rd(),rd(), rd());
				fafcmCounter.setAlgorithmName("FAFCM");

				fafcmCounter.setCounter(*data, numberOfClusters, m, alpha, beta, gamma, Pfa, K, muInitMode);
				count(&fafcmCounter);
				break;
			}
		}	
	}
}

bool Application::setMuInitializationMode(MuInitializationMode mode)
{
	muInitMode = mode;
	return true;
}

void Application::saveOutputToArff(const FuzzyData* fuzzyData, const char * filename, char* title, vector<Attribute*> attributes) const
{
	FILE* datafile;
	if ((datafile = fopen(filename, "w")) == NULL && data != nullptr) {
		cout << "CHYBA" << endl;
	}
	else {
		char* titles[] = {
		"Title",
		"Centers",
		"@RELATION",
		"@ATTRIBUTE",
		"@DATA"
		};
		char comment = '%';
		time_t now = time(0);
		tm* date = localtime(&now);

		vector<string> clusters;
		for (int j = 0; j < numberOfClusters; j++) {
			clusters.push_back("Zhluk"+to_string(j));
		}

		//title
		int which = 0;
		fprintf(datafile, "%c %s: ", comment, titles[which++]);
		fprintf(datafile, "%s\n%c\n", title, comment);
		//centers
		fprintf(datafile, "%c %s:\n", comment, titles[which++]);

		const Object** centers = fuzzyData->getCenters();
		for (int j = 0; j < numberOfClusters; j++) {
			fprintf(datafile, "%c \t", comment);
			for (int k = 0; k < (*data)[0].getNumberOfCoordinates(); k++) {
				fprintf(datafile, "%f ", centers[j]->getValue(k));
			}
			fprintf(datafile, "\n", comment);
		}		
		//fprintf(datafile, "%c\t%s: %d\-%d-%d\n", comment, titles[which++], date->tm_mday, date->tm_mon, date->tm_year + 1900);
		fprintf(datafile, "%c\n\n", comment);
		fprintf(datafile, "%s\n\n", titles[which++]);

		//attributes
		for (int i = 0; i < attributes.size(); i++) {
			string actual = attributes[i]->getName();
			fprintf(datafile, "%s %s \t REAL\n", titles[which], actual.c_str());
		}
		//mu
		fprintf(datafile, "%s MU \t FUZZY {", titles[which]);
		for (string actual : clusters) {
			if (actual != clusters[0]) {
				fprintf(datafile, ", ");
			}
			fprintf(datafile, "%s", actual.c_str());
		}
		fprintf(datafile, "}\n");
		//class
		fprintf(datafile, "%s class \t {", titles[which]);
		for (string actual : clusters) {
			if (actual != clusters[0]) {
				fprintf(datafile, ", ");
			}
			fprintf(datafile, "%s", actual.c_str());
		}
		fprintf(datafile, "}\n");		
		//data
		const double** mu = fuzzyData->getMu();
		fprintf(datafile, "\n%s\n", titles[++which]);
		for (int i = 0; i < data->getSize(); i++) {
			for (int k = 0; k < (*data)[i].getNumberOfCoordinates(); k++) {
				fprintf(datafile, "%.2f, ", (*data)[i].getValue(k));
			}
			for (int j = 0; j < numberOfClusters; j++) {
				fprintf(datafile, "%f, ", mu[i][j]);
			}
			fprintf(datafile, "%s\n", ((*data)[i].getName()).c_str());
		}

		fclose(datafile);
	}
}

void Application::saveArff(const FuzzyData* fuzzyData)
{
	char* title = "Iris";

	/*vector<string> type;
	type.push_back("numeric");
	vector<string> type2;
	type2.push_back("Iris - setosa");
	type2.push_back("Iris - versicolor");
	type2.push_back("Iris - virginica");*/
	vector<Attribute*> attributes;
	/*attributes[0].setValues("sepallength", type);
	attributes[1].setValues("sepalwidth", type);
	attributes[2].setValues("petallength", type);
	attributes[3].setValues("petalwidth", type);
	attributes[4].setValues("class", type2);*/
	/*attributes.push_back(new Attribute("sepallength"));
	attributes.push_back(new Attribute("sepalwidth"));
	attributes.push_back(new Attribute("petallength"));
	attributes.push_back(new Attribute("petalwidth"));*/
	for (int k = 0; k < (*data)[0].getNumberOfCoordinates(); k++) {
		attributes.push_back(new Attribute("Attribute"+to_string(k)));
	}

	saveOutputToArff(fuzzyData,"wine.txt", title, attributes);

	while (attributes.size() > 0) {
		Attribute* last = attributes.back();
		delete last;
		attributes.pop_back();
	}
}

bool Application::setData(const char* fileName)
{
	ifstream myFile;
	myFile.open(fileName);
	if (!myFile.is_open()) {
		return false;
	}
	data = new Dataset();
	vector<double> values;
	Object *flower = new Object();

	char nextChar;
	float value;
	int countChars = 0;
	//string name = "";
	string line = "";

	int next = myFile.peek();
	while (next != EOF) {
		values.clear();
		getline(myFile, line);
		stringstream ss;
		ss << line;
		string word = "";
		double value;

		while (!ss.eof()) {
			getline(ss, word, ',');
			if (stringstream(word) >> value) {
				values.push_back(value);
			}
			else {
				flower->setName(word);
			}
			word = "";
		}
		if (next != 37 && values.size() > 0) {
			flower->setValues(values);
			data->add(flower);
			flower = new Object();
			values.clear();
		}

		next = myFile.peek();

		/*if (isdigit(next)) {
			myFile >> value;
			values.push_back(value);
		}
		else {
			myFile >> nextChar;
			if (nextChar == '\n') {
				flower->setValues(values);
				flower->setName(name);
				data->add(flower);
				flower = new Object();
				values.clear();
			}
			if (next == '%') {
				getline(myFile,name);
				//string line;
				//myFile >> line;
				/*while (nextChar != '\0') {
					myFile >> nextChar;
				}

			}*/
		
	}
	delete flower;

	myFile.close();

	return true;
}

//nacitanie vstupnych dat
/*bool Application::setData(const char* fileName)
{
	FILE* datafile;
	if ((datafile = fopen(fileName, "r")) == NULL) {
		cout << "CHYBA" << endl;
		return false;
	}
	else {
		data = new Dataset();

		vector<double> values;
		float value;
		int returnVal;
		Object *flower = new Object();
		int lastPosition;
		char oneChar[1];
		string name = "";
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
				values.clear();*//*
				name = "";
				do {
					fscanf(datafile, "%c", &oneChar);
					name += oneChar[0];
					countChars++;
					countNumbers = 0;
				} while (*oneChar != ',' && *oneChar != '\n');
			}
			else {
				//fscanf(datafile, "%c",&name);
				if ((countChars > 2 && values.size() > 0) || countNumbers == 1) {
					flower->setValues(values);
					name = name.substr(0, name.size() - 1);
					flower->setName(name);
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
		name = name.substr(0, name.size() - 1);
		flower->setName(name);
		flower->setValues(values);
		data->add(flower);

		fclose(datafile);
	}
	return true;
}*/

bool Application::setNumberOfClusters(int number)
{
	if (number > 0) {
		numberOfClusters = number;
		return true;
	}
	return false;
}

bool Application::setNumberOfReplications(int number)
{
	if (number > 0) {
		numberOfReplications = number;
		return true;
	}
	return false;
}

bool Application::setConfidenceInterval(int value)
{
	switch (value) {
		case 95: {
			actualT = t95;
			return true;
		}
		case 99: {
			actualT = t99;
			return true;
		}
		case 90: {
			actualT = t90;
			return true;
		}
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

void Application::count(Counter * counter)
{
	dataObjectsPrint();

	double sum = 0;
	double sumSquared = 0;
	for (int i = 0; i < numberOfReplications; i++) {
		counter->recount();
		counter->printJm();

		const FuzzyData* best = counter->getBest();
		saveArff(best);

		sum += counter->getJm();
		sumSquared += pow(counter->getJm(),2);
	}
	double average = sum / numberOfReplications;
	cout << counter->getAlgorithmName() << " - Stredna hodnota: " << average << endl;
	if (numberOfReplications >= 30) {
		double s = sqrt(abs((sumSquared / (numberOfReplications - 1)) - pow(sum / (numberOfReplications - 1), 2)));
		double interval = (s * actualT) / sqrt(numberOfReplications);
		cout << counter->getAlgorithmName() << " - Interval spolahlivosti: " << average << " +- " << interval << endl;
	}
	cout << endl;
}
