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

	int koniec = 0;
	do {

		string fileName = "", title = "";
		int mod = 0;
		int modVypoctu = 0;
		int pocetReplikacii = 1;
		int algoritmy[5];
		string menaAlgoritmov[] = { "FCM","PSO","FCM-PSO","FA","FAFCM" };
		int pocetIteraciiAlg[] = {1,1,1,1};
		int pocetAlgoritmov = 5;
		int CVIarray[10];
		string menaCVI[] = { "PC","MPC","PE","FS","XIEBENI","KWON","FHV","PBMF","PCAES","SILHOUETTE" };
		int pocetCVI = 10;
		int pocetZhlukovOd = 0;
		int pocetZhlukovDo = 0;
		int vsetkyIndexy = 0;
		int vsetkyVysledkyDoSuboru = 0;
		int pripona = 0;
		int vysledkyCVIfile = 0;
		int initMode = 0;
		int m, K, w, Ppso, Pfa = 0;
		double c1, c2, r1, r2, maxV, alpha, beta, gamma = 0;
		int defPSO, defFA = 0;
		int intervalSpolahlivosti = 0;
		int nastavitNazov, konecneKriteriumReplikacie = 0;		


		Application counter;
		cout << "METAFUZZY" << endl;
		cout << "Nastavenie parametrov" << endl;

		counter.setData("iris.data");
		counter.setNumberOfReplications(1);
		do {
			cout << "Nazov suboru s datami: ";
			cin >> fileName;
		} while (!counter.setData(fileName.c_str()));

		do {
			cout << "Nastavit nazov datasetu (1-ano, 0-pouzit prednastaveny) : ";
			cin >> nastavitNazov;
		} while (nastavitNazov != 1 && nastavitNazov != 0);

		if (nastavitNazov == 1) {
			cout << "Nazov datasetu: ";
			cin >> title;
			counter.setTitle((char*)title.c_str());
		}

		cout << "Konecne kriterium pre FCM-FPSO je vzdy maximalny pocet iteracii a HFAFCM sa vykona vzdy iba raz." << endl;
		do {
			cout << "Konecne kriterium pre FCM,FPSO,FA (1-maximalny pocet iteracii, 2-minimalna zmena cenier, 3-oboje) : ";
			cin >> konecneKriteriumReplikacie;
		} while (konecneKriteriumReplikacie != 1 && konecneKriteriumReplikacie != 2 && konecneKriteriumReplikacie != 3);

		double minChange = 0;
		if (konecneKriteriumReplikacie == 2 || konecneKriteriumReplikacie == 3) {
			do {
				cout << "Minimalna zmena: ";
				cin >> minChange;
			} while (minChange < 0);
			counter.setMinChange(minChange);
		}
		else {
			counter.setFinalCriterion(FinalCriterion::maxIteration);
		}
		if (konecneKriteriumReplikacie == 2) {
			counter.setFinalCriterion(FinalCriterion::minChange);
		}
		else if (konecneKriteriumReplikacie == 3) {
			counter.setFinalCriterion(FinalCriterion::both);
		}
		
		do {
			cout << "Mod (1-Jedna replikacia, 2-Viac replikacii): ";
			cin >> mod;
		} while (mod != 1 && mod != 2);	//pri mod = 1 vypise aj vystup
		counter.setNumberOfReplications(1);
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
				cout << menaAlgoritmov[i] << ": ";
				cin >> algoritmy[i];
			} while (algoritmy[i] != 1 && algoritmy[i] != 0); 
			if ((i == 0 || i==1 || i==3)&& algoritmy[i] == 1) {
				if (konecneKriteriumReplikacie == 1 || konecneKriteriumReplikacie == 3) {
					do {
						cout << menaAlgoritmov[i] << " pocet iteracii: ";
						cin >> pocetIteraciiAlg[i];
					} while (pocetIteraciiAlg[i] < 0);
				}
			}
			if (i == 2 && algoritmy[i] == 1) {
				do {
					cout << menaAlgoritmov[i] << " pocet iteracii: ";
					cin >> pocetIteraciiAlg[i];
				} while (pocetIteraciiAlg[i] < 0);
			}
		}

		do {
			cout << "Inicializacny mod (1-nahodne, 2-FCM++): ";
			cin >> initMode;
		} while (initMode != 1 && initMode != 2);
		counter.setMuInitializationMode(static_cast<MuInitializationMode>(--initMode));
		/*if (initMode == 1) {
			counter.setMuInitializationMode(MuInitializationMode::random);
		}
		else {
			counter.setMuInitializationMode(MuInitializationMode::fcmPlusPlus);
		}*/

		do {
			cout << "Konstanta m (m > 1, prednastevena m=2): ";
			cin >> m;
		} while (m <= 1);
		counter.setM(m);

		bool jeMetaheuristika = false;
		for (int i = 1; i < pocetAlgoritmov; i++) {
			if (algoritmy[i] == 1) {
				jeMetaheuristika = true;
				break;
			}
		}
		//este vypisy checknut a ulozenia
		if (jeMetaheuristika) {
			do {
				cout << "Konstanta K (K >= 1, pre vypocet fitness funkcie, prednastavena K=1): ";
				cin >> K;
			} while (K < 1);
			counter.setK(K);
		}

		if ((algoritmy[pso] || algoritmy[fcmpso])) {
			bool defaultPSO = true;
			cout << "Prednastavene hodnoty konstant PSO: w=1,c1=1,c2=1,r1=0.5,r2=0.5,maxV=0.0001,P=10" << endl;
			do {
				cout << "Nastavit vlastne hodnoty konstant pre PSO (1-ano, 0-nie): ";
				cin >> defPSO;
			} while (defPSO != 1 && defPSO != 0);
			defaultPSO = defPSO == 1 ? false : true;

			if (!defaultPSO) {
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
		}

		if (algoritmy[fa] || algoritmy[fafcm]) {
			bool defaultFA = true;
			cout << "Prednastavene hodnoty konstant FA: alpa=0.5,beta=1,gama=1,P=20" << endl;
			do {
				cout << "Nastavit vlastne hodnoty konstant pre FA (1-ano, 0-nie): ";
				cin >> defFA;
			} while (defFA != 1 && defFA != 0);
			defaultFA = defFA == 1 ? false : true;
			if (!defaultFA) {
				do {
					cout << "Konstanta alpha <0,1>: ";
					cin >> alpha;
				} while (alpha > 1 || alpha < 0);
				counter.setAlpha(alpha);
				do {
					cout << "Konstanta beta (beta >= 0): ";
					cin >> beta;
				} while (beta < 0);
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
		}

		do {
			cout << "Mod vypoctu (1-Jeden pocet zhlukov, 2-Rozny pocet zhlukov): ";
			cin >> modVypoctu;
		} while (modVypoctu != 1 && modVypoctu != 2);
		if (modVypoctu == 1) {
			do {
				cout << "Pocet zhlukov: ";
				cin >> pocetZhlukovOd;
			} while (pocetZhlukovOd < 0);
			pocetZhlukovDo = pocetZhlukovOd;
		}
		else {
			do {
				do {
					cout << "Pocet zhlukov od: ";
					cin >> pocetZhlukovOd;
				} while (pocetZhlukovOd < 0);
				do {
					cout << "Pocet zhlukov do: ";
					cin >> pocetZhlukovDo;
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
		} while (vsetkyVysledkyDoSuboru != 1 && vsetkyVysledkyDoSuboru != 0);
		if (vsetkyVysledkyDoSuboru == 1) {
			do {
				cout << "Pripona vysledneho suvoru/suborov (1-arff, 2-txt): ";
				cin >> pripona;
				//} while (pripona != 1 && pripona != 2);
			} while (false);
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


		for (int i = pocetZhlukovOd; i < pocetZhlukovDo + 1; i++)
		{
			counter.setNumberOfClusters(i);
			if (algoritmy[fcm] == 1) {
				counter.count(Algorithm::fcm, pocetIteraciiAlg[fcm]);
			}
			if (algoritmy[pso] == 1) {
				counter.count(Algorithm::pso, pocetIteraciiAlg[pso]);
			}
			if (algoritmy[fcmpso] == 1) {
				counter.count(Algorithm::fcmpso, pocetIteraciiAlg[fcmpso]);
			}
			if (algoritmy[fa] == 1) {
				counter.count(Algorithm::fa, pocetIteraciiAlg[fa]);
			}
			if (algoritmy[fafcm] == 1) {
				counter.count(Algorithm::fafcm, 1);
			}
		}
		cout << "DOPOCITANE!!!" << endl;
		cout << endl;

		do {
			cout << "Pokracovat v dalsom skumani alebo skoncit? (1-pokracovat, 0-skoncit) : ";
			cin >> koniec;
		} while (koniec != 1 && koniec != 0);
	} while (koniec == 1);
	cout << "KONIEC" << endl;
	cout << "Dovidenia!" << endl;

 	return 0;
}
