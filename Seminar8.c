#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
1. Ce este un heap

Un heap este o structura de date logica de tip arbore binar complet, dar implementata practic intr-un vector.

Adica nu avem noduri cu pointeri stanga/dreapta, ci doar un vector.

2. Cum se retine arborele in vector

Pentru un element aflat pe pozitia i:
	stanga = 2 * i + 1;
	dreapta = 2 * i + 2;
	parinte = (i - 1) / 2;

Exemplu:
pozitie:  0   1   2   3   4   5   6
vector:  [A] [B] [C] [D] [E] [F] [G]

Arbore logic:
		A
	  /   \
	 B     C
	/ \   / \
   D   E F   G

3. Max-Heap vs Min-Heap

Max-Heap:
	Parintele este mai mare decat copiii.
	radacina = elementul maxim

	Daca facem heap dupa pret, atunci:
	heap.vector[0] este masina cu pretul cel mai mare


Min-Heap
	Parintele este mai mic decat copiii.
	radacina = elementul minim

	Daca facem heap dupa pret, atunci:
	heap.vector[0] este masina cu pretul cel mai mic 


4. Ce inseamna filtrare

Filtrarea reface proprietatea de heap.
	La max-heap:

		1. te uiti la nodul curent
		2. te uiti la copilul stang
		3. te uiti la copilul drept
		4. alegi cel mai mare dintre cele 3
		5. daca cel mai mare nu e nodul curent, faci swap
		6. continui filtrarea pe pozitia unde ai mutat nodul

5. De ce folosim nrElemente si lungime
In structura:
			struct Heap {
			Masina* vector;
			int lungime;
			int nrElemente;
		};
		lungime = nr maxim de elemente => adica CAT SPATIU AM ALOCAT 
		nrElemente cate sunt in heap exact

6. Elemente vizibile vs ascunse
La extragere, in sablonul profesorului:
	elementul extras nu se sterge, doar se ascunde

	Adica:
		-il pun la final
		*Nota: -el doar se ascunde, ramane in vector, dar nu mai face parte din heap ul activ
		-scadem nr elemente
	Exemplu:
		lungime = 5
		nrElemente = 3

		[ elemente vizibile ][ elemente ascunse ]
		[ 0 ][ 1 ][ 2 ]       [ 3 ][ 4 ]

7. Extragerea din heap

La max-heap, extragerea inseamna:
	iau elementul maxim de pe pozitia 0

	Pasii:
	1. salvezi heap.vector[0]
	2. interschimbi primul element cu ultimul vizibil
	3. scazi nrElemente
	4. filtrezi din pozitia 0
	5. returnezi elementul salvat

8. Construirea heap-ului din fisier

In sablonul asta:

	1. citesti toate masinile in vector
	2. setezi nrElemente
	3. filtrezi vectorul ca sa devina heap

	Filtrarea se face de la ultimul nod parinte spre radacina:
		for (int i = heap.nrElemente / 2 - 1; i >= 0; i--) {
			filtreazaHeap(heap, i);

9. De ce incepi de la nrElemente / 2 - 1

Pentru ca de la pozitia:
	nrElemente/2 sunt frunze si pe ale anu le selectam pentru ca nu au copii

Ultimul parinte este:
	nrElemente/2 - 1

--------------Rezumat
10. Ce trebuie sa retii pentru test

Formule importante
		stanga = 2 * i + 1;
		dreapta = 2 * i + 2;
		parinte = (i - 1) / 2;

Max-heap dupa pret
		if (copil.pret > parinte.pret)

Min-heap dupa pret
		if (copil.pret < parinte.pret)

Extragere:
		salvez radacina
		swap radacina cu ultimul vizibil
		nrElemente--
		filtrez de la 0
		returnez radacina salvata

Esenta

Un heap este: vector + reguli de arbore + filtrare
}
*/

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	Masina* vector;
	int lungime;
	int nrElemente;
};
typedef struct Heap Heap;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

Heap initializareHeap(int lungime) {
	//initializeaza heap-ul cu 0 elemente 
	//dar cu o lungime primita ca parametru
	Heap heap;

	heap.lungime = lungime;
	heap.nrElemente = 0;
	heap.vector = malloc(sizeof(Masina) * lungime);

	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	//filtreaza heap-ul pentru nodul a carei pozitie o primeste ca parametru
	int pozitieMax = pozitieNod;

	int pozitieStanga = 2 * pozitieNod + 1;
	int pozitieDreapta = 2 * pozitieNod + 2;

	if (pozitieStanga < heap.nrElemente && heap.vector[pozitieStanga].pret > heap.vector[pozitieMax].pret)
	{
		pozitieMax = pozitieStanga;
	}

	if (pozitieDreapta < heap.nrElemente && heap.vector[pozitieDreapta].pret > heap.vector[pozitieMax].pret)
	{
		pozitieMax = pozitieDreapta;
	}

	if (pozitieMax != pozitieNod)
	{
		Masina aux = heap.vector[pozitieMax];
		heap.vector[pozitieMax] = heap.vector[pozitieNod];
		heap.vector[pozitieNod] = aux;

		filtreazaHeap(heap, pozitieMax);
	}
}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {
	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return initializareHeap(10);
	Heap heap = initializareHeap(10);
	
	while (!feof(file))
	{
		heap.vector[heap.nrElemente++] = citireMasinaDinFisier(file);
	}

	for (int i = (heap.nrElemente - 2) / 2; i >= 0; i--)
	//incepem direct cu ultimul nod care este parinte
	{
		filtreazaHeap(heap, i);
	}
	fclose(file);

	return heap;
}

void afisareHeap(Heap heap) {
	//afiseaza elementele vizibile din heap
	for (int i = 0; i < heap.nrElemente; i++)
	{
		afisareMasina(heap.vector[i]);
		printf("\n");
	}
}

void afiseazaHeapAscuns(Heap heap) {
	//afiseaza elementele ascunse din heap
	for (int i = heap.nrElemente; i < heap.lungime; i++)
	{
		afisareMasina(heap.vector[i]);
	}
}

Masina extrageMasina(Heap* heap) {
	// extrage si returneaza prima masina
	// elementul nu il stergem doar il facem invizibil(il ascundem)
	if (heap->nrElemente <= 0) {
		Masina m;
		m.id = -1;
		m.model = NULL;
		m.numeSofer = NULL;
		return m;
	}

	Masina m = heap->vector[0];

	heap->vector[0] = heap->vector[heap->nrElemente - 1];
	heap->vector[heap->nrElemente - 1] = m;

	heap->nrElemente--;

	filtreazaHeap(*heap, 0);

	return m;
}


void dezalocareHeap(Heap* heap) {
	//sterge toate elementele din Heap
	for (int i = 0; i < heap->lungime; i++) {

		free(heap->vector[i].model);
		free(heap->vector[i].numeSofer);
	}
	free(heap->vector);
	heap->vector = NULL;
	heap->lungime = 0;
	heap->nrElemente = 0;
}

int main() {
	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");

	printf("Afisare heap vizibil:\n");
	afisareHeap(heap);
	printf("Afisare heap ascuns:\n");
	afiseazaHeapAscuns(heap);

	Masina m = extrageMasina(&heap);
	printf("Afisare masina extrasa (prima masina):\n");
	afisareMasina(m);

	printf("Afisare heap ascuns:\n");
	afiseazaHeapAscuns(heap);

	//dezalocareHeap(&heap);
	return 0;
}