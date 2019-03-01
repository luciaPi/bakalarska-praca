#pragma once
#include <string>
#include <vector>

using namespace std;

class FCMcounter
{
private:
	struct Flower {
		vector<double> values;
		string name;
		int count;

		Flower(const char pname[] = " ") : name(pname), count(0) {	};

		//nastavi vektor suradnic
		void setValues(vector<double> pvalues) {
			values = pvalues;
			count = values.size();
		}

		//nastavi novu hodnotu na urcenom mieste
		void setValue(int which, double newValue) {
			values[which] = newValue;
		}

		//vrati vektor suradnic
		vector<double> getValues() {
			return values;
		}

		//vrati konkretnu suradnicu
		double getValue(int poradie) {
			if (poradie >= 0 && poradie < count) {
				return values[poradie];
			}
		}

		//nastavi  nazov zhluku
		void setName(const char pname[]) {
			name = pname;
		}

		//vrati nazov zhluku
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
	double** mu;
	Flower** centers;
	Flower** oldCenters;
	double** d;
	double minChange;

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
	void saveOutputToArff(const char* filename, char* title, char* creator, char* donor, char* relation);
};

