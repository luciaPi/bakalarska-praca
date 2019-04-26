#include <vld.h>
#include <vector>
#include "Application.h"
#include <iostream>
#include <string>

using namespace std;

int main() {		
	//const char* fileName = "vysky.txt";
	//const char* fileName = "fcmexample.txt";
	//const char* fileName = "iris.arff";
	//const char* fileName = "iris.data";
	//const char* fileName = "parkinsons.data";
	//const char* fileName = "glass_csv.csv";
	//const char* fileName = "wine.data";
	//const char* fileName;	

	string fileName = "", title=""; 
	int mod;
	int modVypoctu;
	int pocetReplikacii = 1;
	int algoritmy[5];
	string menaAlgoritmov[] = { "FCM","PSO","FCM-PSO","FA","FAFCM" };
	int pocetAlgoritmov = 5;
	int CVIarray[10];
	string menaCVI[] = { "PC","MPC","PE","FS","XIEBENI","KWON","FHV","PBMF","PCAES","SILHOUETTE" };
	int pocetCVI = 10;
	int pocetZhlukov;
	int pocetZhlukovOd;
	int pocetZhlukovDo;
	int vsetkyIndexy;
	int vsetkyVysledkyDoSuboru;
	int pripona;
	int vysledkyCVIfile;
	int initMode;
	int m, K, w, Ppso, Pfa;
	double c1, c2, r1, r2, maxV, alpha, beta, gamma;
	int defPSO, defFA;
	int intervalSpolahlivosti;
	int nastavitNazov;
	

	Application counter;
	cout << "METAFUZZY" << endl;
	cout << "Nastavenie parametrov" << endl;

	cout << "Nazov suboru s datami: ";
	getline(cin, fileName);
	counter.setData(fileName.c_str());

	do {
		cout << "Nastavit nazov datasetu (1-ano, 0-pouzit prednastaveny) : ";
		cin >> nastavitNazov;
	} while (nastavitNazov != 1 && nastavitNazov != 0);

	if (nastavitNazov == 1) {
		cout << "Nazov datasetu: ";
		getline(cin, title);
		counter.setTitle((char*)title.c_str());
	}
	
	//aj nazvy atriboutov
	//cluster name
	//final criterion, minchange
	//clearclusterattribute

	do {
		cout << "Mod (1-Jedna replikácia, 2-Viac replikacii): ";
		cin >> mod;
	} while (mod != 1 && mod != 2);	//pri mod = 1 vypise aj vystup
	if (mod == 2) {
		do {
			cout << "Pocet replikacii: ";
			cin >> pocetReplikacii;
		} while (pocetReplikacii < 0);
		counter.setNumberOfReplications(pocetReplikacii);
	}
	cout << "Algoritmy (1-pouzit, 0-nepouzit)" << endl;
	for (int i = 0; i < pocetAlgoritmov; i++) {
		do {
			cout << menaAlgoritmov[i] <<  ": ";
			cin >> algoritmy[i];
		} while (algoritmy[i] != 1 && algoritmy[i] != 0); //podla toho pocitat
	}
		
	do {
		cout << "Konstanta m (m >= 1): ";	//dat defulty
		cin >> m;
	} while (m < 1);
	counter.setM(m);

	bool jeMetaheuristika = false;
	for (int i = 1; i < pocetAlgoritmov; i++) {
		if (algoritmy[i] == 1) {
			jeMetaheuristika = true;
			break;
		}
	}
	if (jeMetaheuristika) {
		do {
			cout << "Konstanta K (K >= 1, pre vypocet fitness funkcie): ";
			cin >> K;
		} while (K < 1);
		counter.setK(K);
	}

	bool defaultPSO = true;
	cout << "Prednastavene hodnoty konstant PSO: w=1,c1=1,c2=1,r1=0.5,r2=0.5,maxV=0.0001,P=10" << endl;
	do {
		cout << "Nastavit vlastne hodnoty konstant pre PSO (1-ano, 0-nie): ";
		cin >> defPSO;
	} while (defPSO != 1 && defPSO != 0);
	defaultPSO = defPSO == 1 ? false: true;

	if ((algoritmy[pso] || algoritmy[fcmpso]) && !defaultPSO) {
		do {
			cout << "Konstanta w (w >= 0): ";
			cin >> w;
		} while (w < 0);
		counter.setW(w);
		do {
			cout << "Konstanta c1 (c1 >= 0): ";
			cin >> c1;
		} while (c1 < 0);
		counter.setC1(c1);
		do {
			cout << "Konstanta c2 (c2 >= 0): ";
			cin >> c2;
		} while (c2 < 0);
		counter.setC2(c2);
		do {
			cout << "Konstanta r1 <0,1>: ";
			cin >> r1;
		} while (r1 > 1 || r1 < 0);
		counter.setR1(r1);
		do {
			cout << "Konstanta r2 <0,1>: ";
			cin >> r2;
		} while (r2 > 1 || r2 < 0);
		counter.setR2(r2);
		do {
			cout << "Maximalna rychlost V (V > 0): ";
			cin >> maxV;
		} while (maxV <= 0);
		counter.setMaxV(maxV);
		do {
			cout << "Velkost populacie P (P > 0): ";
			cin >> Ppso;
		} while (Ppso <= 0);
		counter.setSizeOfPopulationPSO(Ppso);
	}

	bool defaultFA = true; // zmenit prednastavene hodnoty  
	cout << "Prednastavene hodnoty konstant PSO: w=1,c1=1,c2=1,r1=0.5,r2=0.5,maxV=0.0001,P=10" << endl;
	do {
		cout << "Nastavit vlastne hodnoty konstant pre FA (1-ano, 0-nie): ";
		cin >> defFA;
	} while (defFA != 1 && defFA != 0);
	defaultFA = defFA == 1 ? false : true;
	if ((algoritmy[fa] || algoritmy[fafcm]) && !defaultFA) {
		do {
			cout << "Konstanta alpha <0,1>: ";
			cin >> alpha;
		} while (alpha > 1 || alpha < 0);
		counter.setAlpha(alpha);
		do {
			cout << "Konstanta beta (beta >= 0): ";
			cin >> beta;
		} while (beta > 1 || beta < 0);
		counter.setBeta(beta);
		do {
			cout << "Konstanta gama (gama >= 0): ";
			cin >> gamma;
		} while (gamma < 0);
		counter.setGamma(gamma);
		do {
			cout << "Velkost populacie P (P > 0): ";
			cin >> Pfa;
		} while (Pfa <= 0);
		counter.setSizeOfPopulationFA(Pfa);
	}

	do {
		cout << "Mod vypoctu (1-Jeden pocet zhlukov, 2-Rozny pocet zhlukov): ";
		cin >> modVypoctu;
	} while (mod != 1 && mod != 2);
	if (mod == 1) {
		do {
			cout << "Pocet zhlukov: ";
			cin >> pocetZhlukov;
		} while (pocetZhlukov < 0);
		// counter.setNumberOfClusters(pocetZhlukov); robit v cykle len jendom
	}
	else {
		do {
			do {
				cout << "Pocet zhlukov od: ";
				cin >> pocetZhlukovOd;
			} while (pocetZhlukovOd < 0);
			do {
				cout << "Pocet zhlukov do: ";
				cin >> pocetZhlukovOd;
			} while (pocetZhlukovDo < 0);
		} while (pocetZhlukovOd > pocetZhlukovDo);
	}

	cout << "Validacne indexy (1-pouzit, 0-nepouzit)" << endl;
	counter.unsetAllCVI();
	do {
		cout << "Vsetky: ";
		cin >> vsetkyIndexy;
	} while (vsetkyIndexy != 1 && vsetkyIndexy != 0);
	if (vsetkyIndexy == 1) {
		counter.setAllCVI();
	}
	else {
		for (int i = 0; i < pocetCVI; i++) {
			do {
				cout << menaCVI[i] << ": ";
				cin >> CVIarray[i];
			} while (CVIarray[i] != 1 && CVIarray[i] != 0);
			if (CVIarray[i] == 1) {
				counter.setCVI(static_cast<IndexEnum>(i));
			}
		}
	}

	do {
		cout << "Interval spolahlivosti (90,95,99)%: ";
		cin >> intervalSpolahlivosti;
	} while (intervalSpolahlivosti != 90 && intervalSpolahlivosti != 95 && intervalSpolahlivosti != 99);
	counter.setConfidenceInterval(intervalSpolahlivosti);

	do {
		cout << "Zaznamenat vysledky jednotlivych vypoctov do suboru (1-ano, 0-nie): ";
		cin >> vsetkyVysledkyDoSuboru;
	} while (vsetkyVysledkyDoSuboru!= 1 && vsetkyVysledkyDoSuboru != 0);
	if (vsetkyVysledkyDoSuboru == 1) {
		do {
			cout << "Pripona vysledneho suvoru/suborov (1-arff, 2-txt): ";
			cin >> pripona;
		} while (pripona != 1 && pripona != 2);
		counter.setFileOutputMode();
		pripona = pripona == 2 ? 0 : 1;
		counter.setTypeOfOutput((OutputType)pripona);
	}
	else {
		counter.unsetFileOutputMode();
	}

	do {
		cout << "Zaznamenat vysledky validacnych indexov do suboru (1-ano, 0-nie): ";
		cin >> vysledkyCVIfile;
	} while (vysledkyCVIfile != 1 && vysledkyCVIfile != 0);
	if (vysledkyCVIfile == 1) {
		counter.setCVIFileOutputMode();
	}
	else {
		counter.unsetCVIFileOutputMode();
	}

	do {	//dat skor hore
		cout << "Inicializacny mod (1-nahodne, 2-FCM++): ";
		cin >> initMode;
	} while (initMode!= 1 && initMode != 2);
	counter.setMuInitializationMode((MuInitializationMode)++initMode);

	//ak skoncit po prejdeni sa opztat
	

	
	

	counter.setAllCVI();
	counter.setFileOutputMode();	
	counter.setCVIFileOutputMode();
	
		 
	//counter.setMuInitializationMode(MuInitializationMode::fcmPlusPlus);

	counter.setTitle("Wine");
	fileName = "wine.data";
	counter.setData(fileName.c_str());
	counter.setNumberOfClusters(3);
	counter.setNumberOfReplications(100);

	counter.count(Algorithm::fcm,100);
	counter.count(Algorithm::pso,100);
	counter.count(Algorithm::fcmpso,3);
	counter.count(Algorithm::fa,20);
	counter.count(Algorithm::fafcm,1);

	/*fileName = "iris.data";
	counter.setTitle("Iris");
	counter.setNumberOfClusters(3);
	counter.setData(fileName);
	counter.count(Algorithm::fcm, 100);
	/*counter.count(Algorithm::pso, 10000);
	counter.count(Algorithm::fcmpso, 50);
	counter.count(Algorithm::fa, 500);
	counter.count(Algorithm::fafcm, 1);

	/*for (int i = 1; i < 10; i++)
	{
		fileName = "iris.data";
		counter.setTitle("Iris");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "parkinsons.data";
		counter.setTitle("Parkinson");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "glass_csv.csv";
		counter.setTitle("Glass");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "wine.data";
		counter.setTitle("Wine");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

	}

	counter.setMuInitializationMode(MuInitializationMode::fcmPlusPlus);
	for (int i = 1; i < 10; i++)
	{
		fileName = "iris.data";
		counter.setTitle("Iris");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "parkinsons.data";
		counter.setTitle("Parkinson");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "glass_csv.csv";
		counter.setTitle("Glass");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

		fileName = "wine.data";
		counter.setTitle("Wine");
		counter.setNumberOfClusters(i);
		counter.setData(fileName);
		counter.count(Algorithm::fcm, 1000);
		counter.count(Algorithm::pso, 10000);
		counter.count(Algorithm::fcmpso, 50);
		counter.count(Algorithm::fa, 500);
		counter.count(Algorithm::fafcm, 1);

	}*/

 	return 0;
}
