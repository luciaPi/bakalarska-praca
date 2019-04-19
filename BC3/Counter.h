#pragma once
#include <string>
#include "FuzzyData.h"

enum class FinalCriterion { minChange, maxIteration, both };

class Counter
{	
private:
	int maxIteration = 100;
	FinalCriterion finalCriterion = FinalCriterion::both;

protected:
	double minChange = 0.0001;

	std::string nameAlg = "Algorithm";

	bool isMetFinalCriterion(int actualIterationNumber) const;
	virtual bool wasSignificantChange() const = 0;

public:
	Counter() {};
	~Counter() {};

	void setMaxIterations(int maxIteration);
	void setMinChange(double minChange);
	void setFinalCriterion(FinalCriterion fc);

	void setAlgorithmName(std::string name);
	std::string getAlgorithmName() const;

	virtual double getJm() const = 0;
	virtual void printJm() const = 0;
	virtual FuzzyData* getBest() const = 0;
	virtual void recount() = 0;
};

