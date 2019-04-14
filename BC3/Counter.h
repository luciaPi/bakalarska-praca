#pragma once
#include <string>

enum class FinalCriterion { minChange, maxIteration, both };

class Counter
{	
private:
	double minChange = 0.0001;
	int maxIteration = 100;
	FinalCriterion finalCriterion = FinalCriterion::both;

protected:
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

	virtual double getJm() const = 0;
	virtual void printJm() const = 0;
};

