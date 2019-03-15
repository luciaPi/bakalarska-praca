#pragma once
#include <vector>

using namespace std;

class Object {
private:
	int numberOfCoordinates;
	vector<double> values;
	string name;

public:
	Object(const char pname[] = " ") : name(pname), numberOfCoordinates(0) {	};
	void setValues(vector<double> pvalues);
	void setValue(int which, double newValue);
	vector<double> getValues();
	double getValue(int poradie) const;
	void setName(const char pname[]);
	string getName();
	int getNumberOfCoordinates() const;
	void printValues() const;
	~Object();
};
