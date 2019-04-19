#include "Object.h"
#include <iostream>

using namespace std;

Object & Object::operator=(const Object & other)
{
	numberOfCoordinates = other.numberOfCoordinates;
	values.clear();
	for (double cur : other.values) {
		values.push_back(cur);
	}
	return *this;
}

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
double Object::getValue(int which) const {
	if (which >= 0 && which < numberOfCoordinates) {
		return values[which];
	}
}

double *Object::getPointerValue(int which)
{
	if (which >= 0 && which < numberOfCoordinates) {
		return  &values[which];			
	}
}

//nastavi  nazov zhluku
void Object::setObjectClass(Attribute* pobjectClass) {
	objectClass = pobjectClass;
}

//vrati nazov zhluku
Attribute* Object::getObjectClass() const {
	return objectClass;
}

string Object::getName() const
{
	return objectClass->getName();
}

void Object::setObjectAssignedClass(Attribute * pobjectClas)
{
	objectAssignedClass = pobjectClas;
}

Attribute * Object::getObjectAssignedClass() const
{
	return objectAssignedClass;
}

string Object::getAssignedName() const
{
	return objectAssignedClass->getName();
}

int Object::getNumberOfCoordinates() const
{
	return numberOfCoordinates;
}

void Object::addToValues(double number)
{
	for (int i = 0; i < numberOfCoordinates; i++) {
		values[i]+=number;
	}
}

void Object::printValues() const
{
	for (double cur : values) {
		cout << cur << " ";
	}
	cout << endl;
}
