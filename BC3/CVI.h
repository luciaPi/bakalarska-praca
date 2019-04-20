#pragma once
#include "FuzzyData.h"
#include "FCMcounter.h"
#include <string>

enum IndexEnum {
	PC = 0, MPC = 1, PE = 2, FS = 3, XIEBENI = 4,
	KWON = 5, FHV = 6, PBMF = 7, PCAES = 8, SILHOUETTE = 9
};

class CVI
{
	struct Index {
		string name = "";
		double value = -1;
		bool count = false;
		int numberOfReplication = 0;
		double sum = 0;
		double sumSquared = 0;

		/*bool countIndex() {
			return count;
		}*/
		Index(string pname) {
			name = pname;
		}
		void setToCount() {
			count = true;
			numberOfReplication = 0;
			value = -1;
			sum = 0;
			sumSquared = 0;
		}
		void unsetToCount() {
			count = false;
		}
		void setName(string pname) {
			name = pname;
		}
		Index& operator=(double pvalue) {
			value = pvalue;
			sum += value;
			sumSquared += pow(value, 2);
			numberOfReplication++;
			return *this;
		}
		Index& operator=(int pvalue) {
			value = pvalue;
			sum += value;
			sumSquared += pow(value, 2);
			numberOfReplication++;
			return *this;
		}
		Index& operator=(bool pvalue) {
			count = pvalue;
			setToCount();
			return *this;
		}
		string getResult(double t) const {
			string result = "";
			if (numberOfReplication > 0) {
				double average = sum / numberOfReplication;
				char averageString[20];
				sprintf(averageString,"%.03f",average);
				result += averageString;
				if (numberOfReplication >= 30) {
					double s = sqrt(abs((sumSquared / (numberOfReplication - 1)) - pow(sum / (numberOfReplication - 1), 2)));
					double interval = (s * t) / sqrt(numberOfReplication);
					sprintf(averageString, "+-%.02f", interval);
					result += averageString;
				}
			}
			return result;
		}
		explicit operator bool() const {
			return count;
		}
		operator int() const {
			return value;
		}
		operator double() const {
			return value;
		}
		operator std::string() const {
			return name;
		}
	};

	int titleSize = 13;
	int algoritmusSize = 10;
	int widthCVI = 15;

	/*double PCvalue = -1;
	double MPCvalue = -1;
	double PEvalue = -1;
	double FSvalue = -1;
	double XIEBENIvalue = -1;
	double KWONvalue = -1;
	double FHVvalue = -1;
	double PBMFvalue = -1;
	double PCAESvalue = -1;
	double SILHOUETTEvalue = -1;*/

	int number = 10;
	FuzzyData *fuzzydata = nullptr;
	vector<Attribute*> clusters;
	vector<Index> indices;

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
	Object* getMainCenter();
	double getMinBeta(int which) const;
	double getMuM() const;
	double getBeta() const;
	double getA(int which) const;
	double getB(int which) const;
	double** getF(double** F,int which);
	double getDeterminant(double** F);

	void setIndexNames();
		
public:
	CVI();
	CVI(int seed1, int seed2);
	~CVI() {};

	bool count(FuzzyData* data, Dataset dataset);
	
	void setIndexToCount(IndexEnum index);
	void unsetIndexToCount(IndexEnum index);
	void setAllIndecesToCount();

	void printResultsHeader(ostream& output) const;
	void printResults(ostream& output, string title, double t) const;

	void setAssignedClusters(vector<Attribute*> clusters);
};

