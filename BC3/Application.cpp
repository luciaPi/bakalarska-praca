#include "Application.h"
#include <iostream>
#include <fstream>
#include <time.h>
#include <ctime>
#include <string>
#include <sstream>
#include <vector>
#include <direct.h>
#include "Object.h"
#include "FCMcounter.h"
#include "PSOcounter.h"
#include "FCMPSOcounter.h"
#include "FAFCMcounter.h"
#include "HFAFCMcounter.h"

using namespace std;

Application::Application()
{
	srand(time(0));
	parameterGenerator = RandomGenerator(rd(), 0, 1);
	cvi = new CVI(rd(), rd());
	cvi->resetCVI();
}

Application::~Application()
{
	delete data;
	data = nullptr;

	delete cvi;
	
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
				if (isSetFinalCriterion) {
					fcmCounter.setFinalCriterion(finalCriterion);
				}
				else {
					fcmCounter.setFinalCriterion(FinalCriterion::minChange);
				}
				if (isSetMinCHange) {
					fcmCounter.setMinChange(minChange);
				}
				fcmCounter.setAlgorithmName("FCM");
				fcmCounter.setMaxIterations(numberOfItertion);

				fcmCounter.setCounter(*data, numberOfClusters, m, K, muInitMode);
				count(&fcmCounter);
				break;
			}
			case Algorithm::pso: {
				PSOcounter psoCounter(rd(), rd(), rd());
				psoCounter.setAlgorithmName("PSO");
				if (isSetFinalCriterion) {
					psoCounter.setFinalCriterion(finalCriterion);
				}
				else {
					psoCounter.setFinalCriterion(FinalCriterion::maxIteration);
				}	
				if (isSetMinCHange) {
					psoCounter.setMinChange(minChange);
				}
				psoCounter.setMaxIterations(numberOfItertion);		
				if (maxV > 0) {
					psoCounter.setMaxV(maxV);
				}

				psoCounter.setCounter(*data, numberOfClusters, m,c1, c2, r1, r2, w, Ppso, K, muInitMode);
				count(&psoCounter);
				break;
			}	
			case Algorithm::fcmpso: {
				FCMPSOcounter fcmpsoCounter(rd(), rd(), rd(),rd(), rd());
				fcmpsoCounter.setAlgorithmName("FCM-PSO");				
				fcmpsoCounter.setNumberOfIterations(numberOfItertion);
				if (maxV > 0) {
					fcmpsoCounter.setMaxV(maxV);
				}

				fcmpsoCounter.setCounter(*data, numberOfClusters, m, c1, c2, r1, r2, w, Ppso, K, muInitMode);
				count(&fcmpsoCounter);
				break;
			}
			case Algorithm::fa: {
				FAFCMcounter faCounter(rd(), rd(), rd(), rd());
				faCounter.setAlgorithmName("FA");
				faCounter.setMaxIterations(numberOfItertion);
				if (isSetFinalCriterion) {
					faCounter.setFinalCriterion(finalCriterion);
				}
				else {
					faCounter.setFinalCriterion(FinalCriterion::maxIteration);
				}		
				if (isSetMinCHange) {
					faCounter.setMinChange(minChange);
				}

				faCounter.setCounter(*data, numberOfClusters, m, alpha, beta, gamma, Pfa, K, muInitMode);
				count(&faCounter);
				break;
			}
			case Algorithm::fafcm: {
				HFAFCMcounter fafcmCounter(rd(), rd(), rd(), rd(),rd(), rd());
				fafcmCounter.setAlgorithmName("FAFCM");

				fafcmCounter.setCounter(*data, numberOfClusters, m, alpha, beta, gamma, Pfa, K, muInitMode);
				count(&fafcmCounter);
				break;
			}
		}	
	}
}

bool Application::setM(int parm)
{
	m = parm;
	return true;
}

bool Application::setC1(double parc1)
{
	c1 = parc1;
	return true;
}

bool Application::setC2(double parc2)
{
	c2 = parc2;
	return true;
}

bool Application::setR1(double parr1)
{
	r1 = parr1;
	return r1;
}

bool Application::setR2(double parr2)
{
	r2 = parr2;
	return true;
}

bool Application::setW(double parw)
{
	w = parw;
	return true;
}

bool Application::setSizeOfPopulationPSO(int size)
{
	Ppso = size;
	return true;
}

bool Application::setSizeOfPopulationFA(int size)
{
	Pfa = size;
	return true;
}

bool Application::setAlpha(double palpha)
{
	alpha = palpha;
	return true;
}

bool Application::setBeta(double pbeta)
{
	beta = pbeta;
	return true;
}

bool Application::setGamma(double pgamma)
{
	gamma = pgamma;
	return true;
}

bool Application::setK(int parK)
{
	K = parK;
	return true;
}

bool Application::setMaxV(double parmaxV)
{
	maxV = parmaxV;
	return true;
}

bool Application::setMinChange(double pminCHange)
{
	minChange = pminCHange;
	isSetMinCHange = true;
	return true;
}

bool Application::unsetMinChange()
{
	isSetMinCHange = false;
	return true;
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

bool Application::setClusterName(string name, int which)
{
	if (which >= 0 && which < numberOfClusters) {
		clusters[which]->setValues(name);
		return true;
	}
	return true;
}

void Application::setCVI(IndexEnum index)
{
	cvi->setIndexToCount(index);
}

void Application::setAllCVI()
{
	cvi->setAllIndecesToCount();
}

void Application::unsetCVI(IndexEnum index)
{
	cvi->unsetIndexToCount(index);
}

void Application::unsetAllCVI()
{
	cvi->unsetAllIndecesToCount();
}

void Application::setFileOutputMode()
{
	fileOutputMode = true;
}

void Application::unsetFileOutputMode()
{
	fileOutputMode = false;
}

void Application::setCVIFileOutputMode()
{
	fileCVIOutputMode = true;
}

void Application::unsetCVIFileOutputMode()
{
	fileCVIOutputMode = false;
}

void Application::setCVIConsoleOutputMode()
{
	consoleCVIOutputMode = true;
}

void Application::unsetCVIConsoleOutputMode()
{
	consoleCVIOutputMode = false;
}

void Application::setEachCVI()
{
	eachCVI = true;
}

void Application::unsetEachCVI()
{
	eachCVI = false;
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

		if (muInitMode == MuInitializationMode::fcmPlusPlus) {
			titles[1] = "Centers++";
		}
		
		//title
		int which = 0;
		fprintf(datafile, "%c %s: ", comment, titles[which++]);
		fprintf(datafile, "%s\n%c\n", title, comment);
		//centers
		fprintf(datafile, "%c %s:\n", comment, titles[which++]);

		Object** centers = fuzzyData->getCenters();
		for (int j = 0; j < numberOfClusters; j++) {
			fprintf(datafile, "%c \t", comment);
			for (int k = 0; k < (*data)[0].getNumberOfCoordinates(); k++) {
				fprintf(datafile, "%f ", centers[j]->getValue(k));
			}
			fprintf(datafile, "\n", comment);
		}		
		
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
			string name = actual->getName();
			fprintf(datafile, "%s", name.c_str());
		}
		fprintf(datafile, "}\n");
		//class
		fprintf(datafile, "%s class \t {", titles[which]);
		for (Attribute *actual : objectClasses) {
			if (actual != objectClasses[0]) {
				fprintf(datafile, ", ");
			}
			string name = (actual->getName());
			fprintf(datafile, "%s", name.c_str());
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
			string name = (*data)[i].getName();
			fprintf(datafile, "%s\n", name.c_str());
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

int Application::whichCenter(int whichObject, FuzzyData* fuzzyData) const
{
	double max = 0;
	int maxCoordinate = -1;
	const double** mu = fuzzyData->getMu();
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
	delete data;
	data = nullptr;

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
	}
	delete flower;

	myFile.close();

	return true;
}

bool Application::setNumberOfClusters(int number)
{
	if (number > 0) {
		numberOfClusters = number;
		resetClusterAttributes();
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

bool Application::setTitle(char * ptitle)
{
	title = ptitle;
	return true;
}

bool Application::setFinalCriterion(FinalCriterion criterion)
{
	finalCriterion = criterion;
	isSetFinalCriterion = true;
	return true;
}

bool Application::unsetFinalCriterion()
{
	isSetFinalCriterion = false;
	return true;
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

void Application::dataObjectsPrintWithClass() const
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
			cout << (*data)[i].getAssignedName() << endl;
		}
	}
}

void Application::count(Counter * counter)
{
	cvi->setAssignedClusters(clusters);

	string resultPath = "";
	string resultCVIPath = "";
	string titleString = title;
	if (fileOutputMode) {		
		resultPath = createFolderForOutput(counter->getAlgorithmName());
	}
	FILE* datafile = nullptr;
	
	fstream fileStream(datafile);
	if (fileCVIOutputMode) {
		resultCVIPath = createFolderForCVIOutput(counter->getAlgorithmName());
		datafile = openCVIFile(resultCVIPath);
		fileStream = fstream(datafile);
		cvi->printResultsHeader(fileStream);
	}
	if (consoleCVIOutputMode) {
		cvi->printResultsHeader(cout);
	}

	double sum = 0;
	double sumSquared = 0;
	for (int i = 0; i < numberOfReplications; i++) {
		counter->recount();

		FuzzyData* best = counter->getBest();

		assignClusters(best);
		cvi->count(best,*data);
		
		if (fileOutputMode) {
			saveResultToFile(best, i + 1, resultPath);
		}
		if (fileCVIOutputMode && datafile != nullptr) {
			cvi->printResults(fileStream, titleString, actualT);
		}
		
		if (numberOfReplications == 1) {
			cout << "----------------------------------------------------------" << endl;
			cout << "Vysledky algoritmu " << counter->getAlgorithmName() << endl;	
			if (consoleCVIOutputMode) {
				cvi->printResultsHeader(cout);
				cvi->printResults(cout, titleString, actualT);
			}
			(counter->getBest())->centersPrint();
			(counter->getBest())->muPrint();
			cout << "----------------------------------------------------------" << endl;
		}
		else {
			if (consoleCVIOutputMode && eachCVI) {
				cvi->printResultsHeader(cout);
				cvi->printResults(cout, titleString, actualT);
			}
		}
	}
	if (consoleCVIOutputMode && !eachCVI && numberOfReplications != 1) {
		cvi->printResultsHeader(cout);
		cvi->printResults(cout, titleString, actualT);
	}
	
	if (datafile != nullptr) {
		fclose(datafile);
	}
}

FILE* Application::openCVIFile(string resultCVIpath)
{
	string titleString = title;
	string filename = titleString + ".txt";
	string filenameWithPath = resultCVIpath + "/" + filename;
	FILE* datafile = nullptr;
	datafile = fopen(filenameWithPath.c_str(), "w");
	return datafile;
}

string Application::createFolderForOutput(string algName) const
{
	string titleString = title;
	time_t now = time(0);
	tm* date = localtime(&now);
	char folderName[50];

	sprintf(folderName, "%s_%02d-%02d-%4d_%02d-%02d-%02d_%s(%d)", titleString.c_str(), date->tm_mday, date->tm_mon, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec, algName.c_str(),numberOfClusters);
	string resultPath = resultFolderName + "/" + folderName;
	mkdir(resultFolderName.c_str());
	mkdir(resultPath.c_str());
	return resultPath;
}

string Application::createFolderForCVIOutput(string algName) const
{
	string titleString = title;
	time_t now = time(0);
	tm* date = localtime(&now);
	char folderName[50];

	sprintf(folderName, "%s_%02d-%02d-%4d_%02d-%02d-%02d_%s(%d)", titleString.c_str(), date->tm_mday, date->tm_mon, date->tm_year + 1900, date->tm_hour, date->tm_min, date->tm_sec, algName.c_str(),numberOfClusters);
	string resultPath = resultCVIFolderName + "/" + folderName;
	mkdir(resultCVIFolderName.c_str());
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
