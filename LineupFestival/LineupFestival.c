//#DEFINE CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
/*
Cerinta pe sablonul profului

Se considera structura Artist, care retine:

* id
* popularitate
* numeArtist
* genMuzical
* scena

Sa se creeze o structura de tip Heap, reprezentata prin:

* vector de artisti
* lungimea vectorului
* numarul de elemente vizibile

Heap-ul va fi de tip MAX-HEAP dupa popularitate.

Se cer urmatoarele functii:
*/
struct Artist
{
	int id;
	int popularitate;
	char* numeArtist;
	char* genMuzical;
	unsigned char scena;
};
typedef struct Artist Artist;

struct Heap
{
	Artist* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

Artist citireArtistDinFisier(FILE* file)
{
	char linie[256];
	Artist a;
	a.id = -1;
	a.popularitate = -1;
	a.numeArtist = NULL;
	a.genMuzical = NULL;
	a.scena = ' '; // ii pun spatiu default ca sa si cum ar fi gol 
	if (file == NULL) return a;
	fgets(linie, 256, file);
	char sep[3] = ",\n";

	char* buffer;
	a.id = atoi(strtok(linie, sep));
	a.popularitate = atoi(strtok(NULL, sep));
	buffer = strtok(NULL, sep);
	a.numeArtist = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(a.numeArtist, buffer);
	buffer = strtok(NULL, sep);
	a.genMuzical = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(a.genMuzical, buffer);

	buffer = strtok(NULL, sep);
	a.scena = buffer[0];

	return a;
}

void afisareArtist(Artist artist)
{
	printf("Id: %d\n", artist.id);
	printf("Popularitate: %d\n", artist.popularitate);
	printf("Nume: %s\n", artist.numeArtist);
	printf("Gen muzical: %s\n", artist.genMuzical);
	printf("Scena: %c\n", artist.scena);
}

Heap initializareHeap(int lungime)
{
	Heap heap;

	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = malloc(sizeof(struct Artist) * lungime);

	for (int i = 0; i < lungime; i++)
	{
		heap.vector[i].numeArtist = NULL;
		heap.vector[i].genMuzical = NULL;
	}

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod)
{
	int pozitieMax = pozitieNod;

	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;

	if (pozitieStanga < heap.nrElemente && heap.vector[pozitieStanga].popularitate > heap.vector[pozitieMax].popularitate)
	{
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente && heap.vector[pozitieDreapta].popularitate > heap.vector[pozitieMax].popularitate)
	{
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitieNod)
	{
		Artist a = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = a;

		filtreazaHeap(heap, pozitieMax);
	}
}

Heap citireHeapDeArtistiDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Heap heap = initializareHeap(5);
	if (file == NULL) return heap;

	while (!feof(file))
	{
		heap.vector[heap.nrElemente++] = citireArtistDinFisier(file);
	}

	//incepem cu ultimul parinte
	for (int i = (heap.nrElemente - 1) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}
	fclose(file);

	return heap;
}

void afisareHeap(Heap heap)
{
	for (int i = 0; i < heap.nrElemente; i++)
	{
		afisareArtist(heap.vector[i]);
		printf("\n");
	}
}

void afiseazaHeapAscuns(Heap heap)
{
	for (int i = heap.nrElemente; i < heap.lungime; i++)
	{
		afisareArtist(heap.vector[i]);
		printf("\n");
	}
}

Artist extrageArtist(Heap* heap)
{
	// il luam pe primul si nu il stergem, il ascundem deci il punem la urma si facem schimb cu uktimul nod
	if (heap->nrElemente <= 0)
	{
		Artist artistInvalid;
		artistInvalid.id = -1;
		artistInvalid.popularitate = -1;
		artistInvalid.numeArtist = NULL;
		artistInvalid.genMuzical = NULL;
		artistInvalid.scena = ' ';

		return artistInvalid;
	}

	Artist a = heap->vector[0];
	heap->vector[0] = heap->vector[heap->nrElemente - 1];
	heap->vector[heap->nrElemente - 1] = a;

	heap->nrElemente--;

	for (int i = (heap->nrElemente - 1) / 2; i >= 0; i--)
	{
		filtreazaHeap(*heap, i);
	}
	return a;
}

void dezalocareHeap(Heap* heap)
{
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->vector[i].numeArtist);
		free(heap->vector[i].genMuzical);
	}

	free(heap->vector);
	heap->lungime = 0;
	heap->nrElemente = 0;
	heap->vector = NULL;
}

int main()
{
	Heap heap = citireHeapDeArtistiDinFisier("artisti.txt");

	printf("Afisare artisti din heap:\n");
	afisareHeap(heap);

	Artist a = extrageArtist(&heap);
	printf("Afisare artist extras:\n");
	afisareArtist(a);

	printf("Afisare artisti din heap ascuns:\n");
	afiseazaHeapAscuns(heap);
	return 0;
}