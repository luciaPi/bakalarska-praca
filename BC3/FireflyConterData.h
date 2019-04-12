#pragma once
#include "CounterData.h"
#include "Firefly.h"

class FireflyConterData : public CounterData, public Firefly
{
private:
	virtual double getFitness() const override;
	virtual bool setX() override;

	virtual void setMinMaxCoordinates() override;

	virtual std::string getName() const override;
	virtual std::string getAlgorithmName() const override;

protected:
	double computeOverallDistance(const double** other) const;

public:
	FireflyConterData();
	~FireflyConterData();
};

