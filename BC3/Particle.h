#pragma once

class Particle 
{
private:
	double c1 = 1;
	double c2 = 1;
	double r1 = -1;
	double r2 = -1;
	double w = 1;

	const double maxV = 1;
	
	double pbestFitness = 0;

	double* gbest = nullptr;
	double* gbestFitness = nullptr;

	void pbestInit();
	void VInit();
	void Xinit();
	void init();
	
	void computeV();
	void computeX();
	void checkFitness();
	
	void clearParticle();

	bool setValues(double *source, double *dest);

protected:
	double* pbest = nullptr;
	double* X = nullptr;
	double* V = nullptr;
	int size = 0;

	virtual void normalize() = 0;
	virtual double getFitness() = 0;
	virtual bool setX() = 0;

public:
	Particle();
	Particle(double c1,double c2, double r1, double r2, double w, int size);
	~Particle();

	void Vprint() const;	
	void pbestPrint() const;
	void Xprint() const;

	bool compute();
	
	void setc1c2(double c1, double c2);
	void setr1r2(double r1, double r2);
	void setw(double w);
	void setSize(int size);
		
	//double getXValue(int i, int j) const;
	//void setV(double par[]);
};

