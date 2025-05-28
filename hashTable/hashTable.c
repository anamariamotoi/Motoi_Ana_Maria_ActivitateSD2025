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
}Apartament;

typedef struct Nod {
    Apartament info;
    struct Nod* next;
}Nod;

typedef struct {
    int dim;
    Nod** vector;
}HashTable;


Apartament citireApartamentDinFisier(FILE* f) {
    char buffer[256];
    char sep[] = ",\n";
    fgets(buffer, 256, f);
    Apartament a;
    char* aux = strtok(buffer, sep);
    a.id = atoi(aux);
    aux = strtok(NULL, sep);
    a.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy(a.adresa, aux);
    a.suprafata = atof(strtok(NULL, sep));
    a.pret = atof(strtok(NULL, sep));
    a.nrCamere = atoi(strtok(NULL, sep));

    return a;
}

void adaugaApartamentInLista(Nod** cap, Apartament a) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = a; 
    nou->next = *cap;
    *cap = nou;
}

void afisareApartament(Apartament a) {
    printf("Id: %d\n", a.id);
    printf("Adresa: %s\n", a.adresa);
    printf("Suprafata: %.2f\n", a.suprafata);
    printf("Pret: %.2f\n", a.pret);
    printf("Numar camere: %d\n", a.nrCamere);
}

void afisareListaApartamente(Nod* cap) {
    while (cap) {
        afisareApartament(cap->info);
        cap = cap->next;
    }
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++) {
        ht.vector[i] = NULL;
    }
    return ht;
}

int calculeazaHash(const char* adresa, int id, int dim) {
    int suma = 0;
    for (int i = 0; i < strlen(adresa); i++) {
        suma += adresa[i];
    }
    suma += id;
    return dim > 0 ? (suma % dim) : -1;
}

void inserareInTabela(HashTable ht, Apartament a) {
    int poz = calculeazaHash(a.adresa, a.id, ht.dim);
    if (poz >= 0 && poz < ht.dim) {
        adaugaApartamentInLista(&(ht.vector[poz]), a);
    }
}

HashTable citireTabelaDinFisier(const char* numeFisier, int dim) {
    HashTable ht = initializareHashTable(dim);
    FILE* f = fopen(numeFisier, "r");
    if (f) {
        while (!feof(f)) {
            Apartament a = citireApartamentDinFisier(f);
            inserareInTabela(ht, a);
        }
        fclose(f);
    }
    return ht;
}


Apartament getApartamentDupaID(Nod* cap, int idCautat) {
    while (cap) {
        if (cap->info.id == idCautat) return cap->info;
        cap = cap->next;
    }
    Apartament a;
    a.id = -1;
    return a;
}

Apartament getApartamentDupaAdresaSiID(HashTable ht, const char* adresa, int id) {
    int poz = calculeazaHash(adresa, id, ht.dim);
    return getApartamentDupaID(ht.vector[poz], id);
}

void stergeLista(Nod** cap) {
    while (*cap) {
        Nod* p = *cap;
        *cap = (*cap)->next;
        free(p->info.adresa);
        free(p);
    }
}

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        stergeLista(&ht->vector[i]);
    }
    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

float calculeazaPretMediu(Nod* cap) {
    float suma = 0;
    int count = 0;
    while (cap) {
        suma += cap->info.pret;
        count++;
        cap = cap->next;
    }
    return count > 0 ? suma / count : 0;
}

float** calculeazaMediiPeClustere(HashTable ht, int* nrClustere) {
    *nrClustere = 0;
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i] != NULL) (*nrClustere)++;
    }

    float** matrice = (float**)malloc(2 * sizeof(float*));
    matrice[0] = (float*)malloc(*nrClustere * sizeof(float));
    matrice[1] = (float*)malloc(*nrClustere * sizeof(float));

    int index = 0;
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i]) {
            matrice[0][index] = i;
            matrice[1][index] = calculeazaPretMediu(ht.vector[i]);
            index++;
        }
    }
    return matrice;
}

int main() {
    HashTable ht = citireTabelaDinFisier("apartamente.txt", 6);
    printf("\nTabela hash:\n");
    for (int i = 0; i < ht.dim; i++) {
        printf("Cluster %d:\n", i);
        afisareListaApartamente(ht.vector[i]);
    }

    Apartament a = getApartamentDupaAdresaSiID(ht, "Bulevardul Unirii 45", 1);
    if (a.id != -1) {
        printf("\nApartament gasit:\n");
        afisareApartament(a);
    }

    int nrClustere = 0;
    float** m = calculeazaMediiPeClustere(ht, &nrClustere);
    printf("\nMedii preturi pe clustere:\n");
    for (int i = 0; i < nrClustere; i++) {
        printf("Cluster %d -> %.2f RON\n", (int)m[0][i], m[1][i]);
    }

    dezalocareHashTable(&ht);
    free(m[0]);
    free(m[1]);
    free(m);

    return 0;
}
