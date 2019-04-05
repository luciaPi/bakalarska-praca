#pragma once
#include <vector>

using namespace std;

class Object {
private:
	int numberOfCoordinates;
	vector<double> values;
	//string name;

public:
	Object() : numberOfCoordinates(0) {};
	~Object() {};
	Object& operator=(const Object& other);

	void setValues(vector<double> pvalues);
	void setValue(int which, double newValue);
	//vector<double> getValues();
	double getValue(int poradie) const;
	void printValues() const;

	int getNumberOfCoordinates() const;

	//void setName(const char pname[]);
	//string getName();	
};
