#include <vld.h>
#include <vector>
#include "Application.h"
#include <iostream>
#include <string>

using namespace std;

int main() {
	int koniec = 0;
	bool prvy = true;
	bool ponechatNastavenia = false;
	bool ponechatDataset = false;

	Application counter;
	cout << "METAFUZZY" << endl;
	cout << "Nastavenie parametrov" << endl;
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
		int vysledkyCVIconsole = 0;
		int initMode = 0;
		int m, K, w, Ppso, Pfa = 0;
		double c1, c2, r1, r2, maxV, alpha, beta, gamma = 0;
		int defPSO, defFA = 0;
		int intervalSpolahlivosti = 0;
		int nastavitNazov, konecneKriteriumReplikacie = 0, nastavitDataset = 0, nastavitNastavenia = 0, vysledkyCVIeach = 0;
		
		if (!prvy)
		{
			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Nastavit novy dataset (1-ano, 0-pouzit uz nastaveny) : ";
				cin >> nastavitDataset;
			} while (nastavitDataset != 1 && nastavitDataset != 0);
			if (nastavitDataset == 1) {
				ponechatDataset = false;
			}
			else {
				ponechatDataset = true;
			}
			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Ponechat vsetky nastavenia predchadzajuceho vypoctu (1-ano, 0-nove nastavenia) : ";
				cin >> nastavitNastavenia;
			} while (nastavitNastavenia != 1 && nastavitNastavenia != 0);
			if (nastavitNastavenia == 1) {
				ponechatNastavenia = true;
			}
			else {
				ponechatNastavenia= false;
			}
		}

		if (!ponechatDataset) {
			do {
				fileName = "";
				cout << "Nazov suboru s datami: ";
				cin >> fileName;
			} while (!counter.setData(fileName.c_str()));

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Nastavit nazov datasetu (1-ano, 0-pouzit prednastaveny) : ";
				cin >> nastavitNazov;
			} while (nastavitNazov != 1 && nastavitNazov != 0);

			if (nastavitNazov == 1) {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Nazov datasetu: ";
				cin >> title;
				counter.setTitle((char*)title.c_str());
			}
		}
				
		if (!ponechatNastavenia) {
			cout << "Konecne kriterium pre FCM-FPSO je vzdy maximalny pocet iteracii a HFAFCM sa vykona vzdy iba raz." << endl;
			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Konecne kriterium pre FCM,FPSO,FA (1-maximalny pocet iteracii, 2-minimalna zmena cenier, 3-oboje) : ";
				cin >> konecneKriteriumReplikacie;
			} while (konecneKriteriumReplikacie != 1 && konecneKriteriumReplikacie != 2 && konecneKriteriumReplikacie != 3);

			double minChange = 0;
			if (konecneKriteriumReplikacie == 2 || konecneKriteriumReplikacie == 3) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
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
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Mod (1-Jedna replikacia, 2-Viac replikacii): ";
				cin >> mod;
			} while (mod != 1 && mod != 2);
			counter.setNumberOfReplications(1);
			if (mod == 2) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Pocet replikacii: ";
					cin >> pocetReplikacii;
				} while (pocetReplikacii < 0);
				counter.setNumberOfReplications(pocetReplikacii);
			}
			cout << "Algoritmy (1-pouzit, 0-nepouzit)" << endl;
			for (int i = 0; i < pocetAlgoritmov; i++) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << menaAlgoritmov[i] << ": ";
					cin >> algoritmy[i];
				} while (algoritmy[i] != 1 && algoritmy[i] != 0);
				if ((i == 0 || i == 1 || i == 3) && algoritmy[i] == 1) {
					if (konecneKriteriumReplikacie == 1 || konecneKriteriumReplikacie == 3) {
						do {
							cin.clear();
							cin.ignore(10000, '\n');
							cout << menaAlgoritmov[i] << " pocet iteracii: ";
							cin >> pocetIteraciiAlg[i];
						} while (pocetIteraciiAlg[i] < 0);
					}
				}
				if (i == 2 && algoritmy[i] == 1) {
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << menaAlgoritmov[i] << " pocet iteracii: ";
						cin >> pocetIteraciiAlg[i];
					} while (pocetIteraciiAlg[i] < 0);
				}
			}

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Inicializacny mod (1-nahodne, 2-FCM++): ";
				cin >> initMode;
			} while (initMode != 1 && initMode != 2);
			counter.setMuInitializationMode(static_cast<MuInitializationMode>(--initMode));

			do {
				cin.clear();
				cin.ignore(10000, '\n');
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

			if (jeMetaheuristika) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Konstanta K (K >= 1, pre vypocet fitness funkcie, prednastavena K=1): ";
					cin >> K;
				} while (K < 1);
				counter.setK(K);
			}

			if ((algoritmy[pso] || algoritmy[fcmpso])) {
				bool defaultPSO = true;
				cout << "Prednastavene hodnoty konstant PSO: w=1,c1=1,c2=1,r1=0.5,r2=0.5,maxV=0.0001,P=10" << endl;
				do {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Nastavit vlastne hodnoty konstant pre PSO (1-ano, 0-nie): ";
					cin >> defPSO;
				} while (defPSO != 1 && defPSO != 0);
				defaultPSO = defPSO == 1 ? false : true;

				if (!defaultPSO) {
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta w (w >= 0): ";
						cin >> w;
					} while (w < 0);
					counter.setW(w);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta c1 (c1 >= 0): ";
						cin >> c1;
					} while (c1 < 0);
					counter.setC1(c1);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta c2 (c2 >= 0): ";
						cin >> c2;
					} while (c2 < 0);
					counter.setC2(c2);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta r1 <0,1>: ";
						cin >> r1;
					} while (r1 > 1 || r1 < 0);
					counter.setR1(r1);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta r2 <0,1>: ";
						cin >> r2;
					} while (r2 > 1 || r2 < 0);
					counter.setR2(r2);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Maximalna rychlost V (V > 0): ";
						cin >> maxV;
					} while (maxV <= 0);
					counter.setMaxV(maxV);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
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
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Nastavit vlastne hodnoty konstant pre FA (1-ano, 0-nie): ";
					cin >> defFA;
				} while (defFA != 1 && defFA != 0);
				defaultFA = defFA == 1 ? false : true;
				if (!defaultFA) {
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta alpha <0,1>: ";
						cin >> alpha;
					} while (alpha > 1 || alpha < 0);
					counter.setAlpha(alpha);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta beta (beta >= 0): ";
						cin >> beta;
					} while (beta < 0);
					counter.setBeta(beta);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Konstanta gama (gama >= 0): ";
						cin >> gamma;
					} while (gamma < 0);
					counter.setGamma(gamma);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Velkost populacie P (P > 0): ";
						cin >> Pfa;
					} while (Pfa <= 0);
					counter.setSizeOfPopulationFA(Pfa);
				}
			}

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Mod vypoctu (1-Jeden pocet zhlukov, 2-Rozny pocet zhlukov): ";
				cin >> modVypoctu;
			} while (modVypoctu != 1 && modVypoctu != 2);
			if (modVypoctu == 1) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
					cout << "Pocet zhlukov: ";
					cin >> pocetZhlukovOd;
				} while (pocetZhlukovOd < 0);
				pocetZhlukovDo = pocetZhlukovOd;
			}
			else {
				do {
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Pocet zhlukov od: ";
						cin >> pocetZhlukovOd;
					} while (pocetZhlukovOd < 0);
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << "Pocet zhlukov do: ";
						cin >> pocetZhlukovDo;
					} while (pocetZhlukovDo < 0);
				} while (pocetZhlukovOd > pocetZhlukovDo);
			}

			cout << "Validacne indexy (1-pouzit, 0-nepouzit)" << endl;
			counter.unsetAllCVI();
			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Vsetky: ";
				cin >> vsetkyIndexy;
			} while (vsetkyIndexy != 1 && vsetkyIndexy != 0);
			if (vsetkyIndexy == 1) {
				counter.setAllCVI();
			}
			else {
				for (int i = 0; i < pocetCVI; i++) {
					do {
						cin.clear();
						cin.ignore(10000, '\n');
						cout << menaCVI[i] << ": ";
						cin >> CVIarray[i];
					} while (CVIarray[i] != 1 && CVIarray[i] != 0);
					if (CVIarray[i] == 1) {
						counter.setCVI(static_cast<IndexEnum>(i));
					}
				}
			}

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Interval spolahlivosti (90,95,99)%: ";
				cin >> intervalSpolahlivosti;
			} while (intervalSpolahlivosti != 90 && intervalSpolahlivosti != 95 && intervalSpolahlivosti != 99);
			counter.setConfidenceInterval(intervalSpolahlivosti);

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Zaznamenat vysledky jednotlivych vypoctov do suboru (1-ano, 0-nie): ";
				cin >> vsetkyVysledkyDoSuboru;
			} while (vsetkyVysledkyDoSuboru != 1 && vsetkyVysledkyDoSuboru != 0);
			if (vsetkyVysledkyDoSuboru == 1) {
				do {
					cin.clear();
					cin.ignore(10000, '\n');
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
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Vypisat vysledky validacnych indexov na obrazovku (1-ano, 0-nie): ";
				cin >> vysledkyCVIconsole;
			} while (vysledkyCVIconsole != 1 && vysledkyCVIconsole != 0);
			if (vysledkyCVIconsole == 1) {
				counter.setCVIConsoleOutputMode();
			}
			else {
				counter.unsetCVIConsoleOutputMode();
			}

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Zaznamenat vysledky validacnych indexov do suboru (1-ano, 0-nie): ";
				cin >> vysledkyCVIfile;
			} while (vysledkyCVIfile != 1 && vysledkyCVIfile != 0);
			if (vysledkyCVIfile == 1) {
				counter.setCVIFileOutputMode();
			}
			else {
				counter.unsetCVIFileOutputMode();
			}

			do {
				cin.clear();
				cin.ignore(10000, '\n');
				cout << "Zaznamenat vysledky CVI pre kazdu replikaciu (1-ano, 0-nie): ";
				cin >> vysledkyCVIeach;
			} while (vysledkyCVIeach != 1 && vysledkyCVIeach != 0);
			if (vysledkyCVIeach == 1) {
				counter.setEachCVI();
			}
			else {
				counter.unsetEachCVI();
			}
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
			cin.clear();
			cin.ignore(10000, '\n');
			cout << "Pokracovat v dalsom skumani alebo skoncit? (1-pokracovat, 0-skoncit) : ";
			cin >> koniec;
		} while (koniec != 1 && koniec != 0);
		prvy = false;
	} while (koniec == 1);
	cout << "KONIEC" << endl;
	cout << "Dovidenia!" << endl;

 	return 0;
}
