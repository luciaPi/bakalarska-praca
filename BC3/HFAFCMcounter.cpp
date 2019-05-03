#include "HFAFCMcounter.h"

void HFAFCMcounter::setCounter(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P, int K, MuInitializationMode muInitMode)
{
	FAFCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FAFCMcounter::setMaxIterations(20);

	FCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FCMcounter::setMaxIterations(20);

	FAFCMcounter::setCounter(data, numberOfClusters, m, alpha, beta, gamma, P,K, muInitMode);
	FCMcounter::setCounter(data, numberOfClusters, m,K,muInitMode);
}

void HFAFCMcounter::count()
{
	FAFCMcounter::recount();
	FCMcounter::count(gbest);
}

void HFAFCMcounter::recount()
{
	if (gbest != nullptr) {
		count();
	}
}

void HFAFCMcounter::setAlgorithmName(string name)
{
	FAFCMcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
	Counter::setAlgorithmName(name);
}

bool HFAFCMcounter::wasSignificantChange() const
{
	return FCMcounter::wasSignificantChange() && FAFCMcounter::wasSignificantChange();
}

double HFAFCMcounter::getJm() const
{
	return FAFCMcounter::getJm();
}

void HFAFCMcounter::printJm() const
{
	return FAFCMcounter::printJm();
}

FuzzyData * HFAFCMcounter::getBest() const
{
	return FAFCMcounter::getBest();
}
