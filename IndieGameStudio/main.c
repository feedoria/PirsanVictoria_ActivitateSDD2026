#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <malloc.h>

//trebuie sa folositi fisierul developeri.txt
//sau va creati un alt fisier cu alte date

struct StructuraDeveloper {
	int id;
	int aniExperienta;
	float rating;
	char* numeDev;
	char* rol;
	char* engine;
};
typedef struct StructuraDeveloper Developer;

//==============================
// VECTOR
// toti membrii studioului indie
//==============================

void afisareDeveloper(Developer dev) {
	//afiseaza toate atributele unui developer
	printf("Id: %d\n", dev.id);
	printf("Ani experienta: %d\n", dev.aniExperienta);
	printf("Rating: %.2f\n", dev.rating);
	printf("Nume dev: %s\n", dev.numeDev);
	printf("Rol: %s\n", dev.rol);
	printf("Engine: %s\n", dev.engine);
}

void afisareVectorDeveloperi(Developer* developeri, int nrDeveloperi) {
	//afiseaza toate elemente de tip developer din vector
	//prin apelarea functiei afisareDeveloper()
	for (int i = 0; i < nrDeveloperi; i++) {
		afisareDeveloper(developeri[i]);
		printf("\n");
	}
}

void adaugaDeveloperInVector(Developer** developeri, int* nrDeveloperi, Developer developerNou) {
	//adauga in vectorul primit un nou developer
	//ATENTIE - se modifica numarul de developeri din vector
	(*nrDeveloperi)++;
	Developer* temp = malloc(sizeof(Developer) * (*nrDeveloperi));
	for (int i = 0; i < (*nrDeveloperi) - 1; i++) {
		temp[i] = (*developeri)[i];
	}

	temp[(*nrDeveloperi) - 1] = developerNou;
	free(*developeri);
	(*developeri) = temp;
}

Developer citireDeveloperDinFisier(FILE* file) {
	//functia citeste un developer dintr-un stream deja deschis
	//developerul citit este returnat
	char linie[256];
	fgets(linie, 256, file);
    char sep[3] = ";\n";

	Developer d;
	d.id = atoi(strtok(linie, sep));
	d.aniExperienta = atoi(strtok(NULL, sep));
	d.rating = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	d.numeDev = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(d.numeDev, buffer);

	buffer = strtok(NULL, sep);
	d.rol = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(d.rol, buffer);

	buffer = strtok(NULL, sep);
	d.engine = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(d.engine, buffer);

	return d;
}

Developer* citireVectorDeveloperiDinFisier(const char* numeFisier, int* nrDeveloperi) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//numarul de developeri este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE * file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	(*nrDeveloperi) = 0;
	Developer* vector = NULL;

	while (!feof(file)) {
		adaugaDeveloperInVector(&vector, nrDeveloperi, citireDeveloperDinFisier(file));
	}
	fclose(file);

	return vector;
}

void dezalocareVectorDeveloperi(Developer** vector, int* nrDeveloperi) {
	//este dezalocat intreg vectorul de developeri
	for (int i = 0; i < (*nrDeveloperi); i++) {
		free((*vector)[i].numeDev);
		free((*vector)[i].rol);
		free((*vector)[i].engine);
	}
	free(*vector);
	*vector = NULL;
	*nrDeveloperi = 0;
}

//==============================
// LSI
// developeri inscrisi la task-uri / sprint-uri
//==============================

//creare structura pentru un nod dintr-o lista simplu inlantuita

struct Nod {
	Developer developer;
	struct Nod* next;
};

void afisareListaDeveloperi(/*lista de developeri*/struct Nod* lista) {
	//afiseaza toate elemente de tip developer din lista simplu inlantuita
	//prin apelarea functiei afisareDeveloper()
	struct Nod* current = lista;
	while (current) {
		afisareDeveloper(current->developer);
		current = current->next;
	}
}

void adaugaDeveloperInLista(/*lista de developeri*/struct Nod** lista, Developer developerNou) {
	//adauga la final in lista primita un nou developer
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->developer = developerNou;
	nou->next = NULL;

	if (*lista == NULL) {
		*lista = nou;
		return;
	}

	struct Nod* current = *lista;

	while (current->next) {
		current = current->next;
	}
	current->next = nou;
}

void adaugaLaInceputInLista(/*lista de developeri*/struct Nod** lista, Developer developerNou) {
	//adauga la inceputul listei un nou developer
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->developer = developerNou;
	nou->next = *lista;
	*lista = nou;
}

struct Nod* citireListaDeveloperiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	struct Nod* lista = NULL;

	while (!feof(file)) {
		Developer developer = citireDeveloperDinFisier(file);
		adaugaDeveloperInLista(&lista, developer);
	}
	fclose(file);

	return lista;
}

void dezalocareListaDeveloperi(/*lista de developeri*/struct Nod** lista) {
	//sunt dezalocati toti developerii si lista de elemente
	struct Nod* current = *lista;

	while (current) {
		struct Nod* temp = current;
		current = current->next;

		free(temp->developer.numeDev);
		free(temp->developer.rol); // aici eliberam continul
		free(temp->developer.engine);
		free(temp); // aici eliberam nodurile
	}
	//free(*lista);
	*lista = NULL; // setam la NULL lista
}

float calculeazaRatingMediu(/*lista de developeri*/struct Nod* lista) {
	//calculeaza ratingul mediu al developerilor din lista
	float suma = 0;
	int nr = 0;
	struct Nod* current = lista;

	while (current) {
		suma += current->developer.rating;
		nr++;
		current = current->next;
	}

	if (nr) {
		return suma/nr;
	}
	else {
		return suma;
	}
}

void stergeDeveloperiDupaEngine(/*lista developeri*/struct Nod** lista, const char* engineCautat) {
	//sterge toti developerii din lista care folosesc engine-ul primit ca parametru
	//tratati situatia ca developerul se afla si pe prima pozitie, si pe ultima pozitie
	struct Nod* current = *lista;
	struct Nod* stanga = NULL;

	while (current) {
		struct Nod* dreapta = current->next;
		if (strcmp(engineCautat, current->developer.engine) == 0) {
			if (stanga != NULL) {
				stanga->next = dreapta;
			}
			else {
				*lista = dreapta;
			}

			free(current->developer.numeDev);
			free(current->developer.rol);
			free(current->developer.engine);
			free(current);
		}
		else {
			stanga = current;
		}
		current = dreapta;
	}
}

float calculeazaRatingulDeveloperilorDintrUnEngine(/*lista developeri*/struct Nod* lista, const char* engineCautat) {
	//calculeaza ratingul total al developerilor care folosesc engine-ul primit ca parametru
	float totalRating = 0;
	struct Nod* current = lista;

	while (current) {
		if (strcmp(engineCautat, current->developer.engine) == 0)
			totalRating += current->developer.rating;
		current	= current->next;
	}
	return totalRating;
}

//==============================
// LDI
// ordinea pentru prezentari / review intern / lineup de proiect
//==============================

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct NodLDI {
	Developer developer;
	struct NodLDI* prev;
	struct NodLDI* next;
};
//creare structura pentru Lista Dubla
struct ListaDubla {
	struct NodLDI* start;
	struct NodLDI* end;
};

void afisareLDDeveloperi(/*lista dubla de developeri*/struct ListaDubla lista) {
	//afiseaza toate elemente de tip developer din lista dublu inlantuita
	//prin apelarea functiei afisareDeveloper()
	struct NodLDI* current = lista.start;

	while (current) {
		afisareDeveloper(current->developer);
		current = current->next;
	}
}

void adaugaDeveloperInLD(/*lista dubla de developeri*/ struct ListaDubla* lista,Developer developerNou) {
	//adauga la final in lista dubla un nou developer
	struct NodLDI* nou = malloc(sizeof(struct NodLDI));
	nou->developer = developerNou;
	nou->prev = lista->end;
	nou->next = NULL;

	if (lista->start == NULL) {
		lista->start = nou;
	}
	else {
		lista->end->next = nou;
	}
	lista->end = nou;
}

void adaugaLaInceputInLD(/*lista dubla de developeri*/struct ListaDubla* lista, Developer developerNou) {
	//adauga la inceputul listei dublu inlantuite un nou developer
	struct NodLDI* nou = malloc(sizeof(struct NodLDI));
	nou->developer = developerNou;
	nou->prev = NULL;
	nou->next = lista->start;

	if (lista->end == NULL) {
		lista->end = nou;
	}
	else {
		lista->start->prev = nou;
	}
	lista->start = nou;
}

struct ListaDubla* citireLDDeveloperiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	struct ListaDubla* lista = malloc(sizeof(struct ListaDubla));
	lista->start = NULL;
	lista->end = NULL;

	while (!feof(file)) {
		Developer dev = citireDeveloperDinFisier(file);
		if (dev.id != -1)
			adaugaDeveloperInLD(lista, dev);
	}
	fclose(file);

	return lista;
}

void dezalocareLDDeveloperi(/*lista dubla de developeri*/struct ListaDubla* lista) {
	//sunt dezalocati toti developerii si lista dublu inlantuita de elemente
	struct NodLDI* current = lista->start;

	while (current) {
		struct NodLDI* temp = current;

		current = current->next;

		free(temp->developer.numeDev);
		free(temp->developer.rol);
		free(temp->developer.engine);
		free(temp);
	}
	lista->start = NULL;
	lista->end = NULL;
	free(lista);
}

float calculeazaRatingMediuLD(/*lista de developeri*/struct ListaDubla lista) {
	//calculeaza ratingul mediu al developerilor din lista dubla
	struct NodLDI * current = lista.start;
	float totalRating = 0;
	int nr = 0;

	while (current) {
		nr++;
		totalRating += current->developer.rating;
		current = current->next;
	}
	if (nr) {
		return totalRating/nr;
	}
	else {
		return totalRating;
	}
}

void stergeDeveloperDupaID(/*lista developeri*/struct ListaDubla* lista, int id) {
	//sterge developerul cu id-ul primit
	//tratati situatia ca developerul se afla si pe prima pozitie, si pe ultima pozitie
	struct NodLDI* current = lista->start;

	while (current)
	{
		struct NodLDI* stanga = current->prev;
		struct NodLDI* dreapta = current->next;

		if (id == current->developer.id)
		{
			if (stanga != NULL)
				stanga->next = dreapta;
			else
				lista->start = dreapta;

			if (dreapta != NULL)
				dreapta->prev = stanga;
			else
				lista->end = stanga;

			free(current->developer.numeDev);
			free(current->developer.rol);
			free(current->developer.engine);
			free(current);

			current = dreapta;
		}
		else
		{
			current = current->next;
		}
	}
}

char* getNumeDeveloperCuRatingMaxim(/*lista dublu inlantuita*/) {
	//cauta developerul cu ratingul maxim
	//returneaza numele acestui developer
	return NULL;
}

//==============================
// STACK
// developeri care refac un task / bugfix urgent / retry code review
//==============================

//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

// vom reprezenta prin LSI

void pushStack(/*stiva*/struct Nod** stack, Developer developerNou) {
	//adauga un developer in varful stivei
	// last in first out LIFO
	// consideram varful stackului = primul elem din lsi
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->developer = developerNou;
	nou->next = *stack;
	*stack = nou;
}

Developer popStack(/*stiva*/struct Nod** stack) {
	//extrage developerul din varful stivei
	Developer d;
	d.id = -1;

	if (*stack == NULL) {
		return d;
	}

	struct Nod* current = *stack;

	d = current->developer;
	*stack = (*stack)->next;

	free(current);

	return d;
}

// 1 daca e empty; 0 daca nu e
int emptyStack(/*stiva*/struct Nod** stack) {
	//verifica daca stiva este goala
	return *stack == NULL;
}

void* citireStackDeveloperiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r");
	struct Nod* stack = NULL;

	while (!feof(file)) {
		Developer d = citireDeveloperDinFisier(file);
		if (d.id != -1)
			pushStack(&stack, d);
	}
	fclose(file);

	return stack;
}

void dezalocareStivaDeDeveloperi(/*stiva*/struct Nod** stack) {
	//sunt dezalocati toti developerii si stiva de elemente
	struct Nod* current = *stack;

	while (current) {
		struct Nod* temp = current;
		current = current->next;
		free(temp->developer.numeDev);
		free(temp->developer.rol);
		free(temp->developer.engine);
		free(temp);
	}
	*stack = NULL;
}

int sizeStack(/*stiva*/struct Nod* stack) {
	//returneaza numarul de elemente din stiva
	struct Nod* current = stack;
	int nr = 0;
	while (current) {
		nr++;
		current = current->next;
	}
	return nr;
}

//==============================
// QUEUE
// coada pentru meeting / build machine / QA testing
//==============================

//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

//ldi
struct Queue {
	struct NodLDI* start;
	struct NodLDI* end;
};

void enqueue(/*coada*/struct Queue* queue, Developer developerNou) {
	//adauga un developer in coada
	// first in first out
	// deci punem la final
	struct NodLDI* nou = malloc(sizeof(struct NodLDI));
	nou->developer = developerNou;
	nou->next = NULL;
	nou->prev = queue->end;

	if (queue->start != NULL) {
		queue->end->next = nou;
	}
	else {
		queue->start = nou;
	}
	queue->end = nou;
}

Developer dequeue(/*coada*/struct Queue* queue) {
	//extrage un developer din coada
	// -> deci il scoatem pe primul din lista
	Developer d;
	d.id = -1;

	if (queue->start == NULL) return d;

	struct NodLDI* current = queue->start;

	d = queue->start->developer;

	queue->start = queue->start->next;

	if (queue->start != NULL) {

		queue->start->prev = NULL;
	}
	else {
		queue->end = NULL;
	}

	free(current);

	return d;
}

struct Queue* citireCoadaDeDeveloperiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return  NULL;
	struct Queue* queue = malloc(sizeof(struct Queue));
	queue->start = NULL;
	queue->end = NULL;

	while (!feof(file)) {
		Developer d = citireDeveloperDinFisier(file);
		if (d.id != -1)
		enqueue(queue, d);
	}
	fclose(file);

	return queue;
}

void dezalocareCoadaDeDeveloperi(/*coada*/struct Queue* queue) {
	//sunt dezalocati toti developerii si coada de elemente
	struct NodLDI* current = queue->start;

	while (current) {
		struct NodLDI* temp = current;
		current = current->next;
		free(temp->developer.engine);
		free(temp->developer.rol);
		free(temp->developer.numeDev);
		free(temp);
	}
	free(queue);
}

Developer getDeveloperByID(/*stiva sau coada de developeri*/struct Nod* lista, int id) {
	//cauta developerul dupa id
	struct Nod* current = lista;
	Developer d;
	d.id = -1;
	while (current) {
		if (id == current->developer.id)
			return current->developer;
		current = current->next;
	}
	return d;
}

float calculeazaRatingTotal(/*stiva sau coada de developeri*/struct Nod* lista) {
	//calculeaza ratingul total al developerilor
	float totalRating = 0;
	struct Nod* current = lista;

	while (current) {
		totalRating += current->developer.rating;
		current = current->next;
	}
	return totalRating;
}

//==============================
// HASHTABLE
// cautare rapida dupa engine
//==============================

//creare structura pentru un nod dintr-o lista simplu inlantuita

// avem deja Nod pentru LSI

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	struct Nod** head;
};
typedef struct HashTable HashTable;

void afisareListaDinTabela(/*lista de developeri*/struct Nod* lista) {
	//afiseaza toate elementele de tip developer dintr-o lista din tabela
	//prin apelarea functiei afisareDeveloper()
	struct Nod* current = lista;

	while (current) {
		afisareDeveloper(current->developer);
		current = current->next;
	}
}

void adaugaDeveloperInListaDinTabela(/*lista de developeri*/struct Nod** lista, Developer developerNou) {
	//adauga la final in lista primita un nou developer
	struct Nod* nou = malloc(sizeof(struct Nod));
	struct Nod* current = *lista;
	nou->developer = developerNou;
	nou->next = NULL;

	if (*lista == NULL) {
		*lista = nou;
		return;
	}

	while (current->next) {
		current = current->next;
	}
	current->next = nou;
}

HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.head = malloc(sizeof(struct Nod*) * dimensiune);
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL
	for (int i = 0; i < ht.dim; i++) {
		ht.head[i] = NULL;
	}
	return ht;
}

int calculeazaHash(/*cheie pentru clusterizare*/const char* engine, int dimensiune) {
	//este calculat hash-ul in functie de dimensiunea tabelei si cheia aleasa
	int suma = 0;
	for (int i = 0; engine[i] != '\0'; i++) {
		suma += engine[i];
	}
	return suma % dimensiune;
}

void inserareDeveloperInTabela(HashTable hash, Developer developerNou) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int pozitiaListeiInHashTable = calculeazaHash(developerNou.engine, hash.dim);
	adaugaDeveloperInListaDinTabela(&hash.head[pozitiaListeiInHashTable], developerNou);
}

HashTable citireTabelaDeveloperiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toti developerii din fisier
	//prin apelul repetat al functiei citireDeveloperDinFisier()
	//acesti developeri sunt inserati intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");

	HashTable hashTable = initializareHashTable(15);
	if (file == NULL) return hashTable;

	while (!feof(file)) {
		Developer d = citireDeveloperDinFisier(file);
		if (d.id != -1)
		inserareDeveloperInTabela(hashTable, d);
	}
	fclose(file);

	return hashTable;
}

void afisareTabelaDeDeveloperi(HashTable ht) {
	// afisare cu evidentierea clusterelor
	for (int i = 0; i < ht.dim; i++) {
		printf("Cluster %d:\n", i + 1);

		if (ht.head[i] != NULL) {
			printf("Cheie cluster: %s\n", ht.head[i]->developer.engine);
			afisareListaDeveloperi(ht.head[i]);
		}

		printf("==================\n");
	}
}

void dezalocareTabelaDeDeveloperi(HashTable* ht) {
	//sunt dezalocati toti developerii din tabela de dispersie
	for (int i = 0; i < ht->dim; i ++) {
		struct Nod* current = ht->head[i];

		while (current) {
			struct Nod* temp = current;
			current = current->next;

			free(temp->developer.numeDev);
			free(temp->developer.rol);
			free(temp->developer.engine);
			free(temp);
		}
		ht->head[i] = NULL;
	}
	free(ht->head);
	ht->head = NULL;
	ht->dim = 0;
}

float* calculeazaRatinguriMediiPerClustere(HashTable ht, int* nrClustere) {
	*nrClustere = 0;

	for (int i = 0; i < ht.dim; i++) {
		if (ht.head[i] != NULL)
			(*nrClustere)++;
	}

	float* ratinguriMedii = malloc(sizeof(float) * (*nrClustere));
	int k = 0;

	for (int i = 0; i < ht.dim; i++) {
		struct Nod* current = ht.head[i];

		if (current != NULL) {
			float suma = 0;
			int nr = 0;

			while (current) {
				suma += current->developer.rating;
				nr++;
				current = current->next;
			}

			ratinguriMedii[k++] = suma / nr;
		}
	}

	return ratinguriMedii;
}

Developer getDeveloperDupaCheie(HashTable ht, const char* engineCautat /*valoarea pentru developerul cautat*/) {
	Developer d;
	d.id = -1;

	int pozitie = calculeazaHash(engineCautat, ht.dim);

	struct Nod* current = ht.head[pozitie];

	while (current) {
		if (strcmp(current->developer.engine, engineCautat) == 0) {
			return current->developer;
		}
		current = current->next;
	}

	return d;
}

//==============================
// MAIN
//==============================

int main() {

	//==============================
	// VECTOR
	//==============================
	printf("=========== VECTOR ===========\n");
	int nrDeveloperi = 0;
	Developer* vector = citireVectorDeveloperiDinFisier("developeri.txt", &nrDeveloperi);

	if (vector != NULL) {
		afisareVectorDeveloperi(vector, nrDeveloperi);
		printf("Numar developeri in vector: %d\n\n", nrDeveloperi);
	}
	else {
		printf("Nu s-a putut deschide fisierul pentru VECTOR.\n\n");
	}

	//==============================
	// LSI
	//==============================
	printf("=========== LSI ===========\n");
	struct Nod* lista = citireListaDeveloperiDinFisier("developeri.txt");

	if (lista != NULL) {
		afisareListaDeveloperi(lista);
		printf("\nRating mediu LSI: %.2f\n", calculeazaRatingMediu(lista));
		printf("Rating total pentru Unity: %.2f\n\n",
			calculeazaRatingulDeveloperilorDintrUnEngine(lista, "Unity"));
	}
	else {
		printf("Nu s-a putut deschide fisierul pentru LSI.\n\n");
	}

	//==============================
	// LDI
	//==============================
	printf("=========== LDI ===========\n");
	struct ListaDubla* listaDubla = citireLDDeveloperiDinFisier("developeri.txt");

	if (listaDubla != NULL) {
		afisareLDDeveloperi(*listaDubla);
		printf("Rating mediu LDI: %.2f\n", calculeazaRatingMediuLD(*listaDubla));

		char* numeMax = getNumeDeveloperCuRatingMaxim(*listaDubla);
		if (numeMax != NULL) {
			printf("Developer cu rating maxim: %s\n\n", numeMax);
			free(numeMax);
		}
	}
	else {
		printf("Nu s-a putut deschide fisierul pentru LDI.\n\n");
	}

	//==============================
	// STACK
	//==============================
	printf("=========== STACK ===========\n");
	struct Nod* stack = citireStackDeveloperiDinFisier("developeri.txt");

	if (stack != NULL) {
		printf("Numar elemente in stack: %d\n", sizeStack(stack));

		Developer dScos = popStack(&stack);
		if (dScos.id != -1) {
			printf("Developer scos din stack:\n");
			afisareDeveloper(dScos);

			free(dScos.numeDev);
			free(dScos.rol);
			free(dScos.engine);
		}

		printf("Numar elemente dupa pop: %d\n\n", sizeStack(stack));
	}
	else {
		printf("Nu s-a putut deschide fisierul pentru STACK.\n\n");
	}

	//==============================
	// QUEUE
	//==============================
	printf("=========== QUEUE ===========\n");
	struct Queue* queue = citireCoadaDeDeveloperiDinFisier("developeri.txt");

	if (queue != NULL) {
		printf("Rating total in queue: %.2f\n", calculeazaRatingTotal(queue->start));

		Developer gasit = getDeveloperByID(queue->start, 3);
		if (gasit.id != -1) {
			printf("Developer gasit in queue dupa ID:\n");
			afisareDeveloper(gasit);
		}

		Developer dDequeued = dequeue(queue);
		if (dDequeued.id != -1) {
			printf("Developer scos din queue:\n");
			afisareDeveloper(dDequeued);

			free(dDequeued.numeDev);
			free(dDequeued.rol);
			free(dDequeued.engine);
		}

		printf("\n");
	}
	else {
		printf("Nu s-a putut deschide fisierul pentru QUEUE.\n\n");
	}

	//==============================
	// HASH TABLE
	//==============================
	printf("=========== HASH TABLE ===========\n");
	HashTable ht = citireTabelaDeveloperiDinFisier("developeri.txt");
	afisareTabelaDeDeveloperi(ht);

	int nrClustere = 0;
	float* medii = calculeazaRatinguriMediiPerClustere(ht, &nrClustere);

	printf("Ratinguri medii per clustere ne-goale:\n");
	for (int i = 0; i < nrClustere; i++) {
		printf("Cluster %d -> %.2f\n", i + 1, medii[i]);
	}
	free(medii);

	Developer cautat = getDeveloperDupaCheie(ht, "Unity");
	if (cautat.id != -1) {
		printf("\nDeveloper gasit dupa cheia Unity:\n");
		afisareDeveloper(cautat);
	}

	//==============================
	// DEZALOCARI
	//==============================
	dezalocareVectorDeveloperi(&vector, &nrDeveloperi);
	dezalocareListaDeveloperi(&lista);
	dezalocareLDDeveloperi(listaDubla);
	dezalocareStivaDeDeveloperi(&stack);
	dezalocareCoadaDeDeveloperi(queue);
	dezalocareTabelaDeDeveloperi(&ht);

	return 0;
}
