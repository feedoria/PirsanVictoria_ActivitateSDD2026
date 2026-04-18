#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul modele.txt
//sau va creati un alt fisier cu alte date

struct StructuraModel {
	int id;
	int experienta;
	float durataWalk;
	char* nume;
	char* outfit;
	unsigned char stil;
};
typedef struct StructuraModel Model;

Model citireModelDinFisier(FILE* file) {
	char linie[256];
	fgets(linie, 256, file);
	Model m;
	char sep[3] = ",\n";

	m.id = atoi(strtok(linie, sep));
	m.experienta = atoi(strtok(NULL, sep));
	m.durataWalk = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	m.nume = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(m.nume, buffer);

	buffer = strtok(NULL, sep);
	m.outfit = malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(m.outfit, buffer);

	buffer = strtok(NULL, sep);
	m.stil = buffer[0];

	return m;
}

void afisareModel(Model model) {
	printf("Id: %d\n", model.id);
	printf("Experienta: %d\n", model.experienta);
	printf("Durata walk: %.2f\n", model.durataWalk);
	printf("Nume: %s\n", model.nume);
	printf("Outfit: %s\n", model.outfit);
	printf("Stil: %c\n\n", model.stil);
}

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

// vom reprezenta stiva prin LSI 
struct NodSimplu
{
	Model model;
	struct NodSimplu* next;
};

void pushStack(/*stiva*/struct NodSimplu** stiva, Model model) {
	// -> Varianta asta e mai putin eficienta: O(n) 
	//struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	// nou->model = model;
	// nou->next = NULL;
	//// last in first out -> in lista il vom baga la urma 
	//struct NodSimplu* current = *stiva;

	//while (current)
	//{
	//	current = current->next;
	//}
	//current = nou;
	//current->next = NULL;

	//Vom lua TOP = primul element din lista 
	struct NodSimplu* current = *stiva;
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->model = model;
	nou->next = *stiva;
	*stiva = nou;
}

/// <summary>
/// Scoate elementul din varful stivei si il returneaza.
/// Daca stiva este goala, returneaza model cu id = -1.
/// </summary>
/// <param name="stiva">Pointer la stiva</param>
/// <returns>Model extras</returns>
Model popStack(/*stiva*/struct NodSimplu** stiva) {
	// last in first out -> din lista il vom returna pe ultimul -> NU mai facem asa e ineficient 
	// il scoatem tot pe primul deci, PRIMUL este considerat TOP SI ULTIMUL ADAUGAT
	Model model;
	model.id = -1;

	if (*stiva == NULL)
	{
		return model;
	}

	struct NodSimplu* current = *stiva;

	model = current->model;

	*stiva = (*stiva)->next;
	free(current);

	return model;
}

int emptyStack(/*stiva*/struct NodSimplu* stiva) {
	// 0 daca nu e empty, 1 daca e empty 
	// vedem daca e stiva e empty sau nu 
	struct NodSimplu* current = stiva;
	int nrElem = 0;

	if (stiva == NULL)
	{
		return 1;
	}

	return 0;
}

struct NodSimplu* citireStackModeleDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate modelele din fisier
	//prin apelul repetat al functiei citireModelDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct NodSimplu* stiva = NULL;
	while (!feof(file))
	{
		pushStack(&stiva, citireModelDinFisier(file));
	}
	fclose(file);

	return stiva;
}

void dezalocareStivaDeModele(/*stiva*/struct NodSimplu** stiva) {
	//sunt dezalocate toate modelele si stiva de elemente
	struct NodSimplu* current = *stiva;

	while (current)
	{
		struct NodSimplu* temp = current;
		current = current->next;

		free(temp->model.nume);
		free(temp->model.outfit);
		free(temp);
	}
}

int size(/*stiva*/struct NodSimplu* stiva) {
	//returneaza numarul de elemente din stiva
	struct NodSimplu* current = stiva;
	int nr = 0;
	while (current)
	{
		nr++;
		current = current->next;
	}
	return nr;
}

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

//vom reprezenta prin LDI 
//-> vom pune la coada listei si vom scoate de la inceput (FIFO)
struct Nod
{
	Model model;
	struct Nod* prev;
	struct Nod* next;
};

struct Queue
{
	struct Nod* start;
	struct Nod* end;
};

void enqueue(/*coada*/struct Queue* queue, Model model) {
	//adauga un model in coada
	//First In First Out

	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->model = model;
	nou->next = NULL;
	nou->prev = queue->end;

	if (queue->end != NULL)
	{
		queue->end->next = nou;
	}
	else
	{
		queue->start = nou;
	}

	queue->end = nou;
}

Model dequeue(/*coada*/struct Queue* queue) {
	//extrage un model din coada
	Model model;
	model.id = -1;

	if (queue->start == NULL)
	{
		return model;
	}

	struct Nod* current = queue->start;
	model = current->model;
	queue->start = queue->start->next;
	queue->start->prev = NULL;
	free(current);
	return model;
}

void* citireCoadaDeModeleDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate modelele din fisier
	//prin apelul repetat al functiei citireModelDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct Queue* queue = malloc(sizeof(struct Queue));
	queue->start = NULL;
	queue->end = NULL;

	while (!feof(file))
	{
		enqueue(queue, citireModelDinFisier(file));
	}
	fclose(file);

	return queue;
}

void dezalocareCoadaDeModele(/*coada*/struct Queue* queue) {
	//sunt dezalocate toate modelele si coada de elemente
	struct Nod* current = queue->start;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->model.nume);
		free(temp->model.outfit);
		free(temp);
	}
	free(queue);
}

//metode de procesare
Model getModelByID(/*stiva sau coada de modele*/struct Queue queue, int id)
{
	Model model;
	model.id = -1; 
	if (queue.start == NULL) return model;
	struct Nod* current = queue.start;

	while (current)
	{
		if (id == current->model.id)
		{
			model = current->model;
			return model;
		}
		current = current->next;
	}
}

float calculeazaDurataTotala(/*stiva sau coada de modele*/struct NodSimplu* stiva)
{
	struct NodSimplu* current = stiva;
	float durata = 0;

	while (current)
	{
		durata += current->model.durataWalk;
	}

	return durata;
}

int main() {
	

	return 0;
}