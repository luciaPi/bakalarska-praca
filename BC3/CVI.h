#pragma once
#include "FuzzyData.h"

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
		
public:
	CVI();
	~CVI();

	bool count(FuzzyData* data);
	
	
};

