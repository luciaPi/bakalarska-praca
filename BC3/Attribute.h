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

	/*const char* getTypes() const {
		return type;
	}*/

	/*int getLength() const {
		return len;
	}*/

	void setValues(string pname) {
		name = pname;/*
		string typeString = "{";
		for (int i = 0; i < ptype.size();i++) {
			if (i > 0) {
				typeString += ',';
			}
			typeString += ptype[i];			
		}
		typeString += '}';
		len = strlen(typeString.c_str())+1;
		type = new char[len];
		strcpy(type, typeString.c_str());*/
	}	
};