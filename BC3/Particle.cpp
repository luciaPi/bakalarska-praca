#include "Particle.h"
#include <iostream>
#include <time.h>

using namespace std;

Particle::Particle()
{
	init();	
}

Particle::Particle(double parc1, double parc2, double parr1, double parr2, double parw, int psize, RandomGenerator* pargenerator)	
{
	Vgenerator = pargenerator;
	
	if (parw >= 0) {
		w = parw;
	}
	if (parc1 >= 0) {
		c1 = parc1;
	}
	if (parc2 >= 0) {
		c2= parc2;
	}
	if (parr1 <= 1 && parr1 >= 0) {
		r1 = parr1;
	}
	else {
		r1 = 0.5;
	}
	if (parr2 <= 1 && parr2 >= 0) {
		r2 = parr2;
	}
	else {
		r2 = 0.5;
	}
	if (psize > 0) {
		size = psize;
	}
	init();
}

Particle::~Particle()
{
	clearParticle();
}

void Particle::init()
{
	if (Vgenerator == nullptr) {
		throw exception();
	}
	bool first = true;
	if (X != nullptr) {
		first = false;
	}
	clearParticle();

	VInit();
	Xinit();

	if (!first) {
		setStartingX();
	}
}

void Particle::setStartingX()
{
	if (size > 0) {
		setX();
		pbestInit();
		checkFitness(nullptr);
	}
}

void Particle::pbestInit()
{
	pbest = new double[size];
	for (int i = 0; i < size; i++) {
		*(pbest + i) = **(X + i);
	}
	pbestFitness = getFitness();
}

void Particle::VInit()
{
	V = new double[size];
	for (int i = 0; i < size; i++) {
		*(V + i) = Vgenerator->nextRandom();
	}
}

void Particle::Xinit()
{
	X = new double*[size];
}

bool Particle::checkFitness(Particle* gbest)
{
	double newFitness = getFitness();
	if (newFitness > pbestFitness) {
		pbestFitness = newFitness;
		setValues(X, pbest);
		
		if (gbest != nullptr) {
			if (newFitness > gbest->getFitness()) {				
				return true;
			}
		}
	}
	return false;
}

void Particle::reinit()
{
	for (int i = 0; i < size; i++) {
		*(V + i) = Vgenerator->nextRandom();
	}
	checkFitness(nullptr);
}

void Particle::computeV(const Particle* gbest)
{
	/*if (gbest == nullptr || pbest == nullptr) {
		throw new exception();
	}	
	if (gbest->getSize() != size) {
		throw new exception();
	}*/
	for (int i = 0; i < size; i++) {
		*(V+i) = w * (*(V + i)) + c1 * r1*(*(pbest + i) - **(X + i)) + c2 * r2*(gbest->getXValue(i) - **(X + i));
		//double xx = *(V + i);
		if (*(V + i) > maxV) {
			*(V + i) = maxV;
		}
		else if (*(V + i) < -maxV) {
			*(V + i) = -maxV;
		}
	}
}

void Particle::computeX()
{
	for (int i = 0; i < size; i++) {
		**(X + i) = **(X + i) + *(V + i);
	}
}

bool Particle::compute(Particle* gbest)
{
	try {
		computeV(gbest);
		computeX();
		normalize();
		return checkFitness(gbest);
	}
	catch (exception ex) {
		return false;
	}
	return false;
}

void Particle::setc1c2(double parc1, double parc2)
{

	if (parc1 >= 0) {
		c1 = parc1;
	}
	if (parc2 >= 0) {
		c2 = parc2;
	}
}

void Particle::setr1r2(double parr1, double parr2)
{

	if (parr1 <= 1 && parr1 >= 0) {
		r1 = parr1;
	}
	if (parr2 <= 1 && parr2 >= 0) {
		r2 = parr2;
	}
}

void Particle::setw(double parw)
{
	if (parw >= 0) {
		w = parw;
	}
}

void Particle::setSize(int psize)
{
	if (psize > 0) {
		size = psize;
		init();
	}
}

void Particle::Vprint() const
{
	cout << getName() << " - V " << getAlgorithmName() << ": " << endl;
	for (int i = 0; i < size; i++) {
		cout << V[i] << " ";		
	}
	cout << endl;
	cout << endl;
}

void Particle::pbestPrint() const
{
	cout << getName() << " - PBest " << getAlgorithmName() << ": " << endl;
	for (int i = 0; i < size; i++) {
		cout << pbest[i] << " ";
	}
	cout << endl;
	cout << endl;
}

void Particle::Xprint() const
{
	cout << getName() << " - X " << getAlgorithmName() << ": " << endl;
	for (int i = 0; i < size; i++) {
		cout << X[i] << " ";
	}
	cout << endl;
	cout << endl;
}

void Particle::clearParticle()
{
	delete[] V;
	V = nullptr;

	delete[] X;
	X = nullptr;

	delete[] pbest;
	pbest = nullptr;
	pbestFitness = 0;
}

bool Particle::setValues(double ** source, double * dest)
{
	if (dest != nullptr && source != nullptr) {
		for (int i = 0; i < size; i++) {
			*(dest + i) = **(source + i);
		}
		return true;
	}
	return false;
}

int Particle::getSize() const
{
	return size;
}


double Particle::getXValue(int i) const
{
	return **(X+i);
}

void Particle::setV(double par[])
{
	for (int i = 0; i < size; i++) {
		*(V+i) = par[i];
	}
}

double Particle::getC1() const
{
	return c1;
}

double Particle::getC2() const
{
	return c2;
}

double Particle::getR1() const
{
	return r1;
}

double Particle::getR2() const
{
	return r2;
}

double Particle::getW() const
{
	return w;
}
