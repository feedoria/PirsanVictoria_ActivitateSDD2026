#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraPacient
{
	int id;
	int varsta;
	char* numePacient;
	char* boala; // orice pacient are o boala
	float gradUrgenta;
};
typedef struct StructuraPacient Pacient;

struct Heap
{
	Pacient* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

Pacient citirePacientDinFisier(FILE* file)
{
	char linie[256];

	Pacient p;
	p.id = -1;
	p.varsta = -1;
	p.numePacient = NULL;
	p.boala = NULL;
	p.gradUrgenta = -1;

	if (file == NULL)
	{
		return p;
	}

	fgets(linie, 256, file);
	char sep[3] = ",\n";

	char* buffer;
	p.id = atoi(strtok(linie, sep));
	p.varsta = atoi(strtok(NULL, sep));
	buffer = strtok(NULL, sep);
	p.numePacient = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(p.numePacient, buffer);
	buffer = strtok(NULL, sep);
	p.boala = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(p.boala, buffer);
	p.gradUrgenta = atof(strtok(NULL, sep));

	return p;
}

Heap initializareHeap(int lungime)
{
	Heap heap;

	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = malloc(sizeof(Pacient) * lungime);

	for (int i = 0; i < lungime; i++)
	{
		heap.vector[i].numePacient = NULL;
		heap.vector[i].boala = NULL;
	}
	return heap;
}

void filtrareHeap(Heap heap, int pozitieNod)
{
	int pozitieMax = pozitieNod;

	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;

	if (pozitieStanga < heap.nrElemente && heap.vector[pozitieStanga].gradUrgenta > heap.vector[pozitieMax].gradUrgenta)
	{
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente && heap.vector[pozitieDreapta].gradUrgenta > heap.vector[pozitieMax].gradUrgenta)
	{
		pozitieMax = pozitieDreapta;
	}

	if (pozitieNod != pozitieMax)
	{
		Pacient aux = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = aux;

		filtrareHeap(heap, pozitieMax);
	}
}

void adaugaPacientInHeap(Heap* heap, Pacient pacientNou)
{
	if (heap->nrElemente <= 0)
	{
		heap->vector[0] = pacientNou;
		heap->nrElemente++;
	}
	else
	{
		heap->vector[heap->nrElemente++] = pacientNou;
		filtrareHeap(*heap, heap->nrElemente - 1);
	}
}

Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(6);
	if (file == NULL) return heap;

	while (!feof(file))
	{
		heap.vector[heap.nrElemente++] = citirePacientDinFisier(file);
	}

	for (int i = (heap.nrElemente - 1) / 2; i >= 0; i--)
	{
		filtrareHeap(heap, i);
	}
	return heap;
}

void afisarePacient(Pacient p)
{
	printf("Id: %d\n", p.id);
	printf("Varsta: %d\n", p.varsta);
	printf("Nume pacient: %s\n", p.numePacient);
	printf("Boala: %s\n", p.boala);
	printf("Grad severitate: %.2f\n", p.gradUrgenta);
}

void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElemente; i++)
	{
		afisarePacient(heap.vector[i]);
		printf("\n");
	}
}

Pacient deepCopyPacient(Pacient p) {
	Pacient copie;
	copie.id = p.id;
	copie.varsta = p.varsta;
	copie.gradUrgenta = p.gradUrgenta;

	copie.numePacient = malloc(strlen(p.numePacient) + 1);
	strcpy(copie.numePacient, p.numePacient);

	copie.boala = malloc(strlen(p.boala) + 1);
	strcpy(copie.boala, p.boala);

	return copie;
}

Pacient extragePacient(Heap* heap)
{
	if (heap->nrElemente <= 0)
	{
		Pacient p;
		p.id = -1;
		p.varsta = -1;
		p.numePacient = NULL;
		p.boala = NULL;
		p.gradUrgenta = -1;

		return p;
	}
	Pacient pExtras = deepCopyPacient(heap->vector[0]);
	Pacient p = heap->vector[0];
	heap->vector[0] = heap->vector[heap->nrElemente - 1];
	heap->vector[heap->nrElemente - 1] = p;

	heap->nrElemente--;

	filtrareHeap(*heap, 0);

	return pExtras;
}

struct NodSimplu
{
	Pacient pacient;
	struct NodSimplu* next;
};
typedef struct NodSimplu Nod;

void adaugaPacientInListaSimpla(Nod** lista, Pacient pacient)
{
	// adaug la inceput 
	Nod* nod = malloc(sizeof(Nod));
	nod->next = (*lista);
	//nod->pacient = pacient; shallow
	nod->pacient = deepCopyPacient(pacient); //deep
	(*lista) = nod;
}

Nod* salveazaPacientiInLD(Heap heap)
{
	Nod* lista = NULL;
	for (int i = 0; i < heap.nrElemente; i++)
	{
		if (heap.vector[i].varsta >= 60)
		{
			adaugaPacientInListaSimpla(&lista, heap.vector[i]);
		}
	}
	return lista;
}

void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].numePacient);
		free(heap->vector[i].boala);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

void dezalocareListaSimpla(Nod** lista)
{
	Nod* current = *lista;

	while (current)
	{
		Nod* temp = current;
		current = current->next;
		free(temp->pacient.numePacient);
		free(temp->pacient.boala);
		free(temp);
	}
	
	*lista = NULL;
}

void afisareLista(Nod* lista)
{
	Nod* current = lista;

	while (current)
	{
		afisarePacient(current->pacient);
		current = current->next;
	}
}

int main()
{
	Heap heap = citireHeapDinFisier("pacienti.txt");

	printf("Afisare pacienti din heap:\n");
	afisareHeap(heap);

	Pacient p = extragePacient(&heap);
	afisarePacient(p);

	Nod* lista = salveazaPacientiInLD(heap);
	afisareLista(lista);

	free(p.numePacient);
	free(p.boala);
	dezalocareHeap(&heap);
	dezalocareListaSimpla(&lista);
	return 0;
}