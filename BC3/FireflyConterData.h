#pragma once
#include "CounterData.h"
#include "Firefly.h"

class FireflyCounterData : public CounterData, public Firefly
{
private:
	virtual double setFitness() const override;
	virtual bool setX() override;
	virtual void normalize() override;

	virtual void setMinMaxCoordinates() override;
	
protected:
	double computeOverallDistance(const double** other) const;

public:
	FireflyCounterData() {};
	FireflyCounterData(int numberOfClusters) : CounterData(numberOfClusters) {};
	FireflyCounterData(Dataset data) : CounterData(data) {};
	FireflyCounterData(Dataset data, int numberOfClusters, int m) : CounterData(data, numberOfClusters, m) {};
	FireflyCounterData(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma) :
		CounterData(data, numberOfClusters, m),
		Firefly(alpha,beta,gamma, data.getSize()*numberOfClusters)
	{
		setStartingX();
	};
	~FireflyCounterData() {};

	void setK(int K);
	void setM(int m);
	void setNumberOfClusters(int number);
	void setData(Dataset pardata);
	void setX(const FireflyCounterData& other);

	void Xprint() const;

	virtual string getName() const override;
	virtual string getAlgorithmName() const override;
};

