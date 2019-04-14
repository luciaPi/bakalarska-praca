#pragma once
#include "FuzzyData.h"
#include "Firefly.h"

class FireflyFuzzyData : public FuzzyData, public Firefly
{
private:
	virtual double setFitness() const override;
	virtual bool setX() override;
	virtual void normalize() override;
	virtual void setMinMaxCoordinates() override;
	
protected:
	double computeOverallDistance(const double** other) const;

public:
	FireflyFuzzyData() {};
	FireflyFuzzyData(int numberOfClusters) : FuzzyData(numberOfClusters) {};
	FireflyFuzzyData(Dataset data) : FuzzyData(data) {};
	FireflyFuzzyData(Dataset data, int numberOfClusters, int m) : FuzzyData(data, numberOfClusters, m) {};
	FireflyFuzzyData(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma) :
		FuzzyData(data, numberOfClusters, m),
		Firefly(alpha,beta,gamma, data[0].getNumberOfCoordinates()*numberOfClusters)
	{
		setStartingX();
	};
	~FireflyFuzzyData() {};

	void setK(int K);
	void setM(int m);
	void setNumberOfClusters(int number);
	void setData(Dataset pardata);
	void setX(const FireflyFuzzyData& other);

	void Xprint() const;

	virtual string getName() const override;
	virtual string getAlgorithmName() const override;
};

