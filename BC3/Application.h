#pragma once
#include "Dataset.h"
#include "Attribute.h"

enum class Algorithm { fcm, pso, fa, fcmpso, fafcm };

class Application
{
private:
	Dataset *data = nullptr;
	int numberOfClusters = 0;
	int m = 2;

	double c1 = 2;
	double c2 = 2;
	double r1 = 0.5;
	double r2 = 0.5;
	int w = 2;
	int Ppso = 10;
	int Pfa = 20;

	double alpha = 0.5;
	double beta = 1;
	double gamma = 1;

	void dataObjectsPrint() const;
	
public:
	Application();
	~Application();		

	void count(Algorithm alg, int numberOfIteration);

	bool setData(const char* name);
	bool setNumberOfClusters(int number);

	void saveOutputToArff(const char* filename, char* title, char* creator, char* donor, char* relation, vector<Attribute*> attributes) const;
};

