#include <vld.h>
#include <vector>
#include "Application.h"
#include <iostream>
#include <string>
#include "RandomGenerator.h"

using namespace std;

int main() {		
	//const char* fileName = "vysky.txt";
	//const char* fileName = "fcmexample.txt";
	//const char* fileName = "iris.arff";
	const char* fileName = "iris.data";
	//const char* fileName = "parkinsons.data";
	//const char* fileName = "glass_csv.csv";
	//const char* fileName = "fifa.csv";
	//const char* fileName = "wine.data";

	//string fileName = "";
	//getline(cin, fileName);
	int numberOfReplication = 3;
	//cin >> numberOfIteration;
	
	/*std::random_device rd;
	RandomGenerator rn(rd(),0,1);
	for (int n = 0; n < 10; ++n) {
		std::cout << rn.nextRandom() << ' ';
	} cout << endl;
	RandomGenerator rn2(rd(),1,2);
	for (int n = 0; n < 10; ++n) {
		std::cout << rn2.nextRandom() << ' ';
	} cout << endl;*/

	Application counter;
	counter.setNumberOfClusters(3);
	//counter.setData(fileName.c_str());
	counter.setData(fileName);

	//counter.setMuInitializationMode(MuInitializationMode::fcmPlusPlus);

	counter.setNumberOfReplications(numberOfReplication);
	counter.count(Algorithm::fcm,100);
	/*counter.count(Algorithm::pso,100);
	counter.count(Algorithm::fcmpso,3);
	counter.count(Algorithm::fa,20);
	counter.count(Algorithm::fafcm,1);*/

 	return 0;
}
