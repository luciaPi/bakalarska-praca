#include "Counter.h"
#include <iostream>
#include <time.h>
#include <ctime>
#include <string>
#include <vector>
#include "Object.h"
#include "Attribute.h"
#include "FCMcounter.h"

using namespace std;

Counter::~Counter()
{
	delete data;
	data = nullptr;
}

void Counter::count(Algorithm alg, const char * name)
{
	readDataFromFile(name);
	FCMcounter fcmCounter;
	fcmCounter.count(data);
}

void Counter::saveOutputToArff(const char * filename, char* title, char* creator, char* donor, char* relation, vector<Attribute*> attributes) const
{
	FILE* datafile;
	if ((datafile = fopen(filename, "w")) == NULL) {
		cout << "CHYBA" << endl;
	}
	else {
		char* titles[] = {
		"1. Title",
		"2. Sources",
		"(a) Creator",
		"(b) Donor",
		"(c) Date",
		"@RELATION",
		"@ATTRIBUTE",
		"@DATA"
		};
		char shape = '%';
		time_t now = time(0);
		tm* date = localtime(&now);

		int which = 0;
		fprintf(datafile, "%c %s: ", shape, titles[which++]);
		fprintf(datafile, "%s\n%c\n", title, shape);
		fprintf(datafile, "%c %s:\n", shape, titles[which++]);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++], creator);
		fprintf(datafile, "%c\t%s: %s\n", shape, titles[which++], donor);
		fprintf(datafile, "%c\t%s: %d\-%d-%d\n", shape, titles[which++], date->tm_mday, date->tm_mon, date->tm_year + 1900);
		fprintf(datafile, "%c\n\n", shape);
		fprintf(datafile, "%s %s\n\n", titles[which++], relation);


		for (int i = 0; i < attributes.size(); i++) {
			const char* actual = attributes[i]->getName();
			fprintf(datafile, "%s %s ", titles[which], attributes[i]->getName());
			fprintf(datafile, "%s\n", attributes[i]->getTypes());
		}
		fprintf(datafile, "\n%s\n", titles[++which]);

		fclose(datafile);
	}
}

//nacitanie vstupnych dat
bool Counter::readDataFromFile(const char* fileName)
{
	FILE* datafile;
	if ((datafile = fopen(fileName, "r")) == NULL) {
		cout << "CHYBA" << endl;
		return false;
	}
	else {
		data = new Dataset();

		vector<double> values;
		float value;
		int returnVal;
		Object *flower = new Object();
		int lastPosition;
		char name[100];
		int counChars = 0;

		do {
			//lastPosition = ftell(datafile);
			if ((returnVal = fscanf(datafile, "%f", &value)) == EOF) {
				break;
			}
			if (returnVal == 0) {
				//fseek(datafile, lastPosition, SEEK_SET);				
				/*fscanf(datafile, "%s", &name);
				flower->setName(name);
				flower->setValues(values);
				numberOfCoordinates = values.size();
				all.push_back(flower);
				flower = new Object();
				values.clear();*/
				fscanf(datafile, "%c", &name);
				counChars++;
			}
			else {
				//fscanf(datafile, "%c",&name);
				if (counChars > 2) {
					flower->setValues(values);
					data->add(flower);
					flower = new Object();
					values.clear();
				}
				values.push_back(value);
				counChars = 0;
			}
		} while (true);
		flower->setValues(values);
		data->add(flower);

		fclose(datafile);
	}
	return true;
}

//vypis vstupnych dat
void Counter::flowersPrint() const
{
	cout << "Data:" << endl;
	int numberOfCoordiantes;
	int numberOfObjects;
	if (numberOfObjects = data->getSize() > 0) {
		numberOfCoordiantes = (*data)[0].getNumberOfCoordinates();
		for (int i = 0; i < numberOfObjects; i++) {
			for (int k = 0; k < numberOfCoordiantes; k++) {
				cout << (*data)[i].getValue(k) << (k < numberOfCoordiantes - 1 ? "," : " ");
			}
			cout << (*data)[i].getName() << endl;
		}
	}
}