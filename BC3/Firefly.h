#pragma once
#include "Object.h"
#include "Dataset.h"

class Firefly
{
private:
	double alpha = -1;
	double beta = 1;
	double gamma = 1;
				
	void XInit();
	void minmaxInit();
	
	double computeOverallDistance(const double** other) const;

	void clearFirefly();

	int getSize() const;

protected:
	int size = 0;
	double *X = nullptr;
	double fitness = 0;

	double* minValues = nullptr;
	double* maxValues = nullptr;

	virtual void normalize() = 0;
	virtual double getFitness() const = 0;
	virtual bool setX() = 0;
	virtual void setMinMaxCoordinates() = 0;

	void setStartingX();
	void init();

public:
	Firefly();
	Firefly(double alpha, double beta, double gamma, int size);
	~Firefly();

	void Xprint() const;
	
	bool move(const double** other);

	void setAlpha(double alpha);
	void setBeta(double beta);
	void setGamma(double gamma);
	void setSize(int size);

	virtual std::string getName() const = 0;
	virtual std::string getAlgorithmName() const = 0;

	//void setCenters(vector<Object> newCenters);
};

