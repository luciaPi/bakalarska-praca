#pragma once
#include <vector>
#include "Attribute.h"

using namespace std;

class Object {
private:
	int numberOfCoordinates;
	vector<double> values;
	Attribute* objectClass = nullptr;
	Attribute* objectAssignedClass = nullptr;

public:
	Object() : numberOfCoordinates(0) {};
	~Object() {};
	Object& operator=(const Object& other);

	void setValues(vector<double> pvalues);
	void setValue(int which, double newValue);
	vector<double> getValues();
	double getValue(int which) const;
	double* getPointerValue(int which);
	void printValues() const;

	int getNumberOfCoordinates() const;

	void addToValues(double number);

	void setObjectClass(Attribute* objectClas);
	Attribute*getObjectClass() const;	
	string getName() const;
	void setObjectAssignedClass(Attribute* objectClas);
	Attribute*getObjectAssignedClass() const;
	string getAssignedName() const;
};
