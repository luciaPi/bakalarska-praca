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
#include <direct.h>

using namespace std;

Application::Application()
{
	srand(time(0));
	parameterGenerator = RandomGenerator(rd(), 0, 1);
	cvi = CVI(rd(), rd());
}

Application::~Application()
{
	delete data;
	data = nullptr;
	
	clearObjectClasses();
	clearClusterAttributes();
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

bool Application::setTypeOfOutput(OutputType type)
{
	switch (type) {
		case OutputType::txt : {
			outputExtesion = "txt";
			return true;
		}
		case OutputType::arff : {
			outputExtesion = "arff";
			return true;
		}
	}
	return false;
}

void Application::setFileOutputMode()
{
	fileOutputMode = true;
}

void Application::unsetFileOutputMode()
{
	fileOutputMode = false;
}

void Application::saveToArff(const FuzzyData* fuzzyData, const char * filename, vector<Attribute*> attributes) const
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
		for (Attribute* actual : clusters) {
			if (actual != clusters[0]) {
				fprintf(datafile, ", ");
			}
			fprintf(datafile, "%s", (actual->getName).c_str());
		}
		fprintf(datafile, "}\n");
		//class
		fprintf(datafile, "%s class \t {", titles[which]);
		for (Attribute *actual : objectClasses) {
			if (actual != objectClasses[0]) {
				fprintf(datafile, ", ");
			}
			fprintf(datafile, "%s", (actual->getName()).c_str());
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

void Application::saveResultToFile(const FuzzyData* fuzzyData, int which, string resultPath)
{
	string titleString = title;
	vector<Attribute*> attributes;

	for (int k = 0; k < (*data)[0].getNumberOfCoordinates(); k++) {
		attributes.push_back(new Attribute("Attribute"+to_string(k)));
	}
		
	string filename = titleString + "(" + to_string(which) + ")." + outputExtesion;
	string filenameWithPath = resultPath + "/" + filename;
	saveToArff(fuzzyData, filenameWithPath.c_str(), attributes);

	while (attributes.size() > 0) {
		Attribute* last = attributes.back();
		delete last;
		attributes.pop_back();
	}
}

void Application::assignClusters(FuzzyData* fuzzyData)
{
	for (int i = 0; i < data->getSize(); i++) {
		int whichCluster = whichCenter(i,fuzzyData);
		(*data)[i].setObjectAssignedClass(clusters[whichCluster]);
	}
}
void Application::clearObjectClasses()
{
	while (objectClasses.size() > 0) {
		Attribute* last = objectClasses.back();
		delete last;
		objectClasses.pop_back();
	}
}
void Application::clearClusterAttributes()
{
	while (clusters.size() > 0) {
		Attribute* last = clusters.back();
		delete last;
		clusters.pop_back();
	}
}
int Application::whichCenter(int whichObject) const 
{
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

bool Application::setData(const char* fileName)
{
	clearObjectClasses();

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
	//string objectClass = "";
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
				bool found = false;
				for (Attribute* actual : objectClasses) {
					if (actual->getName() == word) {
						flower->setObjectClass(actual);
						found = true;
						break;
					}
				}
				if (!found) {
					Attribute* first = new Attribute(word);
					objectClasses.push_back(first);
					flower->setObjectClass(first);
				}
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
				flower->setName(objectClass);
				data->add(flower);
				flower = new Object();
				values.clear();
			}
			if (next == '%') {
				getline(myFile,objectClass);
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
		string objectClass = "";
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
				/*fscanf(datafile, "%s", &objectClass);
				flower->setName(objectClass);
				flower->setValues(values);
				numberOfCoordinates = values.size();
				all.push_back(flower);
				flower = new Object();
				values.clear();*//*
				objectClass = "";
				do {
					fscanf(datafile, "%c", &oneChar);
					objectClass += oneChar[0];
					countChars++;
					countNumbers = 0;
				} while (*oneChar != ',' && *oneChar != '\n');
			}
			else {
				//fscanf(datafile, "%c",&objectClass);
				if ((countChars > 2 && values.size() > 0) || countNumbers == 1) {
					flower->setValues(values);
					objectClass = objectClass.substr(0, objectClass.size() - 1);
					flower->setName(objectClass);
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
		objectClass = objectClass.substr(0, objectClass.size() - 1);
		flower->setName(objectClass);
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
	string resultPath = "";
	if (fileOutputMode) {
		resultPath = createFolderForOutput();
	}

	double sum = 0;
	double sumSquared = 0;
	for (int i = 0; i < numberOfReplications; i++) {
		counter->recount();
		counter->printJm();
		assignClusters();

		FuzzyData* best = counter->getBest();
		cvi.count(best);
		
		if (fileOutputMode) {
			saveResultToFile(best, i + 1, resultPath);
		}

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

string Application::createFolderForOutput() const
{
	string titleString = title;
	time_t now = time(0);
	tm* date = localtime(&now);
	char folderName[50];

	sprintf(folderName, "%s_%02d-%02d-%4d_%02d-%02d-%02d", titleString.c_str(), date->tm_mday, date->tm_mon, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec);
	string resultPath = resultFolderName + "/" + folderName;
	mkdir(resultFolderName.c_str());
	mkdir(resultPath.c_str());
	return resultPath;
}

void Application::resetClusterAttributes()
{
	clearClusterAttributes();
	for (int j = 0; j < numberOfClusters; j++) {
		Attribute* newAttr = new Attribute("Zhluk" + to_string(j));
		clusters.push_back(newAttr);
	}
}
