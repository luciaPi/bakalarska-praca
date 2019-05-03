#pragma once
#include <string>

using namespace std;

class Attribute {
	string name;

public:
	Attribute() {
	}
	Attribute(string pname)
	{
		name = pname;
	}

	~Attribute() {}

	string getName() const {
		return name;
	}

	Attribute(const Attribute& other) {
		name = other.name;
	}

	bool operator==(const Attribute& other) {
		return name == other.name;
	}

	void setValues(string pname) {
		name = pname;
	}	
};