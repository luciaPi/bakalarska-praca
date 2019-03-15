#include "Object.h"
#include <iostream>

using namespace std;

//nastavi vektor suradnic
void Object::setValues(vector<double> pvalues) {
	values = pvalues;
	numberOfCoordinates = values.size();
}

//nastavi novu hodnotu na urcenom mieste
void Object::setValue(int which, double newValue) {
	values[which] = newValue;
}

//vrati vektor suradnic
vector<double> Object::getValues() {
	return values;
}

//vrati konkretnu suradnicu
double Object::getValue(int poradie) const {
	if (poradie >= 0 && poradie < numberOfCoordinates) {
		return values[poradie];
	}
}

//nastavi  nazov zhluku
void Object::setName(const char pname[]) {
	name = pname;
}

//vrati nazov zhluku
string Object::getName() {
	return name;
}

int Object::getNumberOfCoordinates() const
{
	return numberOfCoordinates;
}

void Object::printValues() const
{
	for (double cur : values) {
		cout << cur << " ";
	}
	cout << endl;
}

Object::~Object() {
};

