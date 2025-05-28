#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int id;
	char* adresa;
	float suprafata;
	float pret;
	int nrCamere;
} Apartament;

typedef struct Nod Nod;
struct Nod {
	Nod* st;
	Nod* dr;
	Apartament info;
};

Apartament citireApartamentDinFisier(FILE* f) {
	Apartament a;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;

	fgets(buffer, 100, f);
	a.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	a.adresa = (char*)malloc(strlen(aux) + 1);
	strcpy(a.adresa, aux);
	a.suprafata = atof(strtok(NULL, sep));
	a.pret = atof(strtok(NULL, sep));
	a.nrCamere = atoi(strtok(NULL, sep));

	return a;
}

void afisareApartament(Apartament a) {
	printf("Id: %d\n", a.id);
	printf("Adresa: %s\n", a.adresa);
	printf("Suprafata: %.2f\n", a.suprafata);
	printf("Pret: %.2f\n", a.pret);
	printf("Numar camere: %d\n\n", a.nrCamere);
}

void adaugaApartamentInArbore(Nod** radacina, Apartament aNou) {
	if ((*radacina) != NULL) {
		if (aNou.id < (*radacina)->info.id) {
			adaugaApartamentInArbore(&((*radacina)->st), aNou);
		}
		else if (aNou.id > (*radacina)->info.id) {
			adaugaApartamentInArbore(&((*radacina)->dr), aNou);
		}
		else {
			if ((*radacina)->info.adresa) {
				free((*radacina)->info.adresa);
			}
			(*radacina)->info.suprafata = aNou.suprafata;
			(*radacina)->info.pret = aNou.pret;
			(*radacina)->info.nrCamere = aNou.nrCamere;
			(*radacina)->info.adresa = (char*)malloc(strlen(aNou.adresa) + 1);
			strcpy((*radacina)->info.adresa, aNou.adresa);
		}
	}
	else {
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = aNou;
		(*radacina)->info.adresa = (char*)malloc(strlen(aNou.adresa) + 1);
		strcpy((*radacina)->info.adresa, aNou.adresa);
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file)) {
		Apartament a = citireApartamentDinFisier(file);
		adaugaApartamentInArbore(&radacina, a);
		free(a.adresa);
	}
	fclose(file);
	return radacina;
}

void afisareInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdine(radacina->st);
		afisareApartament(radacina->info);
		afisareInOrdine(radacina->dr);
	}
}

void afisarePreOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareApartament(radacina->info);
		afisarePreOrdine(radacina->st);
		afisarePreOrdine(radacina->dr);
	}
}

void dezalocareArbore(Nod** radacina) {
	if (*radacina != NULL) {
		dezalocareArbore(&((*radacina)->st));
		dezalocareArbore(&((*radacina)->dr));
		free((*radacina)->info.adresa);
		free(*radacina);
		*radacina = NULL;
	}
}

Apartament getApartamentByID(Nod* radacina, int id) {
	Apartament a;
	a.id = -1;

	if (radacina == NULL) return a;

	if (id > radacina->info.id) {
		return getApartamentByID(radacina->dr, id);
	}
	else if (id < radacina->info.id) {
		return getApartamentByID(radacina->st, id);
	}
	else {
		a = radacina->info;
		a.adresa = (char*)malloc(strlen(radacina->info.adresa) + 1);
		strcpy(a.adresa, radacina->info.adresa);
		return a;
	}
}

int numarNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return 1 + numarNoduri(radacina->st) + numarNoduri(radacina->dr);
	}
	return 0;
}

int inaltimeArbore(Nod* radacina) {
	if (radacina == NULL) return 0;
	int inaltimeSt = inaltimeArbore(radacina->st);
	int inaltimeDr = inaltimeArbore(radacina->dr);
	return 1 + (inaltimeSt > inaltimeDr ? inaltimeSt : inaltimeDr);
}

Nod* subarboreCuInaltimeaMaiMare(Nod* radacina) {
	if (radacina == NULL) return NULL;
	int hSt = inaltimeArbore(radacina->st);
	int hDr = inaltimeArbore(radacina->dr);

	if (hSt > hDr) return radacina->st;
	else return radacina->dr;
}

int numarNoduriSubordonate(Nod* radacina) {
	if (radacina == NULL) return 0;
	return numarNoduri(radacina) - 1;  
}

Nod* subarboreCuMaiMulteNoduri(Nod* radacina) {
	if (radacina == NULL) return NULL;
	int nrSt = numarNoduri(radacina->st);
	int nrDr = numarNoduri(radacina->dr);

	if (nrSt > nrDr) return radacina->st;
	else return radacina->dr;
}



int main() {
	Nod* arbore = citireArboreDinFisier("apartamente.txt");

	afisareInOrdine(arbore);

	Apartament a = getApartamentByID(arbore, 2);
	if (a.id != -1) afisareApartament(a);

	int nr = numarNoduri(arbore);
	printf("\nNumar noduri: %d\n", nr);

	printf("Inaltime arbore: %d\n", inaltimeArbore(arbore));

	Nod* maiInalt = subarboreCuInaltimeaMaiMare(arbore);

	Nod* maiMulti = subarboreCuMaiMulteNoduri(arbore);
	

	dezalocareArbore(&arbore);
	return 0;
}
