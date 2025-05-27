#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct {
	int id;
	char* nume;
	int varsta;
	int gradUrgenta;
}Pacient;

typedef struct {
	struct Nod* st;
	struct Nod* dr;
	Pacient info;
}Nod;

Pacient citirePacientDinFisier(FILE* f) {
	Pacient p;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, f);
	p.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	p.nume = (char*)malloc(strlen(aux) + 1);
	strcpy(p.nume, aux);
	p.varsta = atoi(strtok(NULL, sep));
	p.gradUrgenta = atoi(strtok(NULL, sep));
	return p;
}

void afisarePacient(Pacient p) {
	printf("Id: %d\n", p.id);
	printf("Nume: %s\n", p.nume);
	printf("Varta: %d\n", p.varsta);
	printf("Grad urgenta: %d\n", p.gradUrgenta);
}

void adaugaPacientInArbore(Nod** radacina, Pacient pNou) {
	if ((*radacina) != NULL) {
		if (pNou.gradUrgenta < (*radacina)->info.gradUrgenta) {
			adaugaPacientInArbore(&((*radacina)->st), pNou);
		}
		else if (pNou.gradUrgenta > (*radacina)->info.gradUrgenta) {
			adaugaPacientInArbore(&((*radacina)->dr), pNou);
		}

		pNou.nume = (char*)malloc(strlen((*radacina)->info.nume) + 1);
		strcpy(pNou.nume, (*radacina)->info.nume);
	}
	else {
		(*radacina) = malloc(sizeof(Nod));
		(*radacina)->info = pNou;
		(*radacina)->info.nume = (char*)malloc(strlen(pNou.nume) + 1);
		strcpy((*radacina)->info.nume, pNou.nume);
		(*radacina)->st = NULL;
		(*radacina)->dr = NULL;
	}
}

Nod* citireArboreDinFisier(char* numeFisier) {
	Nod* radacina = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Pacient p = citirePacientDinFisier(f);
		adaugaPacientInArbore(&radacina, p);
		free(p.nume);
	}
	fclose(f);
	return radacina;
}

void afisareInOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisareInOrdine(radacina->st);
		afisarePacient(radacina->info);
		afisareInOrdine(radacina->dr);
	}
}

void afisareInPreOrdine(Nod* radacina) {
	if (radacina != NULL) {
		afisarePacient(radacina->info);
		afisareInPreOrdine(radacina->st);
		afisareInPreOrdine(radacina->dr);
	}
}

void dezalocareArbore(Nod** radacina) {
	if ((*radacina) != NULL) {
		dezalocareArbore(&((*radacina)->st));
		dezalocareArbore(&((*radacina)->dr));
		free((*radacina)->info.nume);
		free(*radacina);
		*radacina = NULL;

	}
}

int detNrDeNoduri(Nod* radacina) {
	if (radacina != NULL) {
		return 1 + detNrDeNoduri(radacina->st) + detNrDeNoduri(radacina->dr);
	}
}

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int intaltimeSt = calculeazaInaltimeArbore(radacina->st);
		int intaltimeDr = calculeazaInaltimeArbore(radacina->dr);
		return 1 + (intaltimeSt > intaltimeDr ? intaltimeSt : intaltimeDr);
	}
	else {
		return 0;
	}
}

Pacient getPacientById(Nod* radacina, int id) {
	Pacient p;
	if (id < radacina->info.id) {
		return getPacientById(radacina->st, id);
	}
	else if (id > radacina->info.id) {
		return getPacientById(radacina->dr, id);
	}
	else {
		p = radacina->info;
		p.nume = (char*)malloc(strlen(radacina->info.nume) + 1);
		strcpy(p.nume, radacina->info.nume);
		return p;
	}
	return p;
}

int main() {

	Nod* arbore = citireArboreDinFisier("pacienti.txt");
	afisareInOrdine(arbore);
	printf("\n");
	afisareInPreOrdine(arbore);
	printf("\n");
	int inaltime = calculeazaInaltimeArbore(arbore);
	printf("Inaltime arbore: %d\n", inaltime);

	int nrNoduri = detNrDeNoduri(arbore);
	printf("Nr noduri: %d\n", nrNoduri);

	Pacient p2 = getPacientById(arbore, 2);
	afisarePacient(p2);

	dezalocareArbore(&arbore);


}