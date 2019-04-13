#pragma once
#include "CounterData.h"
#include "Firefly.h"

class FireflyConterData : public CounterData, public Firefly
{
private:
	virtual double getFitness() const override;
	virtual bool setX() override;
	virtual void normalize() override;

	virtual void setMinMaxCoordinates() override;

	virtual string getName() const override;
	virtual string getAlgorithmName() const override;

protected:
	double computeOverallDistance(const double** other) const;

public:
	FireflyConterData() {};
	FireflyConterData(int numberOfClusters) : CounterData(numberOfClusters) {};
	FireflyConterData(Dataset data) : CounterData(data) {};
	FireflyConterData(Dataset data, int numberOfClusters, int m) : CounterData(data, numberOfClusters, m) {};
	FireflyConterData(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma) :
		CounterData(data, numberOfClusters, m),
		Firefly(alpha,beta,gamma, data.getSize()*numberOfClusters)
	{
		setStartingX();
	};
	~FireflyConterData() {};

	void setK(int K);
	void setM(int m);
	void setNumberOfClusters(int number);
	void setData(Dataset pardata);

	void Xprint() const;
};

