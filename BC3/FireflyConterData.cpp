#include "FireflyConterData.h"
#include <math.h> 


double FireflyCounterData::computeOverallDistance(const double ** other) const
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

void FireflyCounterData::setK(int parK)
{
	if (parK > 0) {
		fitness = fitness * parK / K;
		CounterData::setK(parK);		
	}
}

void FireflyCounterData::setM(int parm)
{
	if (parm > 0) {
		CounterData::setM(parm);
		Firefly::init();
	}
}

void FireflyCounterData::setNumberOfClusters(int number)
{
	if (numberOfClusters > 0) {
		CounterData::setNumberOfClusters(number);
		setSize(data.getSize()*numberOfObjects);
	}
}

void FireflyCounterData::setData(Dataset pardata)
{
	CounterData::setData(pardata);
	setSize(data.getSize()*numberOfObjects);
}

void FireflyCounterData::setX(const FireflyCounterData & other)
{
	for (int j = 0; j < numberOfClusters; j++) {
		for (int k = 0; k < numberOfCoordinates; k++) {
			centers[j]->setValue(k, *(other.X + j * numberOfCoordinates + k));
		}
	}
	recalculateFromCenters();
	fitness = setFitness();
}

void FireflyCounterData::Xprint() const
{
	muPrint();
}

void FireflyCounterData::normalize()
{
	recalculateFromCenters();
}

double FireflyCounterData::setFitness() const
{
	return CounterData::getFitness();
}

bool FireflyCounterData::setX()
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

void FireflyCounterData::setMinMaxCoordinates()
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

std::string FireflyCounterData::getName() const
{
	return name;
}

std::string FireflyCounterData::getAlgorithmName() const
{
	return nameAlg;
}
