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
	particles = new ParticleCounterData*[P];
	for (int l = 0; l < P; l++) {
		particles[l] = new ParticleCounterData(data, m, K,minChange,c1,c2,r1,r2,w,numberOfClusters);
	}
	
}

void PSOcounter::compute()
{
	for (int l = 0; l < P; l++) {
		particles[l]->compute(gbest);
	}
}

void PSOcounter::Vprint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->Vprint();
	}
}

void PSOcounter::pbestsPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->pbestPrint();
	}
}

void PSOcounter::gbestPrint() const
{
	cout << "Gbest: " << endl;
	gbest->Xprint();
}

void PSOcounter::Xprint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->Xprint();
	}
}

void PSOcounter::centersPrint() const
{
	for (int l = 0; l < P; l++) {
		particles[l]->centersPrint();
	}
}

//vypis matice euklidovskej vzdialenosti
void PSOcounter::dPrint() const
{
	for (int l = 0; l < P; l++) {
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

void PSOcounter::printJm() const
{
	gbest->printJm();
}

bool PSOcounter::wasSignificantChange() const
{
	for (int l = 0; l < P; l++) {
		if (particles[l]->wasSignificantChange()) {
			return true;
		}
	}
	return false;
}

const ParticleCounterData* PSOcounter::getBest() const
{
	return gbest;
}

double PSOcounter::getJm() const
{
	return gbest->getJm();
}
