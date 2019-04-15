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
	
	double computeOverallDistance(const Firefly* other) const;

	void clearFirefly();

	int getSize() const;
	
protected:
	int size = 0;
	double **X = nullptr;
	double fitness = 0;

	double* minValues = nullptr;
	double* maxValues = nullptr;

	virtual void normalize() = 0;
	virtual double setFitness() const = 0;
	virtual bool setX() = 0;
	virtual void setMinMaxCoordinates() = 0;

	void setStartingX();
	void init();

	double getXValue(int i) const;

public:
	Firefly();
	Firefly(double alpha, double beta, double gamma, int size);
	~Firefly();

	void Xprint() const;
	void printMinMax() const;
	
	bool move(const Firefly* other);
	void moveRandomly();

	void setAlpha(double alpha);
	void setBeta(double beta);
	void setGamma(double gamma);
	void setSize(int size);
	void setX(const Firefly& other);

	virtual std::string getName() const = 0;
	virtual std::string getAlgorithmName() const = 0;
	double getFireflyFitness() const;

	double getAlpha() const;
	double getBeta() const;
	double getGamma() const;

	//void setCenters(vector<Object> newCenters);
};

