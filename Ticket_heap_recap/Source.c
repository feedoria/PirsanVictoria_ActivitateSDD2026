#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Ticket {
	int id;
	float prioritate;
	char* descriere;
};
typedef struct Ticket Ticket;

struct Heap {
	Ticket* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

Ticket citireTicketDinFisier(FILE* file) {
	char linie[256];
	char sep[3] = ",\n";

	Ticket t;
	t.id = -1;
	t.prioritate = -1;
	t.descriere = NULL;

	if (file == NULL) return t;
	if (fgets(linie, 256, file) == NULL) return t;

	t.id = atoi(strtok(linie, sep));
	t.prioritate = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	t.descriere = malloc(strlen(buffer) + 1);
	strcpy(t.descriere, buffer);

	return t;
}

void afisareTicket(Ticket t) {
	printf("Id: %d\n", t.id);
	printf("Prioritate: %.2f\n", t.prioritate);
	printf("Descriere: %s\n\n", t.descriere);
}

// TODO: deepCopyTicket

Ticket deepCopyTicket(Ticket t)
{
	Ticket tk;
	tk.id = t.id;
	tk.prioritate = t.prioritate;
	tk.descriere = malloc(sizeof(char) * (strlen(t.descriere) + 1));
	strcpy(tk.descriere, t.descriere);

	return tk;
}

// TODO: initializareHeap

Heap initializareHeap(int lungime)
{
	Heap heap;

	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = malloc(sizeof(Ticket) * lungime);

	for (int i = 0; i < lungime; i++)
	{
		heap.vector[i].descriere = NULL;
	}
	return heap;
}

// TODO: filtrareHeap

void filtrareHeap(Heap heap, int pozitieNod)
{
	int pozitieMax = pozitieNod;

	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;

	if (pozitieStanga < heap.nrElemente && heap.vector[pozitieStanga].prioritate > heap.vector[pozitieMax].prioritate)
	{
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente && heap.vector[pozitieDreapta].prioritate > heap.vector[pozitieMax].prioritate)
	{
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitieNod)
	{
		Ticket t = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = t;

		filtrareHeap(heap, pozitieMax);
	}
}

// TODO: inserareTicketInHeap
void inserareTicketInHeap(Heap* heap, Ticket ticketNou)
{
	// ca vreau sa inserez un ticket o data cu functia asta
	if (heap->nrElemente + 1 <= heap->lungime) // are logica asta?
		heap->vector[heap->nrElemente++] = ticketNou;
}

// TODO: citireHeapDinFisier
Heap citireHeapDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(6);
	if (file == NULL) return heap;

	while (!feof(file))
	{
		Ticket t = citireTicketDinFisier(file);
		inserareTicketInHeap(&heap, t);
	}

	for (int i = (heap.nrElemente - 1) / 2; i >= 0; i--)
	{
		filtrareHeap(heap, i);
	}

	fclose(file);
	return heap;
}
// TODO: afisareHeap
void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElemente; i++)
	{
		afisareTicket(heap.vector[i]);
		printf("\n");
	}
}

// TODO: extrageTicket
Ticket extrageTicket(Heap* heap, float prioritate)
{
	Ticket tInvalid;
	tInvalid.id = -1;
	tInvalid.prioritate = -1;
	tInvalid.descriere = NULL;

	// aici il extrag mereu pe primul si il pun in zona de elemente ascunse si pe [0] il pun pe ultimul din zona buna
	Ticket ticketExtras = deepCopyTicket(heap->vector[0]);

	Ticket aux = heap->vector[0];
	heap->vector[0] = heap->vector[heap->nrElemente - 1];

	heap->nrElemente--;

	filtrareHeap(*heap, 0);

	return ticketExtras;
}

// TODO: structura lista simpla

// TODO: adaugaTicketInLista

// TODO: salveazaTicketePestePrag

// TODO: afisareLista

// TODO: dezalocareLista

// TODO: dezalocareHeap
void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].descriere);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDinFisier("tickete.txt");

	afisareHeap(heap);

	Ticket t = extrageTicket(&heap, 7.8);
	afisareTicket(t);

	//free(t.descriere);

	//dezalocareHeap(&heap);

	return 0;
}