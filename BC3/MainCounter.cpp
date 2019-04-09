#include "MainCounter.h"



bool MainCounter::isMetFinalCriterion(int actualIterationNumber) const
{
	switch (finalCriterion) {
	case FinalCriterion::maxIteration:
		return actualIterationNumber > maxIteration;
	case FinalCriterion::minChange:
		return !wasSignificantChange();
	}
	return (actualIterationNumber > maxIteration || !counterData->wasSignificantChange());
}

MainCounter::MainCounter()
{
}

MainCounter::~MainCounter()
{
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
