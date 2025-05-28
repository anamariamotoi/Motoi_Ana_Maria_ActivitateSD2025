#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//Task-uri suplimentare
//5.Lista simpla

typedef struct {
	int id;
	char* adresa;
	float suprafata;
	float pret;
	int nrCamere;
}Apartament;

typedef struct {
	Apartament info;
	struct Nod* next;
}Nod;

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
	printf("Numar camere: %d\n", a.nrCamere);
}

void afisareListaApartamente(Nod* cap) {
	while (cap!=NULL) {
		afisareApartament(cap->info);
		cap = cap->next;
	}
}

void adaugaApartamentInLista(Nod** cap, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = NULL;
	if (*cap) {
		Nod* temp = *cap;
		while (temp->next) {
			temp = temp->next;
		}
		temp->next = nou;
	}
	else {
		*cap = nou;
	}

}

void adaugaLaInceputInLista(Nod** cap, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = *cap;
	*cap = nou;
}

Nod* citireListaMasiniDinFisier(const char* numeFisier) {
	Nod* cap = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Apartament a = citireApartamentDinFisier(f);
		adaugaApartamentInLista(&cap, a);
	}
	fclose(f);
	return cap;
}

void stergereNodDinLista(Nod** cap, int index) {
	if (*cap) {
		Nod* temp = *cap;
		if (index == 0) {
			*cap = temp->next;
			if (temp->info.adresa) {
				free(temp->info.adresa);
			}
			free(temp);
			return;
		}
		for (int i = 0; i < index-1; i++) {
			temp = temp->next;
		}
		if (temp != NULL) {
			Nod* sters = temp->next;
			temp->next = sters->next;
			if (sters->info.adresa) {
				free(sters->info.adresa);
			}
			free(sters);
		}
	}
}

Nod* sortareCrescatorDupaPret(Nod* cap) {
	Nod* listaSortata = NULL;

	while (cap) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = cap->info;

		if (cap->info.adresa != NULL) {
			nou->info.adresa = (char*)malloc(strlen(cap->info.adresa) + 1);
			strcpy(nou->info.adresa, cap->info.adresa);
		}
		else {
			nou->info.adresa = NULL;
		}

		nou->next = NULL;

		if (listaSortata == NULL || nou->info.pret < listaSortata->info.pret) {
			nou->next = listaSortata;
			listaSortata = nou;
		}
		else {
			Nod* temp = listaSortata;
			Nod* t = temp->next;
			while (temp->next && t->info.pret < nou->info.pret) {
				temp = temp->next;
			}
			nou->next = temp->next;
			temp->next = nou;
		}

		cap = cap->next;
	}

	return listaSortata;
}

//suprafata mai mare decat 60
Apartament* apartCuSuprafataMare(Nod* cap, int* dimVector) {
	int count = 0;
	Nod* temp = cap;
	while (temp) {
		if (temp->info.suprafata > 60) {
			count++;
		}
		temp = temp->next;
	}
	*dimVector = count;

	Apartament* nou = (Apartament*)malloc(sizeof(Apartament) * (*dimVector));
	temp = cap;
	int i = 0;
	while(temp){
			if (temp->info.suprafata > 60) {
				nou[i] = temp->info;
				nou[i].adresa = (char*)malloc(strlen(temp->info.adresa) + 1);
				strcpy(nou[i].adresa, temp->info.adresa);
				i++;
			}
			temp = temp->next;
	}

	return nou;
}

void dezalocareListaApartamente(Nod** cap) {
	while (*cap) {
		Nod* temp = *cap;
		(*cap) = temp->next;
		if (temp->info.adresa) {
			free(temp->info.adresa);
		}
		free(temp);
	}
}
void dezalocareVectorApartamente(Apartament** vector, int* dim) {
	for (int i = 0; i < *dim; i++) {
		free((*vector)[i].adresa);
	}
	free((*vector));
	*dim = 0;
	*vector = NULL;

}


int main() {

	Nod* cap = citireListaMasiniDinFisier("apartamente.txt");
	afisareListaApartamente(cap);

	stergereNodDinLista(&cap, 2);
	afisareListaApartamente(cap);

	printf("\nLista sortata\n");
	Nod* sortat = sortareCrescatorDupaPret(cap);
	afisareListaApartamente(sortat);

	printf("\nApartamentele cu suprafata mai mare decat 60\n");
	int dim = 0;
	Apartament* suprafata = apartCuSuprafataMare(cap, &dim);
	for (int i = 0; i < dim; i++) {
		afisareApartament(suprafata[i]);
	}

	dezalocareVectorApartamente(&suprafata, &dim);
	dezalocareListaApartamente(&sortat);
	dezalocareListaApartamente(&cap);

	return 0;
}
