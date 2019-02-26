//#define _CRTDBG_MAP_ALLOC
//#include <stdlib.h>
//#include <crtdbg.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <vector>
#include <stdarg.h>
#include <vld.h>
#include "FCMcounter.h"

using namespace std;



void vypisMaticu(float** matica) {
	int pocet = 10;
	int druhy = 3;
	for (int i = 0; i < pocet; i++) {
		for (int j = 0; j < druhy; j++) {
			cout << matica[i][j] << " ";
		}
		cout << endl;
	}
}

int main() {		
	//const char* fileName = "vysky.txt";
	const char* fileName = "iris.data";
	//const char* fileName = "som.data";
	FCMcounter fcm;
	fcm.count(fileName);

	return 0;
}
