#pragma once
#include "Object.h"

using namespace std;

class Dataset
{
private:	
	int numberOfObjects;

	vector<Object*> all;

public:
	Dataset() : numberOfObjects(0) {};
	~Dataset();
	void add(Object* object);
	int getSize() const;
	Object& operator[](const int index) {
		Object* object = all[index];
		return *object;
	};
	const Object& Dataset::operator[](const int index) const;
	int whichNumberOfObject(const Object & object) const;
};

