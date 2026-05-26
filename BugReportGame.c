/*
Se considera structura BugReport, care retine:

* id
* timpEstimare
* titluBug
* tester
* severitate

Sa se creeze o structura de tip Heap, reprezentata prin:

* vector de bug reports
* lungimea vectorului
* numarul de elemente vizibile

Heap-ul va fi de tip MAX-HEAP dupa severitate.

Se cer urmatoarele functii:

BugReport citireBugDinFisier(FILE* file);

void afisareBug(BugReport bug);

Heap initializareHeap(int lungime);

void filtreazaHeap(Heap heap, int pozitieNod);

Heap citireHeapDeBuguriDinFisier(const char* numeFisier);

void afisareHeap(Heap heap);

void afiseazaHeapAscuns(Heap heap);

BugReport extrageBug(void* heap);

void dezalocareHeap(Heap* heap);
*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraBug {
	int id;
	float timpEstimat;
	char* titluBug;
	char* tester;
	int severitate;
};
typedef struct StructuraBug BugReport;

struct Heap {
	BugReport* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

BugReport citireBugDinFisier(FILE* file) {
	char buffer[256];
	char sep[3] = ",\n";
	fgets(buffer, 256, file);

	BugReport bug;

	char* aux = strtok(buffer, sep);
	bug.id = atoi(aux);

	bug.timpEstimat = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	bug.titluBug = malloc(strlen(aux) + 1);
	strcpy(bug.titluBug, aux);

	aux = strtok(NULL, sep);
	bug.tester = malloc(strlen(aux) + 1);
	strcpy(bug.tester, aux);

	bug.severitate = atoi(strtok(NULL, sep));

	return bug;
}

void afisareBug(BugReport bug) {
	printf("Id: %d\n", bug.id);
	printf("Timp estimat: %.2f\n", bug.timpEstimat);
	printf("Titlu bug: %s\n", bug.titluBug);
	printf("Tester: %s\n", bug.tester);
	printf("Severitate: %d\n\n", bug.severitate);
}

Heap initializareHeap(int lungime) {
	Heap heap;

	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = malloc(sizeof(BugReport) * lungime);

	for (int i = 0; i < lungime; i++) {
		heap.vector[i].titluBug = NULL;
		heap.vector[i].tester = NULL;
	}

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	int pozitieMax = pozitieNod;

	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;

	if (pozitieStanga < heap.nrElemente &&
		heap.vector[pozitieStanga].severitate > heap.vector[pozitieMax].severitate) {
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente &&
		heap.vector[pozitieDreapta].severitate > heap.vector[pozitieMax].severitate) {
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitieNod) {
		BugReport aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = aux;

		filtreazaHeap(heap, pozitieMax);
	}
}

Heap citireHeapDeBuguriDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");

	Heap heap = initializareHeap(5);

	if (file == NULL) {
		return heap;
	}

	while (!feof(file) && heap.nrElemente < heap.lungime) {
		BugReport bug = citireBugDinFisier(file);
		heap.vector[heap.nrElemente] = bug;
		heap.nrElemente++;
	}

	for (int i = heap.nrElemente / 2 - 1; i >= 0; i--) {
		filtreazaHeap(heap, i);
	}

	fclose(file);

	return heap;
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrElemente; i++) {
		afisareBug(heap.vector[i]);
	}
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrElemente; i < heap.lungime; i++) {
		if (heap.vector[i].titluBug != NULL) {
			afisareBug(heap.vector[i]);
		}
	}
}

BugReport extrageBug(void* heap) {
	Heap* h = (Heap*)heap;

	BugReport bug;
	bug.id = -1;
	bug.timpEstimat = 0;
	bug.titluBug = NULL;
	bug.tester = NULL;
	bug.severitate = 0;

	if (h->nrElemente <= 0) {
		return bug;
	}

	bug = h->vector[0];

	h->vector[0] = h->vector[h->nrElemente - 1];
	h->vector[h->nrElemente - 1] = bug;

	h->nrElemente--;

	filtreazaHeap(*h, 0);

	return bug;
}

void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++) {
		free(heap->vector[i].titluBug);
		free(heap->vector[i].tester);
	}

	free(heap->vector);

	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDeBuguriDinFisier("buguri.txt");

	printf("HEAP VIZIBIL INITIAL\n");
	afisareHeap(heap);

	BugReport bugExtras = extrageBug(&heap);

	printf("BUG EXTRAS\n");
	afisareBug(bugExtras);

	printf("HEAP VIZIBIL DUPA EXTRAGERE\n");
	afisareHeap(heap);

	printf("HEAP ASCUNS\n");
	afiseazaHeapAscuns(heap);

	dezalocareHeap(&heap);

	return 0;
}
