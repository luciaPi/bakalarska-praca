#include <vld.h>
#include "FCMcounter.h"

using namespace std;

int main() {		
	const char* fileName = "vysky.txt";
	//const char* fileName = "iris.data";
	//const char* fileName = "som.data";
	FCMcounter fcm;
	fcm.count(fileName);
	char* title = "Iris Plants Database";
	char* creator = "Lucia Piatrikova";
	char* donor = " ";
	char* relation = "iris";
	fcm.saveOutputToArff("iris.txt",title,creator,donor,relation);

	return 0;
}
