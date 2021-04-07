// ConsoleApplication1.cpp : Ce fichier contient la fonction 'main'. L'exécution du programme commence et se termine à cet endroit.
//

#include <iostream>
#include <conio.h> // Permet l'utilisation de kbhit() et getch()
#include <ctype.h>
#include <windows.h>
#include <ctime>
#include <time.h>
#include<vector>
#include <stdio.h>
#include<string>
#include <iomanip>

using namespace std;


const int MAXLigne = 17;
const int MAXColonne = 35;
const int PointMax = 50;

const int twistiti = 79;
const int mur = 35;
const int rouleauPoint = 111;
const int rien = 000;
const int Covid1 = 87;

void jouer();
void instruction();
void goToXY(short ligne, short colonne);
void getKeyPress();
void montreMap();
void TrouverChemin(int sx, int sy, int x, int y);
void AjoutMatrice(int x, int y, int mc, int back);

int nbrPoint = 0;
bool twistitiOn = false;
bool stopCounting = false;
//Permet de gérer les intervales de temps
clock_t startTime = 0, endTime = 0;
int vie = 3;
int vitesseCovid = 3;

// Position initiale du joueur
int vieI = 15; int vieJ = 22;
// Position initiale du covid1
int covid1I = 8; int covid1J = 17;

//Map qui sert au mouvement du covid
char c_levelMatrix[MAXLigne][MAXColonne];

// Map pour les mouvements du joueur et des points
char levelMatrix[MAXLigne][MAXColonne] =
{
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},
	{35,79,111,111,111,111,000,000,000,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,000,000,000,111,111,111,111,79,35},
	{35,000,35,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,35,000,35},
	{35,000,35,000,111,111,111,000,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,000,111,111,111,000,35,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,35,000,111,111,111,000,35,35,35,000,35,000,35,35,35,000,35,35,35,000,35,000,35,35,35,000,111,111,111,000,35,000,35},
	{35,111,35,000,35,35,35,000,000,000,000,000,35,000,35,000,000,000,000,000,35,000,35,000,000,000,000,000,35,35,35,000,35,111,35},
	{35,111,35,000,35,35,35,000,35,35,35,000,35,000,35,000,000,000,000,000,35,000,35,000,35,35,35,000,35,35,35,000,35,111,35},
	{35,111,000,000,000,000,000,000,35,35,35,000,35,000,35,000,000,87,000,000,35,000,35,000,35,35,35,000,000,000,000,000,000,111,35},
	{35,111,35,000,35,35,35,000,35,35,35,000,35,000,35,000,000,000,000,000,35,000,35,000,35,35,35,000,35,35,35,000,35,111,35},
	{35,111,35,000,35,35,35,000,000,000,000,000,35,000,35,000,000,79,000,000,35,000,35,000,000,000,000,000,35,35,35,000,35,111,35},
	{35,000,35,000,111,111,111,000,35,35,35,000,35,000,35,35,35,35,35,35,35,000,35,000,35,35,35,000,111,111,111,000,35,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,35,000,111,111,111,000,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,000,111,111,111,000,35,000,35},
	{35,000,35,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,35,000,35},
	{35,79,111,111,111,111,000,000,000,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,000,000,000,111,111,111,111,79,35},
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35}
};

// Map qui sert de structure pour le mouvement du covid
char levelMatrixCopie[MAXLigne][MAXColonne] = 
{
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35},
	{35,000,000,000,000,000,000,000,000,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,000,000,000,000,000,000,000,000,35},
	{35,000,35,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,35,000,35},
	{35,000,35,000,000,000,000,000,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,000,000,000,000,000,35,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,35,000,000,000,000,000,35,35,35,000,35,000,35,35,35,000,35,35,35,000,35,000,35,35,35,000,000,000,000,000,35,000,35},
	{35,000,35,000,35,35,35,000,000,000,000,000,35,000,35,000,000,000,000,000,35,000,35,000,000,000,000,000,35,35,35,000,35,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,35,000,35,000,000,000,000,000,35,000,35,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,000,000,000,000,000,000,35,35,35,000,35,000,35,000,000,87,000,000,35,000,35,000,35,35,35,000,000,000,000,000,000,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,35,000,35,000,000,000,000,000,35,000,35,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,35,000,35,35,35,000,000,000,000,000,35,000,35,000,000,79,000,000,35,000,35,000,000,000,000,000,35,35,35,000,35,000,35},
	{35,000,35,000,000,000,000,000,35,35,35,000,35,000,35,35,35,35,35,35,35,000,35,000,35,35,35,000,000,000,000,000,35,000,35},
	{35,000,35,000,35,35,35,000,35,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,35,000,35,35,35,000,35,000,35},
	{35,000,35,000,000,000,000,000,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,000,000,000,000,000,35,000,35},
	{35,000,35,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,000,35,35,000,35,35,000,35,35,35,35,35,35,000,35},
	{35,000,000,000,000,000,000,000,000,35,35,000,000,000,000,000,000,000,000,000,000,000,000,000,35,35,000,000,000,000,000,000,000,000,35},
	{35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35,35}
};

// Permet de orienter les déplacements du Covid1
class entiter
{
public:
	entiter(int x, int y) {
		this->x = x;
		this->y = y;
	}

	void move_x(int p) {
		if (levelMatrix[y][x + p] == ' ') x += p;
	}

	void move_y(int p) {
		if (levelMatrix[y + p][x] == ' ') y += p;
	}

	void move(int p, int q) {
		x += p;
		y += q;
	}

	int get_x() { return x; }
	int get_y() { return y; }

	void draw(char p) {
		levelMatrix[x][y] = p;
		goToXY(x, y); 
		printf("%c", p);
	}

private:
	int x;
	int y;
};

struct marche {
	short marche_compte;
	short x;
	short y;
	short back;
};

struct cible {
	short x;
	short y;
};

vector<cible> marche_queue;

vector<marche> Array;

// Affiche la map avec les points dessus
void montreMap()
{
	HANDLE  hConsole;
	int k = 14;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Change la couleur des caractères affichés sur la console
	SetConsoleTextAttribute(hConsole, k);

	for (int i = 0; i < MAXLigne; i++)
	{
		for (int j = 0; j < MAXColonne; j++)
		{
			//Mur
			if (levelMatrix[i][j] == mur)
			{
				k = 71;
				SetConsoleTextAttribute(hConsole, k);
			}
			else
				if (levelMatrix[i][j] == rien)
				{
					k = 0;
					SetConsoleTextAttribute(hConsole, k);
					cout << " ";
				}
				else
					if (levelMatrix[i][j] == rouleauPoint)
					{
						k = 15;
						SetConsoleTextAttribute(hConsole, k);
					}
					else
						if (levelMatrix[i][j] == twistiti)
						{
							k = 14;
							SetConsoleTextAttribute(hConsole, k);
						}
							
			cout << levelMatrix[i][j];
			if (j == MAXColonne - 1)
				cout << endl;
			if (j == MAXColonne - 1 && i == MAXLigne - 1)
				stopCounting = true;

		}
	}

	// Couleur
	k = 10;
	SetConsoleTextAttribute(hConsole, k);
}

//Met le curseur sur la console à la position demandée
void goToXY(short x, short y)
{
	COORD position = { x, y };
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hConsole, position);
	
}

//Ajoute une autre matrice avec des contraints de déplacement pour le covid
void AjoutMatrice(int x, int y, int mc, int back)
{
	if (c_levelMatrix[y][x] == rien || c_levelMatrix[y][x] == rouleauPoint)
	{
		c_levelMatrix[y][x] = '#';
		marche count;
		count.x = x;
		count.y = y;
		count.marche_compte = mc;
		count.back = back;
		Array.push_back(count);
	}
}

//Permet au covid de s'orienter sur la map pour trouver le joueur
void TrouverChemin(int sx, int sy, int x, int y)
{
	memcpy(c_levelMatrix, levelMatrixCopie, sizeof(levelMatrixCopie));
	Array.clear();
	marche count;
	count.x = sx;
	count.y = sy;
	count.marche_compte = 0;
	count.back = -1;
	Array.push_back(count);

	for (int i = 0; i < Array.size(); i++)
	{
		if (Array[i].y == y && Array[i].x == x)
		{
			marche_queue.clear();
			cible count2;
			while (Array[i].marche_compte != 0) {
				count2.x = Array[i].x;
				count2.y = Array[i].y;
				marche_queue.push_back(count2);

				i = Array[i].back;
			}

			break;
		}
		AjoutMatrice(Array[i].x + 1, Array[i].y, Array[i].marche_compte + 1, i);
		AjoutMatrice(Array[i].x - 1, Array[i].y, Array[i].marche_compte + 1, i);
		AjoutMatrice(Array[i].x, Array[i].y + 1, Array[i].marche_compte + 1, i);
		AjoutMatrice(Array[i].x, Array[i].y - 1, Array[i].marche_compte + 1, i);
	}

	Array.clear();
}

int main()
{
	char choixM;
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	do
	{
		do {
			SetConsoleTextAttribute(hConsole, 14);
			cout << endl;
			cout << "         #######         " << endl;
			cout << "       ###########       " << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "       ## ##### ##       " << endl;
			cout << "       ###########       " << endl;
			cout << "         #######         " << endl;
			cout << "      #############      " << endl;
			cout << "     ##  #######  ##     " << endl;
			cout << "     #   #######  ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "#";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "         #######   ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "         #######   ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "        ##     ##  ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "        #       #  ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << "       ##       ## ";
			SetConsoleTextAttribute(hConsole, 6);
			cout << "#" << endl;
			SetConsoleTextAttribute(hConsole, 15);
			cout << endl;

			cout << "  (1): Jouer" << endl;
			cout << "  (2): Instruction " << endl;
			cout << "  (3): Personnification " << endl;
			cout << "  (4): Score " << endl;
			cout << "  (5): Quitter " << endl;
			cout << "  Choix : ";

			cin >> choixM;

			if (choixM != '1' && choixM != '2' && choixM != '3' && choixM != '4' && choixM != '5')
			{
				cout << "\nErreur de saisie: veuillez choisir entre les valeurs de 1 a 5" << endl;
				Sleep(1000);
			}
			system("cls");
		} while (choixM != '1' && choixM != '2' && choixM != '3' && choixM != '4' && choixM != '5');

		switch (choixM)
		{
		case '1':
			jouer();
			break;
		case '2':
			instruction();
			break;
		case '3':
			break;
		case '4':
			break;
		case '5':
			cout << "Merci et bonne journee" << endl;
			break;
		}
	} while (choixM != '5');

	

	return 0;
}

void jouer()
{
	HANDLE  hConsole;
	int k;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	bool run = true;
	int x = vieJ;
	int y = vieI;

	int ens_x;
	int ens_y;

	int cx = covid1J;
	int cy = covid1I;

	system("mode 80, 50");

	montreMap();

	goToXY(covid1J, covid1I);
	k = 14;
	SetConsoleTextAttribute(hConsole, k);
	cout << "V";

	int cadre = 0;

	TrouverChemin(cx, cy, x, y);

	do
	{
		cx = covid1J;
		cy = covid1I;

		//Start position
		goToXY(x, y);
		cout << " ";

		ens_x = x;
		ens_y = y;

		getKeyPress();

		x = vieJ;
		y = vieI;

		if (ens_x != x || ens_y != y)
		{
			TrouverChemin(cx, cy, x, y);
		}

		goToXY(x, y);
		k = 14;

		if (twistitiOn == true)
		{
			k = 30;
		}
		SetConsoleTextAttribute(hConsole, k);
		goToXY(x, y);
		cout << "V";




		k = 14;
		SetConsoleTextAttribute(hConsole, k);
		//Permet de gérer la vitesse du covid
		if (cadre % vitesseCovid == 0 && marche_queue.size() != 0)
		{
			cx = marche_queue.back().x;
			cy = marche_queue.back().y;
			marche_queue.pop_back();
		}

		goToXY(covid1J, covid1I);
		if (levelMatrix[covid1I][covid1J] == rouleauPoint)
		{
			k = 15;
			SetConsoleTextAttribute(hConsole, k);
			cout << levelMatrix[covid1I][covid1J];
		}
		else
			if (levelMatrix[covid1I][covid1J] == twistiti)
			{
				k = 14;
				SetConsoleTextAttribute(hConsole, k);
				cout << levelMatrix[covid1I][covid1J];
			}
			else
				cout << ' ';

		if (twistitiOn == true)
		{
			k = 30;
		}
		else
			k = 14;

		SetConsoleTextAttribute(hConsole, k);

		goToXY(cx, cy);
		cout << 'C';

		covid1J = cx;
		covid1I = cy;


		if (vieJ == cx && vieI == cy && twistitiOn)
		{
			covid1I = 8;
			cy = 8;
			covid1J = 17;
			cx = 17;
		}
		// Touche le covid sans Twistiti
		else
			if (vieI == cy && vieJ == cx && !twistitiOn)
			{
				goToXY(covid1J, covid1I);
				cout << " ";
				goToXY(vieJ, vieI);
				cout << " ";
				vie--;
				vieI = 15;
				y = 15;
				vieJ = 22;
				x = 22;
				covid1I = 8;
				cy = 8;
				covid1J = 17;
				cx = 17;
			}

		endTime = clock();

		int intervalTemps = int((endTime - startTime) / CLOCKS_PER_SEC);

		if (intervalTemps > 6)
		{
			twistitiOn = false;
			intervalTemps = 0;
		}

		//Met un délai avant la continuiter de la lecture du code
		Sleep(100);
		cadre++;

		goToXY(0, 18);
		k = 2;
		SetConsoleTextAttribute(hConsole, k);
		cout << "Rouleau obtenu:" << nbrPoint;
		cout << "\t\tVie:" << vie << endl;
		cout << "Rouleau desire:" << PointMax << endl;
		if (twistitiOn == true)
		{
			goToXY(30, 19);
			cout << "IMMUNISER";
		}
		else
		{
			goToXY(30, 19);
			cout << "         ";
		}
		// Commence prise de valeurs



		// Fin prise de valeurs
	} while (vie != 0 && nbrPoint < PointMax);

	nbrPoint = 0;
	vie = 3;

	if (nbrPoint >= PointMax)
	{
		k = 3;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\n\n\tMission successful: number of rolls reached" << endl;
	}
	else
	{
		k = 4;
		SetConsoleTextAttribute(hConsole, k);
		cout << "\n\n\tMission failed: number of rolls not reached" << endl;
	}

	cout << "\tPartie terminee" << endl;
	Sleep(1000);
	system("pause");
	system("cls");
	SetConsoleTextAttribute(hConsole, 15);
}

void instruction()
{
	char choixI;
	char retourner;
	HANDLE  hConsole;
	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	system("cls");

	do 
	{
		do
		{
			cout << "(1): Mouvement" << endl;
			cout << "(2): But" << endl;
			cout << "(3): Info lors de jeux" << endl;
			cout << "(4): Quitter" << endl;
			cout << "Choix : ";
			cin >> choixI;

			if (choixI != '1' && choixI != '2' && choixI != '3' && choixI != '4')
			{
				cout << "\nErreur de saisie" << endl;
				Sleep(1000);
			}
			system("cls");

		} while (choixI != '1' && choixI != '2' && choixI != '3' && choixI != '4');

		switch (choixI)
		{
		case '1':
			cout << "Mouvement:" << endl << endl;
			cout << "\tarrow ^: Amene le personnage a bouger vers le haut" << endl;
			cout << "\tarrow <: Amene le personnage a bouger vers la gauche" << endl;
			cout << "\tarrow >: Amene le personnage a bouger vers la droite" << endl;
			cout << "\tarrow v: Amene le personnage a bouger vers le bas" << endl << endl;

			system("pause");
			break;
		case '2':
			cout << "But:" << endl << endl;
			cout << "Le but du jeux est de ramasser tous les rouleaux presents" << endl;
			cout << "sur la carte sans se faire attraper par la ou les virus." << endl;
			cout << "De plus, le joueur ne dispose que de trois vie" << endl << endl;

			system("pause");
			break;
		case '3':
			cout << "Items\tDescription" << endl << endl;
			SetConsoleTextAttribute(hConsole, 14);
			cout << " O   \t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "La twistitie permet une immunisation, durant un certain temps, contre les virus" << endl << endl;
			cout << " o   \tLes rouleaux permettent une augmentation du nombre de point" << endl << endl;
			SetConsoleTextAttribute(hConsole, 14);
			cout << " V   \t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Le joueur" << endl << endl;
			SetConsoleTextAttribute(hConsole, 14);
			cout << " C   \t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Le virus" << endl << endl;
			cout << " ";
			SetConsoleTextAttribute(hConsole, 30);
			cout << "C\t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Indique que le joueur est immunie contre le virus" << endl << endl;
			cout << " ";
			SetConsoleTextAttribute(hConsole, 30);
			cout << "V\t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Indique que le joueur est immunie contre le virus" << endl << endl;
			cout << " ";
			SetConsoleTextAttribute(hConsole, 71);
			cout << "#\t";
			SetConsoleTextAttribute(hConsole, 15);
			cout << "Le mur" << endl << endl;

			system("pause");
			break;
		case '4':
			break;
		}
		system("cls");

	} while (choixI != '4');
}

//Permet de bouger le joueur avec les curseurs
void getKeyPress()
{
		if (GetAsyncKeyState(VK_UP))
		{
			if (levelMatrix[vieI - 1][vieJ] != mur)
			{
				if (levelMatrix[vieI - 1][vieJ] == twistiti)
				{
					startTime = clock();
					twistitiOn = true;
					levelMatrix[vieI - 1][vieJ] = ' ';
				}
				if (levelMatrix[vieI - 1][vieJ] == rouleauPoint)
				{
					levelMatrix[vieI - 1][vieJ] = ' ';
					nbrPoint++;
				}

				vieI--;
			}
		}
		else
			if (GetAsyncKeyState(VK_LEFT))
			{
				if (levelMatrix[vieI][vieJ - 1] != mur)
				{
					if (levelMatrix[vieI][vieJ - 1] == twistiti)
					{
						startTime = clock();
						twistitiOn = true;
						levelMatrix[vieI][vieJ - 1] = ' ';
					}

					if (levelMatrix[vieI][vieJ - 1] == rouleauPoint)
					{
						levelMatrix[vieI][vieJ - 1] = ' ';
						nbrPoint++;
					}

					vieJ--;
				}
			}
			else
				if (GetAsyncKeyState(VK_RIGHT))
				{
					if (levelMatrix[vieI][vieJ + 1] != mur)
					{
						if (levelMatrix[vieI][vieJ + 1] == twistiti)
						{
							startTime = clock();
							twistitiOn = true;
							levelMatrix[vieI][vieJ + 1] = ' ';
						}

						if (levelMatrix[vieI][vieJ + 1] == rouleauPoint)
						{
							levelMatrix[vieI][vieJ + 1] = ' ';
							nbrPoint++;
						}

						vieJ++;
					}
				}
				else
					if (GetAsyncKeyState(VK_DOWN))
					{
						if (levelMatrix[vieI + 1][vieJ] != mur)
						{
							if (levelMatrix[vieI + 1][vieJ] == twistiti)
							{
								startTime = clock();
								twistitiOn = true;
								levelMatrix[vieI + 1][vieJ] = ' ';
							}

							if (levelMatrix[vieI + 1][vieJ] == rouleauPoint)
							{
								levelMatrix[vieI + 1][vieJ] = ' ';
								nbrPoint++;
							}

							vieI++;
						}
					}
}
