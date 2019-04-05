#include "Particle.h"
#include <iostream>

using namespace std;

Particle::Particle()
{
}

Particle::Particle(const Dataset & pdata,int parm, int parK, double parminchange, double parc1, double parc2, double parr1, double parr2, double parw, int parnumberOfClusters)
{
	init(parm,parK, parminchange, parc1, parc2, parr1, parr2, parw, parnumberOfClusters,&pdata);
}


Particle::~Particle()
{
	for (int i = 0; i < numberOfObjects; i++) {
		delete[] X[i];
		X[i] = nullptr;
	}
	delete[] X;
	X = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] V[i];
		V[i] = nullptr;
	}
	delete[] V;
	V = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] best[i];
		best[i] = nullptr;
	}
	delete[] best;
	best = nullptr;

	for (int i = 0; i < numberOfObjects; i++) {
		delete[] d[i];
		d[i] = nullptr;
	}
	delete[] d;
	d = nullptr;

	for (int j = 0; j < numberOfClusters; j++) {
		delete centers[j];
		centers[j] = nullptr;
	}
	delete[] centers;
	centers = nullptr;

	for (int j = 0; j < numberOfClusters; j++) {
		delete oldCenters[j];
		oldCenters[j] = nullptr;
	}
	delete[] oldCenters;
	oldCenters = nullptr;
}

void Particle::init(int parm, int parK, double parminchange, double parc1, double parc2, double parr1, double parr2, double parw,int parnumberOfClusters,const Dataset* pdata)
{
	numberOfClusters = parnumberOfClusters;
	m = parm;
	K = parK;
	minChange = parminchange;
	w = parw;
	c1 = parc1;
	c2 = parc2;
	r1 = parr1;
	r2 = parr2;

	if (pdata) {
		data = pdata;
		numberOfObjects = data->getSize();
		numberOfCoordinates = (*data)[0].getNumberOfCoordinates();
		XInit();
		VInit();
		dInit();
		bestInit();
		centersInit();

		fitnessBest = -1;
	}
}

void Particle::XInit()
{
	X = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		X[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = (double)rand() / RAND_MAX;
		}
	}
	normalizeX();
	/*double values[] = { 0.15, 0.45, 0.4, 0,0.5,0.5,0.25,0.75,0,1,0,0 ,0.25,0.75,0 };
	int which = 0;
	X = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		X[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = values[which];
			which++;
		}
	}*/

}

//inicializacia matice euklidovskych vzdialenosti
void Particle::dInit()
{
	d = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		d[i] = new double[numberOfClusters];
	}	
}

void Particle::bestInit()
{
	best = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		best[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			best[i][j] = X[i][j];
		}
	}
}

void Particle::VInit()
{
	V = new double*[numberOfObjects];
	for (int i = 0; i < numberOfObjects; i++) {
		V[i] = new double[numberOfClusters];
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = (double)rand() / RAND_MAX * 2.0 - 1.0;
		}
	}
}

//inicializacia centier
void Particle::centersInit()
{
	centers = new Object*[numberOfClusters];
	oldCenters = new Object*[numberOfClusters];
	for (int j = 0; j < numberOfClusters; j++) {
		Object *object = new Object();
		centers[j] = object;
		object = new Object();
		oldCenters[j] = object;
	}
}

double Particle::getFitness() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(X[i][j], m) * pow(d[i][j],1);
		}
	}

	return K / Jm;
}

void Particle::checkFitness(double ** gbest, double &gbestFitness)
{
	double newFitness = getFitness();
	if (newFitness > fitnessBest) {
		fitnessBest = newFitness;
		setMatrix(X,best);
		
		if (newFitness > gbestFitness) {
			setMatrix(best, gbest);
			gbestFitness = newFitness;
		}
	}
}

//vypocet centier
void Particle::computeCenters()
{
	vector<double> values;
	for (int j = 0; j < numberOfClusters; j++) {
		values.clear();
		for (int k = 0; k < numberOfCoordinates; k++) {
			double sum1 = 0;
			double sum2 = 0;
			for (int i = 0; i < numberOfObjects; i++) {
				double product2 = 1;
				for (int l = 0; l < m; l++) {
					product2 *= X[i][j];
				}
				double product1 = product2 * (*data)[i].getValue(k);
				sum1 += product1;
				sum2 += product2;
			}
			values.push_back(sum1 / sum2);
		}
		oldCenters[j] = centers[j];
		centers[j]->setValues(values);
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordinates; k++) {
				double a1 = centers[j]->getValue(k);
				double a2 = (*data)[i].getValue(k);
				if (abs(centers[j]->getValue(k) - (*data)[i].getValue(k)) < 0.00000001) {
					centers[j]->setValue(k, centers[j]->getValue(k) + 0.1*minChange);
				}
			}
		}

	}
}

//vypocet matice euklidovskej vzdialenosti
void Particle::computeD()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			double sum = 0;
			for (int k = 0; k < numberOfCoordinates; k++) {
				double coordinate1 = (*data)[i].getValue(k);
				double coordinate2 = centers[j]->getValue(k);
				sum += pow(coordinate1 - coordinate2, 2);
			}
			d[i][j] = sqrt(sum);
		}
	}
}

void Particle::computeV(double**gbest)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			V[i][j] = w * V[i][j] + c1 * r1*(best[i][j] - X[i][j]) + c2 * r2*(gbest[i][j] - X[i][j]);
		}
	}
}

void Particle::computeX()
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = X[i][j] + V[i][j];
		}
	}
}

//vypis centier
void Particle::centersPrint() const
{
	for (int j = 0; j < numberOfClusters; j++) {
		cout << "Centrum" << j + 1 << ": ";
		for (int k = 0; k < numberOfCoordinates; k++) {
			cout << centers[j]->getValue(k) << " ";
		}
		cout << endl;
	}

}

//vypis matice euklidovskej vzdialenosti
void Particle::dPrint() const
{	
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << d[i][j] << " ";
		}
		cout << endl;
	}
}

void Particle::Xprint()
{
	printMatrix(X, "X: ");
}

void Particle::Vprint() const
{
	printMatrix(V, "V: ");
}

void Particle::bestPrint() const
{
	printMatrix(best, "PBest: ");
}

void Particle::setMatrix(double ** source, double **dest)
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			dest[i][j] = source[i][j];
		}
	}
}

void Particle::setX(double ** newX)
{
	setMatrix(newX, X);
}

double Particle::getXValue(int i, int j) const
{
	return X[i][j];
}

void Particle::setV(double par[])
{
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			int x = i * numberOfClusters + j;
			V[i][j] = par[i*numberOfClusters+j];
		}
	}
}

void Particle::printMatrix(double** matrix, const char* text) const
{
	cout << text << endl;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

//nie nula!!!
void Particle::normalizeX()
{
	for (int i = 0; i < numberOfObjects; i++) {
		int count = 0;
		double rowSum = 0;
		for (int j = 0; j < numberOfClusters; j++) {
			if (X[i][j] < 0) {
				X[i][j] = 0.001;
				count++;
			}
			rowSum += X[i][j];
		}
		if (count == numberOfClusters) {
			for (int j = 0; j < numberOfClusters; j++) {
				if (X[i][j] < 0) {
					X[i][j] = rand() / RAND_MAX;
					count++;
				}
			}
		}
		for (int j = 0; j < numberOfClusters; j++) {
			X[i][j] = X[i][j] / rowSum;
		}
	}
}

void Particle::fitnessPrint() const
{
	cout << "Fitness: " << fitnessBest  << endl;
	cout << "Jm: " << getJm() << endl;
}

double Particle::getJm() const
{
	double Jm = 0;
	for (int i = 0; i < numberOfObjects; i++) {
		for (int j = 0; j < numberOfClusters; j++) {
			Jm += pow(X[i][j], m) * d[i][j];
		}
	}

	return Jm;
}