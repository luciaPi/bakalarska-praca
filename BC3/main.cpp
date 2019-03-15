#include <vld.h>
#include <vector>
#include "Counter.h"
#include <iostream>

using namespace std;

int main() {		
	//const char* fileName = "vysky.txt";
	const char* fileName = "iris.data";
	//const char* fileName = "som.data";
	Counter counter;
	counter.count(Counter::Algorithm::fcm,fileName);
	/*char* title = "Iris Plants Database";
	char* creator = "Lucia Piatrikova";
	char* donor = " ";
	char* relation = "iris";

	vector<string> type;
	type.push_back("numeric");
	vector<string> type2;
	type2.push_back("Iris - setosa");
	type2.push_back("Iris - versicolor");
	type2.push_back("Iris - virginica");
	vector<Attribute*> attributes;
	/*attributes[0].setValues("sepallength", type);
	attributes[1].setValues("sepalwidth", type);
	attributes[2].setValues("petallength", type);
	attributes[3].setValues("petalwidth", type);
	attributes[4].setValues("class", type2);*/
	/*attributes.push_back(new Attribute("sepallength", type));
	attributes.push_back(new Attribute("sepalwidth", type));
	attributes.push_back(new Attribute("petallength", type));
	attributes.push_back(new Attribute("petalwidth", type));
	attributes.push_back(new Attribute("class", type2));

	counter.saveOutputToArff("iris.arff",title,creator,donor,relation,attributes);

	while (attributes.size() > 0) {
		Attribute* last = attributes.back();
		delete last;
		attributes.pop_back();
	}*/
	cout << "end";
 	return 0;
}
