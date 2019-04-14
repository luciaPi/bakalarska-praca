#include "MainCounter.h"

using namespace std;

bool MainCounter::isMetFinalCriterion(int actualIterationNumber) const
{
	switch (finalCriterion) {
	case FinalCriterion::maxIteration:
		return actualIterationNumber > maxIteration;
	case FinalCriterion::minChange:
		return !wasSignificantChange();
	}
	return (actualIterationNumber > maxIteration || !wasSignificantChange());
}

void MainCounter::setMaxIterations(int parmaxIteration)
{
	maxIteration = parmaxIteration;
}

void MainCounter::setMinChange(double parminChange)
{
	minChange = parminChange;
}

void MainCounter::setFinalCriterion(FinalCriterion fc)
{
	finalCriterion = fc;
}

void MainCounter::setAlgorithmName(string pname)
{
	nameAlg = pname;
}
