#include "Firefly.h"
#include <iostream>
#include <math.h> 
#include <string>

using namespace std;

void Firefly::init()
{
	bool first = true;
	if (X != nullptr) {
		first = false;
	}
	clearFirefly();
	XInit();
	minmaxInit();

	if (alpha < 0) {
		alpha = (double)rand() / RAND_MAX;
	}
	if (!first) {
		setStartingX();
	}
}

void Firefly::setStartingX()
{
	if (size > 0) {
		setX();
		setMinMaxCoordinates();
		fitness = getFitness();
	}
}

Firefly::Firefly()
{
	init();
}

Firefly::Firefly(double palpha, double pbeta, double pgamma, int psize)
{
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

double Firefly::computeOverallDistance(const double** other) const
{
	double sum = 0;
	for (int i = 0; i < size; i++) {
		sum += *(X + i);
	}
	return sum;
}

bool Firefly::move(const double** other)
{
	try {
		double r = computeOverallDistance(other);
		double beta = beta * exp(-gamma * pow(r, 2));

		for (int i = 0; i < size; i++) {
			double randNumber = (double)rand() / RAND_MAX;
			double coordinateDistance = **(other + i) - *(X + i);
			double newValue = *(X + i) + beta * coordinateDistance + alpha * (randNumber - 0.5);
			if (newValue < minValues[i]) {
				newValue = minValues[i];
			}
			if (newValue > maxValues[i]) {
				newValue = maxValues[i];
			}
			*(X + i) = newValue;
		}
		normalize();
		fitness = getFitness();
	}
	catch (exception ex) {
		return false;
	}
	return true;
}

void Firefly::setAlpha(double palpha)
{
	if (palpha >= 0) {
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

void Firefly::XInit()
{
	X = new double[size];
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
