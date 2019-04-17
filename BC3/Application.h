#pragma once
#include "Dataset.h"
#include "Attribute.h"
#include "Counter.h"
#include <random>
#include "RandomGenerator.h"
#include "FCMcounter.h"

enum class Algorithm { fcm, pso, fa, fcmpso, fafcm };

class Application
{
private:
	random_device rd;
	RandomGenerator parameterGenerator;

	Dataset *data = nullptr;
	int numberOfClusters = 0;
	int numberOfReplications = 1;
	int m = 2;
	int K = 1;

	MuInitializationMode muInitMode = MuInitializationMode::random;

	double c1 = 2;
	double c2 = 2;
	double r1 = -1;
	double r2 = -1;
	int w = 2;
	int Ppso = 10;
	int Pfa = 20;

	int t90 = 1.645;
	int t95 = 1.96;
	int t99 = 2.576;
	int actualT = t95;

	double alpha = -1;
	double beta = 1;
	double gamma = 1;

	void dataObjectsPrint() const;

	void count(Counter* counter);
	
public:
	Application();
	~Application();		

	void count(Algorithm alg, int numberOfIteration);

	bool setM(int m);
	bool setC1(double c1);
	bool setC2(double c2);
	bool setR1(double r1);
	bool setR2(double r2);
	bool setW(double w);
	bool setSizeOfPopulationPSO(int size);
	bool setSizeOfPopulationFA(int size);
	bool setAlpha(double alpha);
	bool setBeta(double beta);
	bool setGamma(double gamma);
	bool setK(int K);
	double setMaxV(double maxV);

	bool setData(const char* name);
	bool setNumberOfClusters(int number);
	bool setNumberOfReplications(int number);
	bool setConfidenceInterval(int value);

	bool setFinalCriterion(FinalCriterion criterion);
	bool setMinChange(double minCHange);
	bool setMuInitializationMode(MuInitializationMode mode);

	void saveOutputToArff(const char* filename, char* title, char* creator, char* donor, char* relation, vector<Attribute*> attributes) const;
};
