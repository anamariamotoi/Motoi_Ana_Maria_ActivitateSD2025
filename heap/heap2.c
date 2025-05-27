#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct {
	int id;
	int nrUsi;
	float pret;
	char* model;
}Masina;

typedef struct {
	Masina* vector;
	int lungime;
	int nrMasini;
}Heap;

Masina citesteMasinaDinFisier(FILE* f) {
	Masina m;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, f);
	m.id = atoi(strtok(buffer, sep));
	m.nrUsi = atoi(strtok(NULL, sep));
	m.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m.model = (char*)malloc(strlen(aux) + 1);
	strcpy(m.model, aux);

	return m;
}

void afisareMasina(Masina m) {
	printf("Id: %d\n", m.id);
	printf("Numar usi: %d\n", m.nrUsi);
	printf("Pret: %.2f\n", m.pret);
	printf("Model: %s\n", m.model);
}

Heap intializareHeap(int lungime) {
	Heap heap;
	heap.vector = (Masina*)malloc(sizeof(Masina) * lungime);
	heap.lungime = lungime;
	heap.nrMasini = 0;
	return heap;
}

void filtrareHeap(Heap heap, int nodPozitie) {
	int pozStanga = 2 * nodPozitie + 1;
	int pozDreapta = 2 * nodPozitie + 2;
	int pozMax = nodPozitie;
	if (pozStanga< heap.nrMasini && heap.vector[pozStanga].id > heap.vector[pozMax].id) {
		pozMax = pozStanga;
	}
	if (pozDreapta< heap.nrMasini && heap.vector[pozDreapta].id > heap.vector[pozMax].id) {
		pozMax = pozDreapta;
	}
	if(pozMax!=nodPozitie) {
		Masina aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[nodPozitie];
		heap.vector[nodPozitie] = aux;
		if (pozMax < (heap.nrMasini - 1) / 2 ) {
			filtrareHeap(heap, pozMax);
		}
	}
}

Heap citesteHeapDinFisier(char* numeFisier, int lungime) {
	Heap heap = intializareHeap(lungime);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Masina m = citesteMasinaDinFisier(f);
		heap.vector[heap.nrMasini] = m;
		heap.nrMasini++;
	}
	fclose(f);
	for (int i = (heap.nrMasini - 2) / 2; i >= 0; i--) {
		filtrareHeap(heap, i);
	}
	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++) {
		afisareMasina(heap.vector[i]);
	}
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrMasini; i++) {
		free(heap->vector[i].model);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->nrMasini = 0;
	heap->lungime = 0;


}

Masina extrageMasinaDinHeap(Heap* heap) {
	Masina m = heap->vector[0];
	m.model = (char*)malloc(strlen(heap->vector[0].model) + 1);
	strcpy(m.model, heap->vector[0].model);
	free(heap->vector[0].model);
	heap->vector[0] = heap->vector[heap->nrMasini - 1];
	heap->nrMasini--;
	for (int i = (heap->nrMasini - 2) / 2; i >= 0; i--) {
		filtrareHeap(*heap, i);
	}
	return m;
}

int main() {

	Heap heap = citesteHeapDinFisier("masini.txt", 5);
	afisareHeap(heap);

	printf("\nMasina extrasa\n");
	Masina m = extrageMasinaDinHeap(&heap);
	afisareMasina(m);
	printf("\n");
	
	afisareHeap(heap);

	dezalocareHeap(&heap);

	return 0;
}