#pragma once
#include "FuzzyData.h"
#include "FCMcounter.h"

enum class Index {
	PC = 0, MPC = 1, PE = 2, FS = 3, XIEBENI = 4,
	KWON = 5, FHV = 6, PBMF = 7, PCAES = 8, SILHOUETTE = 10
};

class CVI
{
	double PCvalue = -1;
	double MPCvalue = -1;
	double PEvalue = -1;
	double FSvalue = -1;
	double XIEBENIvalue = -1;
	double KWONvalue = -1;
	double FHVvalue = -1;
	double PBMFvalue = -1;
	double PCAESvalue = -1;
	double SILHOUETTEvalue = -1;

	int number = 10;
	bool* whichCount = nullptr;
	FuzzyData *fuzzydata = nullptr;
	vector<Attribute*> clusters;

	int numberOfObjects = 0;
	int numberOfClusters = 0;
	int numberOfCoordinates = 0;

	Object** centers = nullptr;
	const double** mu = nullptr;
	Dataset data;

	FCMcounter fcmCounter;

	double countPC();
	double countMPC();
	double countPE();
	double countFS();
	double countXIEBENI();
	double countKWON();
	double countFHV();
	double countPBMF();
	double countPCAES();
	double countSILHOUETTE();

	double getMinCentersDistance() const;
	double getMaxCentersDistance() const;
	double getJmDifferent() const;
	Object** getMainCenter();
	double getMinBeta(int which) const;
	double getMuM() const;
	double getBeta() const;
	double getA(int which) const;
	double getB(int which) const;
	double** getF(double** F,int which);
	double getDeterminant(double** F);
		
public:
	CVI();
	CVI(int seed1, int seed2);
	~CVI();

	bool count(FuzzyData* data);
	
	
};

