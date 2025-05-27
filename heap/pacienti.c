/*#define _CRT_SECURE_NO_WARNINGS
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
	Pacient* vector;
	int lungime;
	int nrPacienti;
}Heap;

Heap intializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.vector = malloc(sizeof(Pacient) * lungime);
	heap.nrPacienti = 0;
	return heap;
}

Pacient citireFisier(FILE* f) {
	Pacient p;
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, f);
	p.id = atoi(strtok(buffer, sep));
	aux = strtok(NULL, sep);
	p.nume = malloc(strlen(aux) + 1);
	strcpy(p.nume, aux);
	p.varsta = atoi(strtok(NULL, sep));
	p.gradUrgenta = atoi(strtok(NULL, sep));

	return p;
}

void filtrareHeap(Heap heap, int pozitieNod) {
	int pozStanga = 2 * pozitieNod + 1;
	int pozDreapta = 2 * pozitieNod + 2;
	int pozMax = pozitieNod;
	if (pozStanga < heap.nrPacienti && heap.vector[pozStanga].gradUrgenta > heap.vector[pozitieNod].gradUrgenta) {
		pozMax = pozStanga;
	}
	else if (pozDreapta < heap.nrPacienti && heap.vector[pozDreapta].gradUrgenta > heap.vector[pozitieNod].gradUrgenta) {
		pozMax = pozDreapta;
	}
	if (pozitieNod != pozMax) {
		Pacient aux = heap.vector[pozMax];
		heap.vector[pozMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;
		if (pozMax < (heap.nrPacienti - 1) / 2) {
			filtrareHeap(heap, pozMax);
		}
	}
}

Heap citireHeapDinFisier(char* numeFisier, int lungime) {
	Heap heap = intializareHeap(lungime);
	FILE* f = fopen(numeFisier, "r");
	while (!feof(f)) {
		Pacient p = citireFisier(f);
		heap.vector[heap.nrPacienti] = p;
		heap.nrPacienti++;
	}
	fclose(f);
	for (int i = (heap.nrPacienti - 2) / 2; i >= 0; i--) {
		filtrareHeap(heap, i);
	}
	return heap;
}

void afisarePacient(Pacient p) {
	printf("Id: %d\n", p.id);
	printf("Nume: %s\n", p.nume);
	printf("Varsta: %d\n", p.varsta);
	printf("Grad urgenta: %d\n", p.gradUrgenta);
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrPacienti; i++) {
		afisarePacient(heap.vector[i]);
	}
}

Pacient extragereDinHeap(Heap* heap) {
	Pacient p = heap->vector[0];
	p.nume = (char*)malloc(strlen(heap->vector[0].nume) + 1);
	strcpy(p.nume, heap->vector[0].nume);
	free(heap->vector[0].nume);
	heap->vector[0] = heap->vector[heap->nrPacienti - 1];
	heap->nrPacienti--;
	for (int i = 0; i < heap->nrPacienti; i++) {
		filtrareHeap(*heap, i);
	}
	return p;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->nrPacienti; i++) {
		free(heap->vector[i].nume);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrPacienti = 0;
}

int main() {

	Heap heap = citireHeapDinFisier("pacienti.txt", 10);
	afisareHeap(heap);

	Pacient p = extragereDinHeap(&heap);
	printf("\nPacientul extras:\n");
	afisarePacient(p);
	printf("\n");
	afisareHeap(heap);

	dezalocareHeap(&heap);

	return 0;
}

*/