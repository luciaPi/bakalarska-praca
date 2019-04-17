#include "Firefly.h"
#include <iostream>
#include <math.h> 
#include <string>

using namespace std;

void Firefly::init()
{
	if (randMovementGenerator == nullptr || randMovementFirstGenerator == nullptr) {
		throw exception();
	}
	bool first = true;
	if (X != nullptr) {
		first = false;
	}
	clearFirefly();
	XInit();
	minmaxInit();

	if (!first) {
		setStartingX();
	}
}

double Firefly::getXValue(int i) const
{
	return **(X + i);
}

void Firefly::setStartingX()
{
	if (size > 0) {
		setX();
		setMinMaxCoordinates();
		fitness = setFitness();
	}
}

Firefly::Firefly()
{
	init();
}

Firefly::Firefly(double palpha, double pbeta, double pgamma, int psize, RandomGenerator* parrandMovementGenerator, RandomGenerator* parrandMovementFirstGenerator)
{
	randMovementGenerator = parrandMovementGenerator;
	randMovementFirstGenerator = parrandMovementFirstGenerator;

	if (palpha >= 0 && alpha <= 1) {
		alpha = palpha;
	}
	if (pbeta >= 0) {
		beta = pbeta;
	}
	if (pgamma >= 0) {
		gamma = pgamma;
	}
	if (psize > 0) {
		size = psize;
	}
	init();
}

Firefly::~Firefly()
{
	clearFirefly();
}

void Firefly::Xprint() const
{
	cout << getName() << " - X " << getAlgorithmName() << ": " << endl;
	for (int i = 0; i < size; i++) {
		cout << X[i] << " ";
	}
	cout << endl;
	cout << endl;
}

double Firefly::computeOverallDistance(const Firefly* other) const
{
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += pow(**(X + i)-other->getXValue(i),2);
	}
	return sqrt(sum);
}

bool Firefly::move(const Firefly* other)
{
	try {
		double r = computeOverallDistance(other);
		double betaMove = beta * exp(-gamma * pow(r, 2));	//dat na druhu
		//cout << getName() << " to " << other->getName() << endl;
		if (betaMove > 0) {
			for (int i = 0; i < size; i++) {
				double randNumber = randMovementGenerator->nextRandom();
				double coordinateDistance = **(other->X + i) - **(X + i);
				double newValue = **(X + i) + betaMove * coordinateDistance + alpha * (randNumber - 0.5);
				if (newValue < minValues[i]) {
					newValue = minValues[i];
				}
				if (newValue > maxValues[i]) {
					newValue = maxValues[i];
				}
				//cout << **(X + i) << " : " << newValue << endl;
				**(X + i) = newValue;
			}
			//cout << endl;
			normalize();
			fitness = setFitness();
		}
	}
	catch (exception ex) {
		return false;
	}
	return true;
}

void Firefly::moveRandomly()
{
	for (int i = 0; i < size; i++) {
		**(X + i) = randMovementFirstGenerator->nextRandom() / randMovementFirstGenerator->getMax() * (maxValues[i] - minValues[i]) + minValues[i];
	}
	normalize();
	fitness = setFitness();
}

void Firefly::setAlpha(double palpha)
{
	if (palpha >= 0 && alpha <= 1) {
		alpha = palpha;
	}
}

void Firefly::setBeta(double pbeta)
{
	if (pbeta >= 0) {
		beta = pbeta;
	}
}

void Firefly::setGamma(double pgamma)
{
	if (pgamma >= 0) {
		gamma = pgamma;
	}
}

void Firefly::setSize(int psize)
{
	if (psize > 0) {
		size = psize;
		init();
	}
}

double Firefly::getFireflyFitness() const
{
	return fitness;
}

double Firefly::getAlpha() const
{
	return alpha;
}

double Firefly::getBeta() const
{
	return beta;
}

double Firefly::getGamma() const
{
	return gamma;
}

void Firefly::setX(const Firefly & other)
{
	for (int i = 0; i < size; i++) {
		*(X + i) = *(other.X + i);
	}
	fitness = setFitness();
}

void Firefly::XInit()
{
	X = new double*[size];
}

void Firefly::minmaxInit()
{
	minValues = new double[size];
	maxValues = new double[size];
}

void Firefly::clearFirefly()
{
	delete[] X;
	X = nullptr;

	delete[] minValues;
	minValues = nullptr;

	delete[] maxValues;
	maxValues = nullptr;
	
	fitness = 0;
}

int Firefly::getSize() const
{
	return size;
}

void Firefly::printMinMax() const
{
	cout << "min: ";
	for (int i = 0; i < size; i++) {
		cout << minValues[i] << " ";
	}
	cout << endl;
	cout << "max: ";
	for (int i = 0; i < size; i++) {
		cout << maxValues[i] << " ";
	}
	cout << endl;
}
