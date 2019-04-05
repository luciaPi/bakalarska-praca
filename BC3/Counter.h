#pragma once
#include "Dataset.h"
#include "Attribute.h"

class Counter
{
private:
	Dataset *data;

	bool readDataFromFile(const char* name);
	void dataObjectsPrint() const;
	
public:
	Counter() : data(nullptr) {};
	~Counter();

	enum class Algorithm { fcm };	

	void count(Algorithm alg, const char* name);
	void saveOutputToArff(const char* filename, char* title, char* creator, char* donor, char* relation, vector<Attribute*> attributes) const;
};

