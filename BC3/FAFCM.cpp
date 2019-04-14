#include "FAFCM.h"

void FAFCM::count(Dataset data, int numberOfClusters, int m, double alpha, double beta, double gamma, int P)
{
	FAcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FAcounter::setMaxIterations(20);

	FCMcounter::setFinalCriterion(FinalCriterion::maxIteration);
	FCMcounter::setMaxIterations(20);

	FAcounter::count(data, numberOfClusters, m, alpha, beta, gamma, P);
	FCMcounter::count(gbest);

	gbest->centersPrint();
	gbest->printJm();
}

void FAFCM::setAlgorithmName(string name)
{
	FAcounter::setAlgorithmName(name);
	FCMcounter::setAlgorithmName(name);
}
