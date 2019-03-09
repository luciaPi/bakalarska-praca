#pragma once
#include <string>

using namespace std;

class Attribute {
	const char* name;
	char* type;
	int len;

public:
	Attribute() {
	}
	Attribute(char* pname, vector<string> ptype)
	{
		setValues(pname, ptype);
	}

	~Attribute() {
		delete type;
		type = nullptr;
	}

	const char* getName() const {
		return name;
	}

	const char* getTypes() const {
		return type;
	}

	/*int getLength() const {
		return len;
	}*/

	void setValues(char* pname, vector<string> ptype) {
		name = pname;
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
		strcpy(type, typeString.c_str());
	}	
};