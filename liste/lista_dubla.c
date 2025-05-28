#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

//Task-uri suplimentare
//6.Lista dubla

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
	struct Nod* prev;
}Nod;

typedef struct {
	Nod* inceput;
	Nod* final;
}ListaDubla;


Apartament initalizareApartament(int id, char* adresa, float suprafata, float pret, int nrCamere) {
	Apartament a;
	a.id = id;
	a.adresa = (char*)malloc(strlen(adresa) + 1);
	strcpy(a.adresa, adresa);
	a.suprafata = suprafata;
	a.pret = pret;
	a.nrCamere = nrCamere;

	return a;

}
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

void afisareListaApartamenteDeLAInceput(ListaDubla* lista) {
	Nod* temp = lista->inceput;
	while (temp) {
		afisareApartament(temp->info);
		temp = temp->next;
	}
}

void afisareListaApartamenteDeLaFinal(ListaDubla* lista) {
	Nod* temp = lista->final;
	while (temp) {
		afisareApartament(temp->info);
		temp = temp->prev;
	}
}

void adaugaApartamentInLista(ListaDubla* lista, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = NULL;
	nou->prev = lista->final;
	if (lista->final!=NULL) {
		lista->final->next = nou;
	}
	else {
		lista->inceput = nou;
	}
	lista->final = nou;
}

void adaugaApartamentLaInceputInLista(ListaDubla* lista, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = lista->inceput;
	nou->prev = NULL;
	if (lista->inceput != NULL) {
		lista->inceput->prev = nou;
	}
	else {
		lista->final = nou;
	}
	lista->inceput = nou;
}


ListaDubla citireListaMasiniDinFisier(const char* numeFisier) {
	ListaDubla lista;
	lista.inceput = NULL;
	lista.final = NULL;
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Apartament a = citireApartamentDinFisier(f);
		adaugaApartamentInLista(&lista, a);
	}
	fclose(f);
	return lista;
}

void stergereNodDinLista(ListaDubla* lista, int index) {
	if (lista == NULL || lista->inceput == NULL || index < 0) return;

	Nod* temp = lista->inceput;
	int i = 0;
	while (temp != NULL && i < index) {
		temp = temp->next;
		i++;
	}

	if (temp == NULL) return;
	Nod* t = temp->prev;
	if (temp->prev != NULL) {
		t->next = temp->next;
	}
	else {
		lista->inceput = temp->next;
	}

	Nod* aux = temp->next;
	if (temp->next != NULL) {
		aux->prev = temp->prev;
	}
	else {
		lista->final = temp->prev;
	}
	if (temp->info.adresa) {
		free(temp->info.adresa);
	}
	free(temp);
}

void sortareCrescatorDupaPret(ListaDubla* lista, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = NULL;
	nou->prev = NULL;

	if (lista->inceput == NULL) {
		lista->inceput = nou;
		lista->final = nou;
		return;
	}

	Nod* temp = lista->inceput;

	while (temp && temp->info.pret < aNou.pret) {
		temp = temp->next;
	}

	if (temp == NULL) {
		nou->prev = lista->final;
		lista->final->next = nou;
		lista->final = nou;
	}
	else if (temp == lista->inceput) {
		nou->next = lista->inceput;
		lista->inceput->prev = nou;
		lista->inceput = nou;
	}
	else {
		Nod* aux = temp->prev;
		nou->prev = temp->prev;
		nou->next = temp;
		aux->next = nou;
		temp->prev = nou;
	}
}

void sortareDescrescatorDupaPret(ListaDubla* lista, Apartament aNou) {
	Nod* nou = (Nod*)malloc(sizeof(Nod));
	nou->info = aNou;
	nou->next = NULL;
	nou->prev = NULL;

	if (lista->inceput == NULL) {
		lista->inceput = nou;
		lista->final = nou;
		return;
	}

	Nod* temp = lista->inceput;

	while (temp && temp->info.pret > aNou.pret) {
		temp = temp->next;
	}

	if (temp == NULL) {
		nou->prev = lista->final;
		lista->final->next = nou;
		lista->final = nou;
	}
	else if (temp == lista->inceput) {
		nou->next = lista->inceput;
		lista->inceput->prev = nou;
		lista->inceput = nou;
	}
	else {
		Nod* aux = temp->prev;
		nou->prev = temp->prev;
		nou->next = temp;
		aux->next = nou;
		temp->prev = nou;
	}
}

void afisareListaSortataCrescatorDupaPret(ListaDubla* lista) {
	ListaDubla listaSortata;
	listaSortata.inceput = NULL;
	listaSortata.final = NULL;

	Nod* temp = lista->inceput;
	while (temp) {
		Apartament aNou;
		aNou.id = temp->info.id;
		aNou.suprafata = temp->info.suprafata;
		aNou.pret = temp->info.pret;
		aNou.nrCamere = temp->info.nrCamere;
		aNou.adresa = (char*)malloc(strlen(temp->info.adresa) + 1);
		strcpy(aNou.adresa, temp->info.adresa);

		sortareCrescatorDupaPret(&listaSortata, aNou);

		temp = temp->next;
	}

	printf("\nLista sortata crescator dupa pret:\n");
	afisareListaApartamenteDeLAInceput(&listaSortata);
}

void afisareListaSortataDescrescatorDupaPret(ListaDubla* lista) {
	ListaDubla listaSortata;
	listaSortata.inceput = NULL;
	listaSortata.final = NULL;

	Nod* temp = lista->inceput;
	while (temp) {
		Apartament aNou;
		aNou.id = temp->info.id;
		aNou.suprafata = temp->info.suprafata;
		aNou.pret = temp->info.pret;
		aNou.nrCamere = temp->info.nrCamere;
		aNou.adresa = (char*)malloc(strlen(temp->info.adresa) + 1);
		strcpy(aNou.adresa, temp->info.adresa);

		sortareDescrescatorDupaPret(&listaSortata, aNou);

		temp = temp->next;
	}

	printf("\nLista sortata descrescator dupa pret:\n");
	afisareListaApartamenteDeLAInceput(&listaSortata);
}

//suprafata mai mare decat 60
Apartament* apartCuSuprafataMare(ListaDubla* lista, int* dimVector) {
	int count = 0;
	Nod* temp = lista->inceput;

	while (temp) {
		if (temp->info.suprafata > 60) {
			count++;
		}
		temp = temp->next;
	}
	*dimVector = count;

	Apartament* nou = (Apartament*)malloc(sizeof(Apartament) * (*dimVector));
	temp = lista->inceput;
	int i = 0;
	while (temp) {
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

void dezalocareListaApartamente(ListaDubla* lista) {
	while (lista->inceput) {
		if (lista->inceput->info.adresa) {
			free(lista->inceput->info.adresa);
		}
		Nod* temp = lista->inceput;
		lista->inceput = temp->next;
		free(temp);
	}
	lista->final = NULL;
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

	ListaDubla lista = citireListaMasiniDinFisier("apartamente.txt");
	afisareListaApartamenteDeLAInceput(&lista);
	printf("\n");
	afisareListaApartamenteDeLaFinal(&lista);
	stergereNodDinLista(&lista, 2);
	afisareListaApartamenteDeLAInceput(&lista);

	Apartament a1 = initalizareApartament(1, "Bulevardul Unirii 45", 65.2, 85000.0, 3);
	Apartament a2 = initalizareApartament(2, "Strada Muncii 12", 48.5, 62000.0, 2);
	Apartament a3 = initalizareApartament(3, "Calea Dorobantilor 8", 74.3, 109000.0, 3);
	Apartament a4 = initalizareApartament(4, "Strada Lalelelor 23", 52.0, 67000.0, 2);
	ListaDubla lista2;
	lista2.inceput = NULL;
	lista2.final = NULL;
	printf("\nLista sortata\n");
	sortareCrescatorDupaPret(&lista2, a1);
	sortareCrescatorDupaPret(&lista2, a2);
	sortareCrescatorDupaPret(&lista2, a3);
	sortareCrescatorDupaPret(&lista2, a4);
	afisareListaApartamenteDeLaFinal(&lista2);

	afisareListaSortataCrescatorDupaPret(&lista);
	afisareListaSortataDescrescatorDupaPret(&lista);

	printf("\nApartamentele cu suprafata mai mare decat 60\n");
	int dim = 0;
	Apartament* suprafata = apartCuSuprafataMare(&lista, &dim);
	for (int i = 0; i < dim; i++) {
		afisareApartament(suprafata[i]);
	}

	dezalocareVectorApartamente(&suprafata, &dim);

	dezalocareListaApartamente(&lista2);
	dezalocareListaApartamente(&lista);


	return 0;
}