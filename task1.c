#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

//task-uri suplimentare 
//1.Structura si functii
//2.Vectori
//3.Fisiere
//4.Matrice

struct Apartament {
	int id;
	char* adresa;
	float suprafata;
	float pret;
	int nrCamere;

};

struct Apartament initializare(int id, const char* adresa, float suprafata, float pret, int nrCamere) {
	struct Apartament a;
	a.id = id;
	a.suprafata = suprafata;
	a.pret = pret;
	a.nrCamere = nrCamere;

	a.adresa = (char*)malloc((strlen(adresa) + 1) * sizeof(char));
	strcpy(a.adresa, adresa);
	return a;

}

struct Apartament citireApartament() {
	struct Apartament a;

	printf("Id: ");
	scanf("%d", &a.id);

	a.adresa = (char*)malloc(100 * sizeof(char));
	getchar();
	printf("Adresa: ");
	fgets(a.adresa, 100, stdin);


	printf("Suprafata: ");
	scanf("%f", &a.suprafata);

	printf("Pret: ");
	scanf("%f", &a.pret);

	printf("Numar camere: ");
	scanf("%d", &a.nrCamere);

	return a;
}

//pretul mediu al apartamentelor inregistrate
float pretMediuApartamente(struct Apartament* apartamente, int nrApartamente) {
	float suma = 0;
	float medie = 0;
	for (int i = 0; i < nrApartamente; i++) {
		suma += apartamente[i].pret;
	}

	medie = suma / nrApartamente;
	return medie;
}

struct Apartament modificarePret(struct Apartament* a, float pretNou) {
	if (pretNou > 0) {
		a->pret = pretNou;
	}

	return *a;
}

//copiere intr-un vector obiectele cu suprafata mai mica de 60 si pretul mai mare de 100000
struct Apartament* afisareApartPretSuprafata(struct Apartament* apartamente, int nrApartamente, int* nrRezultate) {
	int count = 0;
	for (int i = 0; i < nrApartamente; i++) {
		if (apartamente[i].pret > 100000 && apartamente[i].suprafata < 60) {
			count++;
		}
	}
	*nrRezultate = count;
	if (count > 0) {
		struct Apartament* apartNou = (struct Apartament*)malloc(sizeof(struct Apartament) * count);
		int index = 0;
		for (int i = 0; i < nrApartamente; i++) {
			if (apartamente[i].pret > 100000 && apartamente[i].suprafata < 60) {
				apartNou[index] = apartamente[i];
				apartNou[index].adresa = (char*)malloc((strlen(apartamente[i].adresa) + 1) * sizeof(char));
				strcpy(apartNou[index].adresa, apartamente[i].adresa);
				index++;
			}
		}
		return apartNou;
	}
	else {
		return NULL;
	}

}

//copierea intr-un vector a obiectelor cu numarul de camere mai mare sau egal cu 3
struct Apartament* apartNrCamere(struct Apartament* apartament, int nrApartamente, int* nrRezultate) {
	int count = 0;
	for (int i = 0; i < nrApartamente; i++) {
		if (apartament[i].nrCamere >= 3) {
			count++;
		}
	}
	*nrRezultate = count;
	if (count > 0) {
		int index = 0;
		struct Apartament* aNou = (struct Apartament*)malloc(sizeof(struct Apartament) * count);
		for (int i = 0; i < nrApartamente; i++) {
			if (apartament[i].nrCamere >= 3) {
				aNou[index] = apartament[i];
				aNou[index].adresa = (char*)malloc((strlen(apartament[i].adresa) + 1) * sizeof(char));
				strcpy(aNou[index].adresa, apartament[i].adresa);
				index++;
			}
		}
		return aNou;
	}
	else {
		return NULL;
	}
}

//concatenare a doi vectori
struct Apartament* concatenareDoiVectori(struct Apartament* v1, struct Apartament* v2, int nrApartV1, int nrApartV2) {
	//verificare duplicate
	int count = nrApartV1 + nrApartV2;
	int index = 0;
	struct Apartament* concatenat = (struct Apartament*)malloc(sizeof(struct Apartament)*count);
	for (int i = 0; i < nrApartV1; i++) {
		concatenat[index] = v1[i];
		concatenat[index].adresa = (char*)malloc((strlen(v1[i].adresa) + 1) * sizeof(char));
		strcpy(concatenat[index].adresa, v1[i].adresa);
		index++;
	}

	for (int i = 0; i < nrApartV2; i++) {
		concatenat[index] = v2[i];
		concatenat[index].adresa = (char*)malloc((strlen(v2[i].adresa) + 1) * sizeof(char));
		strcpy(concatenat[index].adresa, v2[i].adresa);
		index++;
	}
	return concatenat;
}



void dezalocareVectorApartamente(struct Apartament** a, int* nrApartamente) {
	for (int i = 0; i < *nrApartamente; i++) {
		free((*a)[i].adresa);
	}
	free(*a);
	*a = NULL;
	*nrApartamente = 0;

}

struct Apartament afisareApartament(struct Apartament a) {
	printf("Id: %d\n", a.id);
	printf("Adresa: %s\n", a.adresa);
	printf("Suprafata: %.2f\n", a.suprafata);
	printf("Pret: %.2f\n", a.pret);
	printf("Numar camere: %d\n", a.nrCamere);

}

void afisareVectorApartamente(struct Apartament* a, int dim) {
	for (int i = 0; i < dim; i++) {
		afisareApartament(a[i]);
		printf("\n");
	}

}

struct Apartament citireApartamenteDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	struct Apartament a1;
	
	a1.id = atoi(strtok(buffer, sep));

	char* aux = strtok(NULL, sep);
	a1.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy_s(a1.adresa, strlen(aux) + 1, aux);

	a1.suprafata = atof(strtok(NULL, sep));
	a1.pret = atof(strtok(NULL, sep));
	a1.nrCamere = atoi(strtok(NULL, sep));

	return a1;
}

void inserareInVector(struct Apartament** vector, int* dim, struct Apartament a) {
	struct Apartament* temp = (struct Apartament*)malloc(sizeof(struct Apartament) * (*dim+1));
	for (int i = 0; i < *dim; i++) {
		temp[i] = (*vector)[i];
	}
	temp[*dim].id = a.id;
	temp[*dim].adresa = (char*)malloc(strlen(a.adresa)+1);
	strcpy(temp[*dim].adresa, a.adresa);
	temp[*dim].suprafata = a.suprafata;
	temp[*dim].pret = a.pret;
	temp[*dim].nrCamere = a.nrCamere;
	free(*vector);
	*vector = temp;
	(*dim)++;
}

struct Apartament* citireVectorDinFisier(const char* numeFisier, int* dim) {
	FILE* f = fopen(numeFisier, "r");
	struct Apartament* apartFisier = NULL;

	while (!feof(f)) {
		struct Apartament a = citireApartamenteDinFisier(f);
		inserareInVector(&apartFisier, dim, a);
	}
	fclose(f);
	return apartFisier;
}

void scriereApartamentInFisier(struct Apartament a, char* numeFisier) {
	FILE* f = fopen(numeFisier, "w");
	char buffer[100];
	sprintf(buffer, "%d,%s,%.2f,%.2f,%d", a.id, a.adresa, a.suprafata, a.pret, a.nrCamere);
	fputs(buffer, f);
	fclose(f);

}

void scriereVectorInFisier(struct Apartament* vector, int dimVector, char* numeFisier) {
	FILE* f = fopen("vector.txt", "w");
	char buffer[100];
	for (int i = 0; i < dimVector; i++) {
		sprintf(buffer, "%d,%s,%.2f,%.2f,%d\n", vector[i].id, vector[i].adresa, vector[i].suprafata, vector[i].pret, vector[i].nrCamere);
		fprintf(f, buffer);
	}
	fclose(f);
}

struct Apartament** copiereVectorInMatriceDupaNrCamere(struct Apartament* vector, int dimVector, int* coloane) {
	int linii = 3;
	struct Apartament** matrice = (struct Apartament**)malloc(sizeof(struct Apartament*) * linii);
	for (int i = 0; i < linii; i++) {
		matrice[i] = (struct Apartament*)malloc(sizeof(struct Apartament) * (*coloane));
		for (int j = 0; j < *coloane; j++) {
			if (i == 0) {
				for (int v = 0; v < dimVector; v++) {
					if (vector[v].nrCamere == 1) {
						matrice[i][j].id = vector[v].id;
						matrice[i][j].adresa = (char*)malloc(strlen(vector[v].adresa) + 1);
						strcpy(matrice[i][j].adresa, vector[v].adresa);
						matrice[i][j].suprafata = vector[v].suprafata;
						matrice[i][j].pret = vector[v].pret;
						matrice[i][j].nrCamere = vector[v].nrCamere;
					}
				}
			}
			else if (i == 1) {
				for (int v = 0; v < dimVector; v++) {
					if (vector[v].nrCamere == 2) {
						matrice[i][j].id = vector[v].id;
						matrice[i][j].adresa = (char*)malloc(strlen(vector[v].adresa) + 1);
						strcpy(matrice[i][j].adresa, vector[v].adresa);
						matrice[i][j].suprafata = vector[v].suprafata;
						matrice[i][j].pret = vector[v].pret;
						matrice[i][j].nrCamere = vector[v].nrCamere;
					}
				}
			}
			else if (i == 2) {
				for (int v = 0; v < dimVector; v++) {
					if (vector[v].nrCamere == 3) {
						matrice[i][j].id = vector[v].id;
						matrice[i][j].adresa = (char*)malloc(strlen(vector[v].adresa) + 1);
						strcpy(matrice[i][j].adresa, vector[v].adresa);
						matrice[i][j].suprafata = vector[v].suprafata;
						matrice[i][j].pret = vector[v].pret;
						matrice[i][j].nrCamere = vector[v].nrCamere;
					}
				}
			}
			
		}
	}
	return matrice;
}

void afisareMatrice(struct Apartament** matrice, int linii, int coloane) {
	for (int i = 0; i < linii; i++) {
		for (int j = 0; j < coloane; j++) {
			printf("%d, %s, %.2f, %.2f, %d",
				matrice[i][j].id,
				matrice[i][j].adresa,
				matrice[i][j].suprafata,
				matrice[i][j].pret,
				matrice[i][j].nrCamere);
		}
		printf("\n");
	}
}

int main() {

	//struct Apartament apart1 = citireApartament();

	//afisareApartament(apart1);
	float pretNou = 140000;
	//modificarePret(&apart1, pretNou);
	//printf( "%.2f" , apart1.pret);
	int nrApartamente = 5;
	struct Apartament* apartamente = (struct Apartament*)malloc(sizeof(struct Apartament) * nrApartamente);
	apartamente[0] = initializare(2, "adresa2", 20, 50000, 2);
	apartamente[1] = initializare(3, "adresa3", 30, 100000, 3);
	apartamente[2] = initializare(4, "adresa4", 40, 120000, 3);
	apartamente[3] = initializare(5, "adresa5", 50, 200000, 4);
	apartamente[4] = initializare(6, "adresa6", 60, 350000, 3);

	int nrRezultate = 0;
	struct Apartament* aNou = afisareApartPretSuprafata(apartamente, nrApartamente, &nrRezultate);

	printf("Apartamentele cu pretul mai mare decat 100000 si suprafata mai mica decat 60\n");
	for (int i = 0; i < nrRezultate; i++) {
		afisareApartament(aNou[i]);
	}

	int nrRezCamere = 0;
	struct Apartament* aNouCamere = apartNrCamere(apartamente, nrApartamente, &nrRezCamere);
	printf("\nApartamentele cu numarul de camere mai mic sau egal cu 3\n");
	for (int i = 0; i < nrRezCamere; i++) {
		afisareApartament(aNouCamere[i]);
	}

	int nrApartamente2 = 2;
	struct Apartament* apartamente2 = (struct Apartament*)malloc(sizeof(struct Apartament) * nrApartamente2);
	apartamente2[0] = initializare(7, "adresa7", 100, 400000, 4);
	apartamente2[1] = initializare(8, "adresa8", 60, 350000, 3);

	struct Apartament* concatenare = concatenareDoiVectori(apartamente, apartamente2, nrApartamente, nrApartamente2);
	int nrConcatenare = nrApartamente + nrApartamente2;
	for (int i = 0; i < nrConcatenare; i++) {
		afisareApartament(concatenare[i]);
	}

	FILE* f = fopen("apart.txt", "r");
	if (f != NULL) {
		struct Apartament a = citireApartamenteDinFisier(f);
		afisareApartament(a);
		free(a.adresa);
	}
	fclose(f);
	
	int dimFisier = 0;
	//struct Apartament* apartFisier = citireVectorDinFisier("fisier1.txt", &dimFisier);
	FILE* file = fopen("fisier1.txt", "r");
	struct Apartament* apartFisier = NULL;
	while (!feof(f)) {
		struct Apartament a = citireApartamenteDinFisier(file);
		inserareInVector(&apartFisier, &dimFisier, a);
	}
	fclose(f);
	afisareVectorApartamente(apartFisier, dimFisier);
	dezalocareVectorApartamente(&apartFisier, &dimFisier);

	
	struct Apartament scrieFisier = initializare(1, "adresa", 12, 12332, 2);
	scriereApartamentInFisier(scrieFisier, "scriere.txt");
	free(scrieFisier.adresa);
	
	scriereVectorInFisier(apartamente, nrApartamente, "vector.txt");

	int nrApartamenteMatrice = 10;
	struct Apartament* vectorMatrice = (struct Apartament*)malloc(sizeof(struct Apartament) * nrApartamenteMatrice);

	vectorMatrice[0] = initializare(1, "Strada Florilor 12", 40, 75000, 1);
	vectorMatrice[1] = initializare(2, "Strada Trandafirilor 15", 55, 90000, 2);
	vectorMatrice[2] = initializare(3, "Strada Lalelelor 9", 30, 50000, 1);
	vectorMatrice[3] = initializare(4, "Strada Bujorilor 22", 60, 110000, 3);
	vectorMatrice[4] = initializare(5, "Strada Garoafelor 10", 35, 60000, 2);
	vectorMatrice[5] = initializare(6, "Strada Magnoliei 5", 45, 85000, 1);
	vectorMatrice[6] = initializare(7, "Strada Crinilor 18", 50, 95000, 2);
	vectorMatrice[7] = initializare(8, "Strada Zambilelor 25", 75, 130000, 3);
	vectorMatrice[8] = initializare(9, "Strada Irisului 13", 40, 70000, 2);
	vectorMatrice[9] = initializare(10, "Strada Orhideelor 30", 55, 90000, 3);

	int coloane = 0;
	int linii = 3;
	struct Apartament** matrice = copiereVectorInMatriceDupaNrCamere(vectorMatrice, nrApartamenteMatrice, &coloane);
	afisareMatrice(matrice, linii, coloane);

	dezalocareVectorApartamente(&aNouCamere, &nrRezCamere);
	dezalocareVectorApartamente(&aNou, &nrRezultate);
	dezalocareVectorApartamente(&apartamente, &nrApartamente);
	dezalocareVectorApartamente(&apartamente2, &nrApartamente2);
	

	return 0;
}