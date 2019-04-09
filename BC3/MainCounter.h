#pragma once

enum class FinalCriterion { minChange, maxIteration, both };

class MainCounter
{	
private:
	double minChange = 0.0001;
	int maxIteration = 20;
	FinalCriterion finalCriterion = FinalCriterion::both;

protected:
	bool isMetFinalCriterion(int actualIterationNumber) const;
	virtual bool wasSignificantChange() const = 0;

public:
	MainCounter();
	~MainCounter();

	void setMaxIterations(int maxIteration);
	void setMinChange(double minChange);
	void setFinalCriterion(FinalCriterion fc);

	virtual double getJm() const = 0;
	virtual void printJm() const = 0;
};

