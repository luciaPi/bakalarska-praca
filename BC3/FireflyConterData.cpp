#include "FireflyConterData.h"
#include <math.h> 


double FireflyConterData::computeOverallDistance(const double ** other) const
{
	double allSum = 0;
	for (int j = 0; j < numberOfClusters; j++) {
		double sum = 0;
		for (int k = 0; k < numberOfCoordinates; k++) {
			double coordinate1 = centers[j]->getValue(k);
			double coordinate2 = **(other+j*numberOfCoordinates+k);
			sum += pow(coordinate1 - coordinate2, 2);
		}
		allSum += sqrt(sum);
	}
	return allSum;
}

FireflyConterData::FireflyConterData()
{
}


FireflyConterData::~FireflyConterData()
{
}


double FireflyConterData::getFitness() const
{
	return 0.0;
}

bool FireflyConterData::setX()
{
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			*(X + j * numberOfCoordinates + k) = centers[j]->getPointerValue(k);
		}
	}
}

void FireflyConterData::setMinMaxCoordinates()
{
}

std::string FireflyConterData::getName() const
{
	return std::string();
}

std::string FireflyConterData::getAlgorithmName() const
{
	return std::string();
}
