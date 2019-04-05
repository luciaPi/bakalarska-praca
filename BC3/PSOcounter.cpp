#include "PSOcounter.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

using namespace std;

void PSOcounter::init(const Dataset* pdata)
{
	srand(time(NULL));

	K = 2;
	m = 2;
	c1 = 2;
	c2 = 2;
	r1 = (double) rand() / RAND_MAX;
	//r1 = 0.5;
	r2 = (double) rand() / RAND_MAX;
	//r2 = 0.5;
	w = 0.5;
	
	maxIterationNumber = 100;
	if (pdata) {
		data = pdata;
	}
	minChange = 0.1;
	P = 10;
	numberOfClusters = 3;
	particles = nullptr;
	gbestX = nullptr;
}

PSOcounter::PSOcounter()
{
	init();
}

PSOcounter::~PSOcounter()
{
	removeParticles();	

	int numberOfObjects = data->getSize();
	for (int i = 0; i < numberOfObjects; i++) {
		delete[] gbestX[i];
		gbestX[i] = nullptr;
	}
	delete[] gbestX;
	gbestX = nullptr;	
}

PSOcounter::PSOcounter(const Dataset& pdata)
{
	init(&pdata);
}

void PSOcounter::count(const Dataset * pdata)
{
	if (pdata) {
		init(pdata);
	}
	if (data) {
		if (data->getSize() > 0) {
			int numberOfIterations = 0;

			removeParticles();
			particlesInit();
			//setV();
			/*dPrint();
			Vprint();
			Xprint();
			bestsPrint();*/
			
			gbestInit();
			gbestPrint();

			computeCenters();
			centersPrint();
			fitnessPrint();

			int i = 1;
			do {	
				//cout << "Round" << i++ << endl;
				computeD();
				//dPrint();

				checkFitness();	
				//bestsPrint();
				//gbestPrint();
				//fitnessPrint();

				computeV();
				computeX();
				normalizeX();

				//Vprint();
				//Xprint();

				computeCenters();
				//centersPrint();
			} while (numberOfIterations++ < maxIterationNumber);
		}
		centersPrint();
		gbestPrint();
		printbestCentre();
		printJm();
		//Xprint();
		//Vprint();
	}
}

void PSOcounter::particlesInit()
{
	particles = new Particle*[P];
	for (int l = 0; l < P; l++) {
		particles[l] = new Particle(*data, m, K,minChange,c1,c2,r1,r2,w,numberOfClusters);
	}
	
}

void PSOcounter::gbestInit()
{
	int numberOfObjects = data->getSize();
	gbestX = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		gbestX[i] = new double[numberOfClusters];
	}
	gbestFitness = -1;
	checkFitness();
}

void PSOcounter::computeV()
{
	for (int l = 0; l < P; l++) {
		particles[l]->computeV(gbestX);
	}
}

void PSOcounter::Vprint() const
{
	cout << "Rychlost V: " << endl;
	for (int l = 0; l < P; l++) {
		cout << "Rychlost V"<< l+1 << ": " << endl;
		particles[l]->Vprint();
	}
}

void PSOcounter::checkFitness()
{
	for (int l = 0; l < P; l++) {
		particles[l]->checkFitness(gbestX, gbestFitness);
	}
}

void PSOcounter::bestsPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->bestPrint();
	}
}

void PSOcounter::gbestPrint() const
{
	cout << "Gbest: " << endl;
	int numberOfObjects = data->getSize();
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << gbestX[i][j] << " ";
		}
		cout << endl;
	}
}

void PSOcounter::fitnessPrint() const
{	
	for (int l = 0; l < P; l++) {
		cout << "Fitness "<< l+1 << ": " << endl;
		particles[l]->fitnessPrint();
	}

	cout << "Fitness gbest " << gbestFitness << ": " << endl;
	cout << "Jm: " << 1 / (gbestFitness / K) << endl;
}

void PSOcounter::computeX()
{
	for (int l = 0; l < P; l++) {
		particles[l]->computeX();
	}
}

void PSOcounter::normalizeX()
{
	for (int l = 0; l < P; l++) {
		particles[l]->normalizeX();
	}
}

void PSOcounter::Xprint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->Xprint();
	}
}

//vypocet centier
void PSOcounter::computeCenters()
{
	for (int l = 0; l < P; l++) {
		particles[l]->computeCenters();
	}
}

void PSOcounter::centersPrint() const
{
	cout << "Centra:" << endl;
	for (int l = 0; l < P; l++) {
		cout << "centra " << l + 1 << ": " << endl;
		particles[l]->centersPrint();
	}
}

//vypocet matice euklidovskej vzdialenosti
void PSOcounter::computeD()
{
	for (int l = 0; l < P; l++) {
		particles[l]->computeD();
	}
}

//vypis matice euklidovskej vzdialenosti
void PSOcounter::dPrint() const
{
	cout << "Matica euklidovskej vzdialenosti:" << endl;
	for (int l = 0; l < P; l++) {
		cout << "Matica " << l+1 << ": " << endl;
		particles[l]->dPrint();
	}
}

void PSOcounter::removeParticles()
{
	if (particles != nullptr) {
		for (int l = 0; l < P; l++) {
			delete particles[l];

		}
		delete[] particles;
		particles = nullptr;
	}
}

void PSOcounter::setV()
{
	double v1[] = {0.96,-0.4,-0.75,0.34,-0.19,-0.42,-0.42,-0.89,0.33,0.83,-0.79,0.47,0.28,0.61,-0.12};
	particles[0]->setV(v1);
	double v2[] = {0.93,-0.81,-0.22,-0.69,-0.2,-0.85,-0.61,0.6,-0.1,0.03,-0.05,-0.83,-0.27,0.81,-0.03};
	particles[1]->setV(v2);
}

//vypocet centier
void PSOcounter::printbestCentre() const
{
	cout << "Gbest centra:" << endl;
	int numberOfCoordinates = (*data)[0].getNumberOfCoordinates();
	int numberOfObjects = data->getSize();
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum " << j + 1 << ":";
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum1 = 0;
			double sum2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				double product2 = 1;
				for (int l = 0; l < m; l++) {
					product2 *= gbestX[i][j];
				}
				double product1 = product2 * (*data)[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			cout <<  sum1 / sum2 << " ";
		}
		cout << endl;
	}
}

void PSOcounter::printJm() const
{
	cout << "Jm PSO = ";
	cout << 1/(gbestFitness / K) << endl;
}

const double ** PSOcounter::getBestX() const
{
	return (const double**)gbestX;
}
