#include "FAFCMcounter.h"

void FAFCMcounter::setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode)
{
	FAcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FAcounter::setMaxIterations(20);

	FCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FCMcounter::setMaxIterations(20);

	FAcounter::setCounter(data, numberOfClusters, m, alpha, beta, gamma, P,K, muInitMode);
	FCMcounter::setCounter(data, numberOfClusters, m,K,muInitMode);
}

void FAFCMcounter::count()
{
	FAcounter::recount();
	FCMcounter::count(gbest);

	//gbest->centersPrint();
	//gbest->printJm();
}

void FAFCMcounter::recount()
{
	if (gbest != nullptr) {
		count();
	}
}

void FAFCMcounter::setAlgorithmName(string name)
{
	FAcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
}

bool FAFCMcounter::wasSignificantChange() const
{
	return FCMcounter::wasSignificantChange() && FAcounter::wasSignificantChange();
}

double FAFCMcounter::getJm() const
{
	return FAcounter::getJm();
}

void FAFCMcounter::printJm() const
{
	return FAcounter::printJm();
}

const FuzzyData * FAFCMcounter::getBest() const
{
	return FAcounter::getBest();
}
