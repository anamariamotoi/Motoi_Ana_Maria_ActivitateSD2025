#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//Task-uri suplimentare 
//8.HashTable


typedef struct {
    int id;
    char* adresa;
    int anConstructie;
    float suprafata;
}Cladire;

typedef struct Nod {
    Cladire info;
    struct Nod* next;
}Nod;

typedef struct {
    int dim;
    Nod** vector;
}HashTable;

Cladire citireCladireDinFisier(FILE* f) {
    Cladire c;
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;

    fgets(buffer, 100, f);
    c.id = atoi(strtok(buffer, sep));
    aux = strtok(NULL, sep);
    c.adresa = (char*)malloc(strlen(aux) + 1);
    strcpy(c.adresa, aux);
    c.anConstructie = atoi(strtok(NULL, sep));
    c.suprafata = atof(strtok(NULL, sep));

    return c;
}

HashTable initializareHashTable(int dim) {
    HashTable ht;
    ht.dim = dim;
    ht.vector = (Nod**)malloc(sizeof(Nod*) * dim);
    for (int i = 0; i < dim; i++)
        ht.vector[i] = NULL;
    return ht;
}

int calculeazaHash(int anConstructie, int dim) {
    return dim > 0 ? (anConstructie % dim) : -1;
}

void adaugaCladireInLista(Nod** cap, Cladire c) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = c;
    nou->next = *cap;
    *cap = nou;
}

void inserareCladire(HashTable ht, Cladire c) {
    int poz = calculeazaHash(c.anConstructie, ht.dim);
    adaugaCladireInLista(&ht.vector[poz], c);
}

void afisareCladire(Cladire c) {
    printf("Id: %d\n", c.id);
    printf("Adresa: %s\n", c.adresa);
    printf("An constructie: %d\n", c.anConstructie);
    printf("Suprafata: %.2f\n", c.suprafata);
}

HashTable citireTabelaDinFisier(const char* numeFisier, int dimTabela) {
    FILE* f = fopen(numeFisier, "r");
    HashTable ht = initializareHashTable(dimTabela);
    while (!feof(f)) {
        Cladire c = citireCladireDinFisier(f);
        if (c.adresa != NULL)
            inserareCladire(ht, c);
    }
    fclose(f);
    return ht;
}

void afisareCladiriDinAn(HashTable ht, int an) {
    int poz = calculeazaHash(an, ht.dim);
    Nod* temp = ht.vector[poz];
    while (temp) {
        if (temp->info.anConstructie == an)
            afisareCladire(temp->info);
        temp = temp->next;
    }
}

void stergeCladireDupaIDSiAn(HashTable* ht, int id, int an) {
    int poz = calculeazaHash(an, ht->dim);
    Nod* temp = ht->vector[poz];
    Nod* anterior = NULL;

    while (temp) {
        if (temp->info.id == id && temp->info.anConstructie == an) {
            if (anterior)
                anterior->next = temp->next;
            else
                ht->vector[poz] = temp->next;

            free(temp->info.adresa);
            free(temp);
            return;
        }
        anterior = temp;
        temp = temp->next;
    }
}

void stergeCladireDupaID(HashTable* ht, int id) {
    for (int i = 0; i < ht->dim; i++) {
        Nod* temp = ht->vector[i];
        Nod* anterior = NULL;
        while (temp) {
            if (temp->info.id == id) {
                if (anterior)
                    anterior->next = temp->next;
                else
                    ht->vector[i] = temp->next;

                free(temp->info.adresa);
                free(temp);
                return;
            }
            anterior = temp;
            temp = temp->next;
        }
    }
}

Cladire* getCladiriDinAn(HashTable ht, int an, int* nrCladiri) {
    int poz = calculeazaHash(an, ht.dim);
    Nod* temp = ht.vector[poz];
    *nrCladiri = 0;

    Nod* p = temp;
    while (p) {
        if (p->info.anConstructie == an)
            (*nrCladiri)++;
        p = p->next;
    }

    if (*nrCladiri == 0) return NULL;

    Cladire* vector = (Cladire*)malloc(sizeof(Cladire) * (*nrCladiri));
    int i = 0;
    while (temp) {
        if (temp->info.anConstructie == an) {
            vector[i].id = temp->info.id;
            vector[i].anConstructie = temp->info.anConstructie;
            vector[i].suprafata = temp->info.suprafata;
            vector[i].adresa = (char*)malloc(strlen(temp->info.adresa) + 1);
            strcpy(vector[i].adresa, temp->info.adresa);
            i++;
        }
        temp = temp->next;
    }
    return vector;
}

void modificaAnConstructie(HashTable* ht, int id, int anVechi, int anNou) {
    int poz = calculeazaHash(anVechi, ht->dim);
    Nod* temp = ht->vector[poz];
    Nod* anterior = NULL;

    while (temp) {
        if (temp->info.id == id && temp->info.anConstructie == anVechi) {
            Cladire c = temp->info;

            if (anterior)
                anterior->next = temp->next;
            else
                ht->vector[poz] = temp->next;

            free(temp);
            c.anConstructie = anNou;
            inserareCladire(*ht, c);
            return;
        }
        anterior = temp;
        temp = temp->next;
    }
}

void stergeLista(Nod** cap) {
    while (*cap) {
        Nod* temp = *cap;
        *cap = (*cap)->next;
        free(temp->info.adresa);
        free(temp);
    }
}

void dezalocareTabela(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++)
        stergeLista(&ht->vector[i]);
    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

int main() {
    HashTable ht = citireTabelaDinFisier("cladiri.txt", 5);

    printf("\nCladiri din anul 1990\n");
    afisareCladiriDinAn(ht, 1990);

    stergeCladireDupaIDSiAn(&ht, 2, 1990);
    afisareCladiriDinAn(ht, 1990);

    stergeCladireDupaID(&ht, 5);
    afisareCladiriDinAn(ht, 1977);

    printf("\nVector cladiri din anul 1990\n");
    int nr = 0;
    Cladire* vector = getCladiriDinAn(ht, 1990, &nr);
    for (int i = 0; i < nr; i++) {
        afisareCladire(vector[i]);
        free(vector[i].adresa);
    }
    free(vector);

    modificaAnConstructie(&ht, 4, 1990, 2000);
    afisareCladiriDinAn(ht, 1990);
    afisareCladiriDinAn(ht, 2000);

    dezalocareTabela(&ht);
    return 0;
}