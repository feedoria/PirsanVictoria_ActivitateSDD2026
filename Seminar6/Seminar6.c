#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

/*
BRIEF TEORETIC

STACK:
LIFO = Last In First Out

Operații de bază
push → adaugă element în vârf
pop → scoate element din vârf
top → vezi elementul din vârf (fără să-l scoți)
empty → verifică dacă e goală

QUEUE:
FIFO = First In First Out

Operații de bază
enqueue → adaugă la final
dequeue → scoate de la început
front → vezi primul element
*/

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

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

//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

struct NodSimplu
{
	Masina masina;
	struct NodSimplu* next;
};

void pushStack(/*stiva*/struct NodSimplu** stack, Masina masina) {
	struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
	nou->masina = masina;
	nou->next = *stack;

	*stack = nou;
	// top ul stackului este inceputul listei
}

Masina popStack(/*stiva*/struct NodSimplu** stack) {
	Masina masina;
	masina.id = -1;

	if (*stack == NULL)
	{
		return masina;
	}

	struct NodSimplu* ultimulNod = *stack;
	masina = ultimulNod->masina;
	*stack = (*stack)->next;
	free(ultimulNod);

	return masina;
}

int emptyStack(/*stiva*/struct NodSimplu* stack) {
	return !stack;
}

void* citireStackMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct NodSimplu* stack = NULL;

	while (!feof(file))
	{
		Masina masina = citireMasinaDinFisier(file);
		pushStack(&stack, masina);
	}
	fclose(file);

	return stack;
}

void dezalocareStivaDeMasini(/*stiva*/struct NodSimplu** stack) {
	//sunt dezalocate toate masinile si stiva de elemente
	struct NodSimplu* current = *stack;

	while (current)
	{
		struct NodSimplu* temp = current;

		current = current->next;

		free(temp->masina.numeSofer);
		free(temp->masina.model);
		free(temp);
	}

	*stack = NULL;
}

int size(/*stiva*/struct NodSimplu* stack) {
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

//QUEUE
//Alegeti prin ce veti reprezenta coada si creati structura necesara acestei cozi
//putem reprezenta o coada prin LSI, LDI sau vector

struct Nod
{
	Masina masina;
	struct Nod* prev;
	struct Nod* next;
};

struct Queue
{
	struct Nod* start;
	struct Nod* end;
};

void enqueue(struct Queue* queue, Masina masina) {
	// adauga o masina in coada
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->masina = masina;
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

Masina dequeue(/*coada*/struct Queue* queue) {
	//extrage o masina din coada
	Masina masina;
	masina.id = -1;

	//verificam daca lista e goala si daca e returnam val default
	if (queue->start == NULL)
	{
		return masina;
	}

	//daca nu e goala retinem startul si masina din el si verificam daca asta e singurul nod si refacem
	//legatura apoi eliberam memoria nodului pe care l am scos din coada si returnam masina scoasa 
	struct Nod* nod = queue->start;

	masina = nod->masina;

	queue->start = queue->start->next;

	if (queue->start == NULL)
	{
		queue->end = NULL;
	}

	free(nod);

	return masina;
}

struct Queue* citireCoadaDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct Queue* queue = (struct Queue*)malloc(sizeof(struct Queue));
	queue->start = NULL;
	queue->end = NULL;

	while (!feof(file))
	{
		enqueue(queue, citireMasinaDinFisier(file));
	}
	fclose(file);

	return queue;
}

void dezalocareCoadaDeMasini(/*coada*/struct Queue* queue) {
	//sunt dezalocate toate masinile si coada de elemente
	struct Nod* current = queue->start;

	while (current != NULL)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->masina.model);
		free(temp->masina.numeSofer);
		free(temp);
	}

	queue->start = NULL;
	queue->end = NULL;
}


//metode de procesare

float calculeazaPretTotal(struct NodSimplu* stack)
{
	struct NodSimplu* current = stack;

	float suma = 0;

	while (current)
	{
		suma += current->masina.pret;
		current = current->next;
	}

	return suma;
}

Masina getMasinaByID(struct Queue queue, int id)
{
	Masina masina;
	masina.id = -1;

	if (queue.start == NULL) return masina;

	struct Nod* current = queue.start;
	masina = current->masina;

	while (current != NULL)
	{
		if (id == current->masina.id)
		{
			//masina = dequeue(&queue, current->masina);
			masina = current->masina;
		}
		current = current->next;
	}

	return masina;
}

/*
REGULA INVATATA 
queue        = Queue*
&queue       = Queue**
*/

int main() {
	struct Queue* queue = citireCoadaDeMasiniDinFisier("masini.txt");

	if (queue == NULL)
	{
		printf("Fisierul nu a putut fi deschis.\n");
		return 0;
	}

	// afisare coada initiala
	printf("COADA INITIALA:\n");
	struct Nod* current = queue->start;

	while (current != NULL)
	{
		afisareMasina(current->masina);
		current = current->next;
	}

	// test getMasinaByID
	Masina masinaGasita = getMasinaByID(*queue, 2);
	if (masinaGasita.id != -1)
	{
		printf("\nMasina gasita dupa ID:\n");
		afisareMasina(masinaGasita);
	}
	else
	{
		printf("\nNu exista masina cu acest ID.\n");
	}

	// test dequeue
	Masina masinaScoasa = dequeue(queue);
	if (masinaScoasa.id != -1)
	{
		printf("\nMasina scoasa din coada:\n");
		afisareMasina(masinaScoasa);
	}

	// afisare dupa dequeue
	printf("\nCOADA DUPA DEQUEUE:\n");
	current = queue->start;

	while (current != NULL)
	{
		afisareMasina(current->masina);
		current = current->next;
	}

	Masina m;
	m.id = 100;
	m.nrUsi = 4;
	m.pret = 5000;

	m.model = malloc(strlen("BMW") + 1);
	strcpy(m.model, "BMW");

	m.numeSofer = malloc(strlen("Ion") + 1);
	strcpy(m.numeSofer, "Ion");

	m.serie = 'A';

	enqueue(queue, m);

	// afisare dupa enqueue
	printf("\nCOADA DUPA ENQUEUE:\n");
	current = queue->start;

	while (current != NULL)
	{
		afisareMasina(current->masina);
		current = current->next;
	}

	dezalocareCoadaDeMasini(queue);
	free(queue);
	return 0;
}