#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul vrajitori.txt
//sau va creati un alt fisier cu alte date

struct StructuraVrajitor {
	int id;
	int anStudiu;
	float putereMagica;
	char* nume;
	char* casa;
	unsigned char bagheta;
};
typedef struct StructuraVrajitor Vrajitor;

//==============================
// VECTOR
// toti vrajitorii inregistrati la Hogwarts
//==============================

void afisareVrajitor(Vrajitor v) {
	//afiseaza toate atributele unui vrajitor
	printf("Id: %d\n", v.id);
	printf("An de studiu: %d\n", v.anStudiu);
	printf("Putere magica: %.2f\n", v.putereMagica);
	printf("Nume: %s\n", v.nume);
	printf("Casa: %s\n", v.casa);
	printf("Bagheta: %c\n", v.bagheta);
}

void afisareVectorVrajitori(Vrajitor* vrajitori, int nrVrajitori) {
	//afiseaza toate elemente de tip vrajitor din vector
	//prin apelarea functiei afisareVrajitor()
	for (int i = 0; i < nrVrajitori; i++)
	{
		afisareVrajitor(vrajitori[i]);
		printf("\n");
	}
}

void adaugaVrajitorInVector(Vrajitor** vrajitori, int *nrVrajitori, Vrajitor vrajitorNou) {
	//adauga in vectorul primit un nou vrajitor
	//ATENTIE - se modifica numarul de vrajitori din vector
	(*nrVrajitori)++;
	Vrajitor* temp = malloc(sizeof(Vrajitor) * (*nrVrajitori));

	for (int i = 0; i < (*nrVrajitori) - 1; i++)
	{
		temp[i] = (*vrajitori)[i];
	}

	temp[(*nrVrajitori) - 1] = vrajitorNou;

	free(*vrajitori);

	(*vrajitori) = temp;
}

Vrajitor citireVrajitorFisier(FILE*file) {
	//functia citeste un vrajitor dintr-un stream deja deschis
	//vrajitorul citit este returnat
	char linie[256];
	fgets(linie, 256, file);
	char sep[3] = ";\n";

	Vrajitor v;
	v.id = atoi(strtok(linie, sep));
	v.anStudiu = atoi(strtok(NULL, sep));
	v.putereMagica = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	v.nume = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(v.nume, buffer);

	buffer = strtok(NULL, sep);
	v.casa = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(v.casa, buffer);

	buffer = strtok(NULL, sep);
	v.bagheta = buffer[0];

	return v;
}

Vrajitor* citireVectorVrajitoriFisier(const char* numeFisier, int *nrVrajitori) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//numarul de vrajitori este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Vrajitor* vector = NULL;
	*nrVrajitori = 0;

	while (!feof(file))
	{
		adaugaVrajitorInVector(&vector, nrVrajitori, citireVrajitorFisier(file));
	}
	fclose(file);

	return vector;
}

void dezalocareVectorVrajitori(Vrajitor** vector, int *nrVrajitori) {
	//este dezalocat intreg vectorul de vrajitori
	for (int i = 0; i < (*nrVrajitori); i++)
	{
		free((*vector)[i].nume);
		free((*vector)[i].casa);
	}
	free(*vector);
	*vector = NULL;
	*nrVrajitori = 0;
}

//==============================
// LSI
// vrajitori in lista pentru dueluri / cluburi / selectii
//==============================

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct NodSimplu
{
	Vrajitor vrajitor;
	struct NodSimplu* next;
};

void afisareListaVrajitori(struct NodSimplu* lista) {
	//afiseaza toate elemente de tip vrajitor din lista simplu inlantuita
	//prin apelarea functiei afisareVrajitor()
	struct NodSimplu* current = lista;

	while (current)
	{
		afisareVrajitor(current->vrajitor);
		current = current->next;
	}
}

void adaugaVrajitorInLista(struct NodSimplu** lista, Vrajitor vrajitorNou) {
	//adauga la final in lista primita un nou vrajitor
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->vrajitor = vrajitorNou;
	nou->next = NULL;
	struct NodSimplu* current = *lista;

	if (*lista == NULL)
	{
		*lista = nou;
		return;
	}

	while (current->next)
	{
		current = current->next;
	}
	// raman pe ultimul nod!!! apoi pun pe next nou

	current->next = nou;
}

void adaugaLaInceputInLista(struct NodSimplu** lista, Vrajitor vrajitorNou) {
	//adauga la inceputul listei un nou vrajitor
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->vrajitor = vrajitorNou;
	nou->next = *lista;
	*lista = nou;
}

void* citireListaVrajitoriDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct NodSimplu* lista = NULL;

	if (file == NULL) return lista;

	while (!feof(file))
	{
		adaugaVrajitorInLista(&lista, citireVrajitorFisier(file));
	}
	fclose(file);

	return lista;
}

void dezalocareListaVrajitori(struct NodSimplu** lista) {
	//sunt dezalocati toti vrajitorii si lista de elemente
	struct NodSimplu* current = *lista;

	while (current)
	{
		struct NodSimplu* temp = current;

		current = current->next;

		free(temp->vrajitor.nume);
		free(temp->vrajitor.casa);
		free(temp);
	}
	(*lista) = NULL;
}

float calculeazaPutereMedie(struct NodSimplu* lista) {
	//calculeaza puterea magica medie a vrajitorilor din lista
	struct NodSimplu* current = lista;
	float sumaPutere = 0;
	int nr = 0;
	while (current)
	{
		nr++;
		sumaPutere += current->vrajitor.putereMagica;
		current = current->next;
	}
	if (nr)
	{
		return sumaPutere / nr;
	}
	else
	{
		return sumaPutere;
	}
}

void stergeVrajitoriDinCasa(struct NodSimplu** lista, const char* casaCautata) {
	//sterge toti vrajitorii din lista care apartin casei cautate
	//tratati situatia ca vrajitorul se afla si pe prima pozitie, si pe ultima pozitie
	struct NodSimplu* current = *lista;
	struct NodSimplu* stanga = NULL;

	while (current)
	{
		struct NodSimplu* dreapta = current->next;

		if (strcmp(casaCautata, current->vrajitor.casa) == 0)
		{

			if (stanga == NULL)
			{
				*lista = dreapta;
			}
			else
			{
				stanga->next = dreapta;
			}
			
			free(current->vrajitor.nume);
			free(current->vrajitor.casa);
			free(current);
		}
		else
		{
			stanga = current;
		}
		current = dreapta;
	}
}

float calculeazaPutereaVrajitorilorDintrOAnumitaCasa(struct NodSimplu* lista, const char* casaCautata) {
	//calculeaza puterea magica a tuturor vrajitorilor dintr-o anumita casa
	float totalPutereMagica = 0;
	struct NodSimplu* current = lista;
	while (current)
	{
		if (strcmp(casaCautata,current->vrajitor.casa) == 0)
			totalPutereMagica += current->vrajitor.putereMagica;
		current = current->next;
	}
	return totalPutereMagica;
}

//==============================
// LDI
// ordine speciala pentru ceremonia de sortare / lineup eveniment
//==============================

//creare structura pentru un nod dintr-o lista dublu inlantuita

struct Nod
{
	Vrajitor vrajitor;
	struct Nod* prev;
	struct Nod* next;
};

//creare structura pentru Lista Dubla
struct ListaDubla
{
	struct Nod* start;
	struct Nod* end;
};

void afisareLDVrajitori(struct ListaDubla lista) {
	//afiseaza toate elemente de tip vrajitor din lista dublu inlantuita
	//prin apelarea functiei afisareVrajitor()
	struct Nod* current = lista.start;

	while (current)
	{
		afisareVrajitor(current->vrajitor);
		printf("\n");
		current = current->next;
	}
}

void adaugaVrajitorInLD(struct ListaDubla* lista, Vrajitor vrajitorNou) {
	//adauga la final in lista dubla un nou vrajitor
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->vrajitor = vrajitorNou;
	nou->next = NULL;
	nou->prev = lista->end;
	if (lista->end != NULL)
		lista->end->next = nou;
	else
		lista->start = nou;
	lista->end = nou;
}

void adaugaLaInceputInLD(struct ListaDubla* lista, Vrajitor vrajitorNou) {
	//adauga la inceputul listei dublu inlantuite un nou vrajitor
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->vrajitor = vrajitorNou;
	nou->prev = NULL;
	nou->next = lista->start;
	if (lista->start != NULL)
		lista->start->prev = nou;
	else
		lista->end = nou;
	lista->start = nou;
}

struct ListaDubla* citireLDVrajitoriDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct ListaDubla* lista = malloc(sizeof(struct ListaDubla));
	lista->start = NULL;
	lista->end = NULL;
	while (!feof(file))
	{
		adaugaVrajitorInLD(lista, citireVrajitorFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareLDVrajitori(struct ListaDubla* lista) {
	//sunt dezalocati toti vrajitorii si lista dublu inlantuita de elemente
	struct Nod* current = lista->start;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;
		
		free(temp->vrajitor.nume);
		free(temp->vrajitor.casa);
		free(temp);
	}
	free(lista);
}

float calculeazaPutereMedieLD(struct ListaDubla lista) {
	//calculeaza puterea magica medie a vrajitorilor din lista dubla
	struct Nod* current = lista.start;
	float sumaPutere = 0;
	int nr = 0;
	while (current)
	{
		sumaPutere += current->vrajitor.putereMagica;
		nr++;
		current = current->next;
	}
	if (nr)
	{
		return sumaPutere / nr;
	}
	else
	{
		return sumaPutere;
	}
}

void stergeVrajitorDupaID(struct ListaDubla* lista, int idCautat) {
	//sterge vrajitorul cu id-ul primit
	//tratati situatia ca vrajitorul se afla si pe prima pozitie, si pe ultima pozitie
	
	
	
}

char* getNumeVrajitorCuPutereMaxima(struct ListaDubla lista) {
	//cauta vrajitorul cu puterea magica maxima
	//returneaza numele acestui vrajitor
	float putereMax = -1;
	struct Nod* current = lista.start;

	while (current)
	{
		if (putereMax < current->vrajitor.putereMagica)
		{
			putereMax = current->vrajitor.putereMagica;
		}
		current = current->next;
	}

	current = lista.start;
	char* nume = NULL;

	while (current)
	{
		if (putereMax == current->vrajitor.putereMagica)
		{
			nume = malloc(sizeof(char) * (strlen(current->vrajitor.nume) + 1));
			strcpy(nume, current->vrajitor.nume);

			return nume;
		}
		current = current->next;
	}
	return nume;
}

//==============================
// STACK
// vrajitori care trebuie sa refaca o proba / duel
//==============================

//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

// vom reprezenta stack prin ldi 
// avem deja NodSimplu


void pushStack(struct NodSimplu** stack, Vrajitor vrajitorNou) {
	//adauga un vrajitor in varful stivei
	// consideram varful stivei (top) = primul nod
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->vrajitor = vrajitorNou;
	nou->next = *stack;
	*stack = nou;
}

Vrajitor popStack(struct NodSimplu** stack) {
	//extrage vrajitorul din varful stivei
	Vrajitor vrajitor;
	vrajitor.id = -1;

	if (*stack == NULL)
	{
		return vrajitor;
	}

	struct NodSimplu* current = *stack;
	vrajitor = current->vrajitor;

	(*stack) = (*stack)->next;
	free(current);

	return vrajitor;
}

int emptyStack(struct NodSimplu* stack) {
	//verifica daca stiva este goala
	if (stack == NULL)
	{
		return 1;
	}
	return 0;
}

struct NodSimplu* citireStackVrajitoriDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	struct NodSimplu* stack = NULL;

	while (!feof(file))
	{
		pushStack(&stack, citireVrajitorFisier(file));
	}
	fclose(file);

	return stack;
}

void dezalocareStivaDeVrajitori(struct NodSimplu** stack) {
	//sunt dezalocati toti vrajitorii si stiva de elemente
	struct NodSimplu* current = *stack;

	while (current)
	{
		struct NodSimplu* temp = current;
		current = current->next;
		free(temp->vrajitor.nume);
		free(temp->vrajitor.casa);
		free(temp);
	}
	*stack = NULL;
}

int sizeStack(struct NodSimplu* stack) {
	//returneaza numarul de elemente din stiva
	struct NodSimplu* current = stack;
	int nr = 0;
	while (current)
	{
		nr++;
		current = current->next;
	}
	return nr;
}

//==============================
// QUEUE
// coada la Sorting Hat / intrare la examene / potiuni
//==============================

//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

// reprezentam prin LDI, avem deja nod cu prev next si vrajitor 

struct Queue
{
	struct Nod* start;
	struct Nod* end;
};

void enqueue(struct Queue* queue, Vrajitor vrajitorNou) {
	// adauga un vrajitor in coada
	// first in first out -> punem la final
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->vrajitor = vrajitorNou;
	//noul nod trebuie sa stie cine era ultimul
	nou->prev = queue->end;
	nou->next = NULL;
	// refacem legatura 
	if (queue->end != NULL)
		//vechiul ultim nod trebuie sa stie de nodul nou
		queue->end->next = nou;
	else
		queue->start = nou;
	// stiva trebuie sa stie ca nodul nou e ultimul 
	queue->end = nou;
}

Vrajitor dequeue(struct Queue* queue) {
	//extrage un vrajitor din coada
	// il scoatem pe primul din lista 

	/*
	salvez primul
	mut start
	noul start are prev = NULL
	dacă nu mai există start, atunci end = NULL
	free nodul vechi
	*/
	Vrajitor v;
	v.id = -1;

	if (queue->start == NULL)
		return v;

	struct Nod* current = queue->start;
	v = current->vrajitor;

	queue->start = queue->start->next;

	if (queue->start != NULL)
	{
		queue->start->prev = NULL;
	}
	else
	{
		queue->end = NULL;
	}

	free(current);

	return v;
}

struct Queue* citireCoadaDeVrajitoriDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL)
		return NULL;
	struct Queue* queue = malloc(sizeof(struct Queue));
	queue->start = NULL;
	queue->end = NULL;

	while (!feof(file))
	{
		enqueue(queue, citireVrajitorFisier(file));
	}
	fclose(file);

	return queue;
}

void dezalocareCoadaDeVrajitori(struct Queue* queue) {
	//sunt dezalocati toti vrajitorii si coada de elemente
	struct Nod* current = queue->start;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->vrajitor.nume);
		free(temp->vrajitor.casa);
		free(temp);
	}
	free(queue);
}

Vrajitor getVrajitorByID(struct Queue queue, int id) {
	//cauta vrajitorul dupa id
	struct Nod* current = queue.start;

	while (current)
	{
		if (id == current->vrajitor.id)
		{
			return current->vrajitor;
		}
		current = current->next;
	}
	Vrajitor v;
	v.id = -1;
	return v;
}

float calculeazaPutereTotala(struct Queue queue) {
	//calculeaza puterea magica totala
	float putereTotala = 0;
	struct Nod* current = queue.start;

	while (current)
	{
		putereTotala += current->vrajitor.putereMagica;
		current = current->next;
	}
	return putereTotala;
}

//==============================
// HASHTABLE
// cautare rapida dupa casa / id / initiala baghetei
//==============================

//creare structura pentru un nod dintr-o lista simplu inlantuita

// am deja 

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable
{
	int dim;
	struct NodSimplu** head;
};
typedef struct HashTable HashTable;

//void afisareListaDinTabela(HashTable hashTable) {
//	//afiseaza toate elementele de tip vrajitor dintr-o lista din tabela
//	//prin apelarea functiei afisareVrajitor()
//	
//}

void adaugaVrajitorInListaDinTabela(struct NodSimplu** lista, Vrajitor vrajitor) {
	//adauga la final in lista primita un nou vrajitor
	struct NodSimplu* current = *lista;
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->next = NULL;
	nou->vrajitor = vrajitor;
	
	if (*lista == NULL)
	{
		*lista = nou;
		return;
	}

	while (current->next)
	{
		current = current->next;
	}
	current->next = nou;
}

HashTable initializareHashTable(int dimensiune) {
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL
	HashTable ht;
	ht.dim = dimensiune;
	ht.head = malloc(sizeof(struct NodSimplu*) * (dimensiune));
	for (int i = 0; i < ht.dim; i++)
	{
		ht.head[i] = NULL;
	}
	return ht;
}

int calculeazaHash(const char* casa, int dimensiune) {
	//calculeaza hash-ul in functie de cheia aleasa
	int suma = 0;
	for (int i = 0; casa[i] != '\0'; i++)
	{
		suma += casa[i];
	}
	return suma % dimensiune;
}

void inserareVrajitorInTabela(HashTable ht, Vrajitor vrajitor) {
	//se determina pozitia si se realizeaza inserarea pe pozitia respectiva
	// chaining!!!
	int pozitiaListei = calculeazaHash(vrajitor.casa, ht.dim);

	//if (ht.head != NULL)
	//{
		adaugaVrajitorInLista(&ht.head[pozitiaListei], vrajitor);
		//return;
	//}
}

HashTable citireTabelaVrajitoriDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti vrajitorii din fisier
	//prin apelul repetat al functiei citireVrajitorFisier()
	//acesti vrajitori sunt inserati intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	HashTable hash = initializareHashTable(8);
	if (file == NULL) return hash;

	while (!feof(file))
	{
		inserareVrajitorInTabela(hash, citireVrajitorFisier(file));
	}
	fclose(file);
	return hash;
}

void afisareTabelaDeVrajitori(HashTable ht) {
	//sunt afisati toti vrajitorii din tabela cu evidentierea clusterelor realizate
	for (int i = 0; i < ht.dim; i++)
	{
		printf("LISTA %d\n", i + 1);
		afisareListaVrajitori(ht.head[i]);
		printf("==========================\n");
	}
}

void dezalocareTabelaDeVrajitori(HashTable* ht) {
	//sunt dezalocati toti vrajitorii din tabela de dispersie
	for (int i = 0; i < ht->dim; i++)
	{
		dezalocareListaVrajitori(&ht->head[i]);
	}
	free(ht->head);
	ht->head = NULL;
	ht->dim = 0;
}

float* calculeazaPuteriMediiPerClustere(HashTable ht, int *nrClustere) {
	//calculeaza puterea medie a vrajitorilor din fiecare cluster
	//trebuie returnat un vector cu valorile medii per cluster
	//lungimea vectorului este data de numarul de clustere care contin elemente
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.head[i] != NULL)
			(*nrClustere)++;
	}

	float* totalPuteri = malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		float totalPutere = 0;
		int nr = 0;
		struct NodSimplu* current = ht.head[i];

		while (current)
		{
			totalPutere += current->vrajitor.putereMagica;
			nr++;
			current = current->next;
		}

		if(nr)
		{
			totalPuteri[k++] = totalPutere / nr;
		}
		else
		{
			totalPuteri[k++] = totalPutere;
		}
	}
	return totalPuteri;
}

Vrajitor getVrajitorDupaCheie(HashTable ht, const char* casa) {
	//cauta vrajitorul dupa cheia folosita in calcularea hash-ului
	for (int i = 0; i < ht.dim; i++)
	{
		if (strcmp(casa, ht.head[i]->vrajitor.casa) == 0)
			return ht.head[i]->vrajitor;
	}
}

//==============================
// MAIN
//==============================

int main() {
	//==============================
	// VECTOR
	//==============================
	printf("=========== VECTOR ===========\n");
	int nrVrajitori = 0;
	Vrajitor* vector = citireVectorVrajitoriFisier("vrajitori.txt", &nrVrajitori);

	if (vector != NULL)
	{
		afisareVectorVrajitori(vector, nrVrajitori);
		printf("Numar vrajitori in vector: %d\n\n", nrVrajitori);
	}
	else
	{
		printf("Nu s-a putut deschide fisierul pentru VECTOR.\n\n");
	}

	//==============================
	// LSI
	//==============================
	printf("=========== LSI ===========\n");
	struct NodSimplu* lista = citireListaVrajitoriDinFisier("vrajitori.txt");

	if (lista != NULL)
	{
		afisareListaVrajitori(lista);
		printf("\nPutere medie LSI: %.2f\n", calculeazaPutereMedie(lista));
		printf("Putere totala Gryffindor: %.2f\n\n",
			calculeazaPutereaVrajitorilorDintrOAnumitaCasa(lista, "Gryffindor"));
	}
	else
	{
		printf("Nu s-a putut deschide fisierul pentru LSI.\n\n");
	}

	//==============================
	// LDI
	//==============================
	printf("=========== LDI ===========\n");
	struct ListaDubla* listaDubla = citireLDVrajitoriDinFisier("vrajitori.txt");

	if (listaDubla != NULL)
	{
		afisareLDVrajitori(*listaDubla);
		printf("Putere medie LDI: %.2f\n", calculeazaPutereMedieLD(*listaDubla));

		char* numeMax = getNumeVrajitorCuPutereMaxima(*listaDubla);
		if (numeMax != NULL)
		{
			printf("Vrajitor cu putere maxima: %s\n\n", numeMax);
			free(numeMax);
		}
	}
	else
	{
		printf("Nu s-a putut deschide fisierul pentru LDI.\n\n");
	}

	//==============================
	// STACK
	//==============================
	printf("=========== STACK ===========\n");
	struct NodSimplu* stack = citireStackVrajitoriDinFisier("vrajitori.txt");

	if (stack != NULL)
	{
		printf("Numar elemente in stack: %d\n", sizeStack(stack));

		Vrajitor vScos = popStack(&stack);
		if (vScos.id != -1)
		{
			printf("Vrajitor scos din stack:\n");
			afisareVrajitor(vScos);
			free(vScos.nume);
			free(vScos.casa);
		}

		printf("Numar elemente dupa pop: %d\n\n", sizeStack(stack));
	}
	else
	{
		printf("Nu s-a putut deschide fisierul pentru STACK.\n\n");
	}

	//==============================
	// QUEUE
	//==============================
	printf("=========== QUEUE ===========\n");
	struct Queue* queue = citireCoadaDeVrajitoriDinFisier("vrajitori.txt");

	if (queue != NULL)
	{
		printf("Putere totala in queue: %.2f\n", calculeazaPutereTotala(*queue));

		Vrajitor gasit = getVrajitorByID(*queue, 2);
		if (gasit.id != -1)
		{
			printf("Vrajitor gasit in queue dupa ID:\n");
			afisareVrajitor(gasit);
		}

		Vrajitor vDequeued = dequeue(queue);
		if (vDequeued.id != -1)
		{
			printf("Vrajitor scos din queue:\n");
			afisareVrajitor(vDequeued);
			free(vDequeued.nume);
			free(vDequeued.casa);
		}

		printf("\n");
	}
	else
	{
		printf("Nu s-a putut deschide fisierul pentru QUEUE.\n\n");
	}

	//==============================
	// HASH TABLE
	//==============================
	printf("=========== HASH TABLE ===========\n");
	HashTable ht = citireTabelaVrajitoriDinFisier("vrajitori.txt");
	afisareTabelaDeVrajitori(ht);

	/*int nrClustere = 0;
	float* medii = calculeazaPuteriMediiPerClustere(ht, &nrClustere);

	printf("Puteri medii per clustere ne-goale:\n");
	for (int i = 0; i < nrClustere; i++)
	{
		printf("Cluster %d -> %.2f\n", i + 1, medii[i]);
	}
	free(medii);*/

	// daca functia getVrajitorDupaCheie e corectata
	
	/*Vrajitor cautat = getVrajitorDupaCheie(ht, "Gryffindor");
	if (cautat.id != -1)
	{
		printf("Vrajitor gasit dupa cheie:\n");
		afisareVrajitor(cautat);
	}*/
	

	//==============================
	// DEZALOCARI
	//==============================
	dezalocareVectorVrajitori(&vector, &nrVrajitori);
	dezalocareListaVrajitori(&lista);
	dezalocareLDVrajitori(listaDubla);
	dezalocareStivaDeVrajitori(&stack);
	dezalocareCoadaDeVrajitori(queue);
	dezalocareTabelaDeVrajitori(&ht);

	return 0;
}