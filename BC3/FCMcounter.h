#pragma once
#include <string>
#include <vector>

using namespace std;

class FCMcounter
{
private:
	struct Flower {
		vector<float> values;
		string name;
		int count;

		Flower(char pname[] = " ") : name(pname), count(0) {	};

		void setValues(vector<float> pvalues) {
			values = pvalues;
			count = values.size();
		}

		vector<float> getValues() {
			return values;
		}

		float getValue(int poradie) {
			if (poradie >= 0 && poradie < count) {
				return values[poradie];
			}
		}

		void setName(char pname[]) {
			name = pname;
		}

		string getName() {
			return name;
		}

		~Flower() {
		};
	};
	int m;
	int numberOfCoordinates;
	int numberOfClusters;
	int numberOfObjects;
	vector<Flower*> all;
	float** mu;
	Flower** centers;
	Flower** oldCenters;
	float** d;
	float minChange;

	bool readDataFromFile(const char* name);
	void flowersPrint();
	void flowersPrintWithType();

	void muInit();
	void muPrint();
	void computeMu();

	void centersInit();
	void centersPrint();
	void computeCenters();	

	void dInit();
	void dPrint();
	void computeD();

	bool isSignificantChange();
	int whichCenter(Flower &flower);
	int whichNumberOfObject(Flower &flower);

public:
	FCMcounter();
	~FCMcounter();	
	void count(const char* name);
};

