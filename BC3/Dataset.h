#pragma once
#include "Object.h"

using namespace std;

class Dataset
{
private:	
	int numberOfObjects = 0;
	vector<Object*> all;

public:
	Dataset() {};
	Dataset(const Dataset& other);
	~Dataset();
	Dataset& operator=(const Dataset& other);

	void add(Object* object);
	int getSize() const;

	Object& operator[](const int index);
	const Object Dataset::operator[](const int index) const;

	double getMinCoordinate(int which) const;
	double getMaxCoordinate(int which) const;

	//int whichNumberOfObject(const Object & object) const;
};

