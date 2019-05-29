#include "Counter.h"
#include <iostream>

using namespace std;

bool Counter::isMetFinalCriterion(int actualIterationNumber) const
{
	switch (finalCriterion) {
	case FinalCriterion::maxIteration:
		return actualIterationNumber > maxIteration;
	case FinalCriterion::minChange:
		return !wasSignificantChange();
	}
	return (actualIterationNumber > maxIteration || !wasSignificantChange());
}

void Counter::setMaxIterations(int parmaxIteration)
{
	maxIteration = parmaxIteration;
}

void Counter::setMinChange(double parminChange)
{
	minChange = parminChange;
}

void Counter::setFinalCriterion(FinalCriterion fc)
{
	finalCriterion = fc;
}

void Counter::setAlgorithmName(string pname)
{
	nameAlg = pname;
}

string Counter::getAlgorithmName() const
{
	return nameAlg;
}

