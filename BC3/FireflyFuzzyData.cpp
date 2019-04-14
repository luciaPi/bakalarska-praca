#include "FireflyFuzzyData.h"
#include <math.h> 


double FireflyFuzzyData::computeOverallDistance(const double ** other) const
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

void FireflyFuzzyData::setK(int parK)
{
	if (parK > 0) {
		fitness = fitness * parK / K;
		FuzzyData::setK(parK);		
	}
}

void FireflyFuzzyData::setM(int parm)
{
	if (parm > 0) {
		FuzzyData::setM(parm);
		Firefly::init();
	}
}

void FireflyFuzzyData::setNumberOfClusters(int number)
{
	if (numberOfClusters > 0) {
		FuzzyData::setNumberOfClusters(number);
		setSize(data.getSize()*numberOfObjects);
	}
}

void FireflyFuzzyData::setData(Dataset pardata)
{
	FuzzyData::setData(pardata);
	setSize(data.getSize()*numberOfObjects);
}

void FireflyFuzzyData::setX(const FireflyFuzzyData & other)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			mu[i][j] = other.mu[i][j];
		}
	}
	recalculateFromMu();
	fitness = setFitness();

	/*muPrint();
	other.muPrint();
	centersPrint();
	other.centersPrint();*/
}

void FireflyFuzzyData::Xprint() const
{
	centersPrint();
}

void FireflyFuzzyData::normalize()
{
	recalculateFromCenters();
	recalculateFromMu();
}

double FireflyFuzzyData::setFitness() const
{
	return FuzzyData::getFitness();
}

bool FireflyFuzzyData::setX()
{
	if (size == numberOfClusters * numberOfCoordinates) {
		for (int j = 0; j < numberOfClusters; j++) {
			for (int k = 0; k < numberOfCoordinates; k++) {
				*(X + j * numberOfCoordinates + k) = centers[j]->getPointerValue(k);
			}
		}
		recalculateFromCenters(); //mozno?
		return true;
	}
	return false;
}

void FireflyFuzzyData::setMinMaxCoordinates()
{
	for (int k = 0; k < numberOfCoordinates; k++) {
		double min = data.getMinCoordinate(k);
		double max = data.getMaxCoordinate(k);
		for (int j = 0; j < numberOfClusters; j++) {
			*(minValues + j * numberOfCoordinates + k) = min;
			*(maxValues + j * numberOfCoordinates + k) = max;
		}
	}
}

std::string FireflyFuzzyData::getName() const
{
	return name;
}

std::string FireflyFuzzyData::getAlgorithmName() const
{
	return nameAlg;
}
