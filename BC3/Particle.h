#pragma once
//#include "ParticleCounterData.h"

class Particle 
{
private:
	double c1 = 1;
	double c2 = 1;
	double r1 = -1;
	double r2 = -1;
	double w = 1;

	const double maxV = 1;
	
	void pbestInit();
	void VInit();
	void Xinit();
	
	void computeV(const Particle* gbest);
	void computeX();
	void checkFitness(const Particle* gbest);
	
	void clearParticle();

	double getXValue(int i) const;
	bool setValues(double *source, double *dest);

	int getSize() const;

protected:
	double* pbest = nullptr;
	double pbestFitness = 0;

	double* X = nullptr;
	double* V = nullptr;
	int size = 0;

	virtual void normalize() = 0;
	virtual double getFitness() const = 0;
	virtual bool setX() = 0;
	
	void init();

	void setStartingX();

public:
	Particle();
	Particle(double c1,double c2, double r1, double r2, double w, int size);
	~Particle();

	void Vprint() const;	
	void pbestPrint() const;
	void Xprint() const;

	bool compute(const Particle* gbest);
	
	void setc1c2(double c1, double c2);
	void setr1r2(double r1, double r2);
	void setw(double w);
	void setSize(int size);
		
	void setV(double par[]);	//dat prec
};

