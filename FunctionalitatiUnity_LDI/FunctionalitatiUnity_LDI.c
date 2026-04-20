#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul functionalitati.txt
//sau va creati un alt fisier cu alte date

struct StructuraFunctionalitate {
	int id;
	int complexitate;
	float timpImplementare;
	char* nume;
	char* descriere;
	unsigned char tip;
};
typedef struct StructuraFunctionalitate Functionalitate;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Functionalitate functionalitate;
	struct Nod* next;
	struct Nod* prev;
};

//creare structura pentru Lista Dubla
struct ListaDubla {
	struct Nod* prim;
	struct Nod* ultim;
};
typedef struct ListaDubla ListaDubla;

Functionalitate citireFunctionalitateDinFisier(FILE* file) {
	char buffer[256];
	char sep[3] = ",\n";
	fgets(buffer, 256, file);
	char* aux;
	Functionalitate f1;
	aux = strtok(buffer, sep);
	f1.id = atoi(aux);
	f1.complexitate = atoi(strtok(NULL, sep));
	f1.timpImplementare = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	f1.nume = malloc(strlen(aux) + 1);
	strcpy_s(f1.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	f1.descriere = malloc(strlen(aux) + 1);
	strcpy_s(f1.descriere, strlen(aux) + 1, aux);

	f1.tip = *strtok(NULL, sep);
	return f1;
}

void afisareFunctionalitate(Functionalitate functionalitate) {
	printf("Id: %d\n", functionalitate.id);
	printf("Complexitate: %d\n", functionalitate.complexitate);
	printf("Timp implementare: %.2f\n", functionalitate.timpImplementare);
	printf("Nume: %s\n", functionalitate.nume);
	printf("Descriere: %s\n", functionalitate.descriere);
	printf("Tip: %c\n\n", functionalitate.tip);
}

void afisareListaFunctionalitati(/*lista dubla de functionalitati*/ListaDubla lista) {
	//afiseaza toate elemente de tip functionalitate din lista dublu inlantuita
	//prin apelarea functiei afisareFunctionalitate()
	struct Nod* current = lista.prim;

	while (current)
	{
		afisareFunctionalitate(current->functionalitate);
		current = current->next;
	}
}

void adaugaFunctionalitateInLista(/*lista dubla de functionalitati*/ListaDubla* lista, Functionalitate functionalitateNoua) {
	//adauga la final in lista primita o noua functionalitate pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	struct Nod* current = lista->ultim;
	nou->functionalitate = functionalitateNoua;
	nou->prev = current;
	nou->next = NULL;
	
	if (lista == NULL)
	{
		lista->prim = nou;
		lista->ultim = nou;
	}
	else
	{
		lista->ultim->next = nou;
		lista->ultim = nou;
	}
}

void adaugaLaInceputInLista(/*lista dubla de functionalitati*/ListaDubla* lista, Functionalitate functionalitateNoua) {
	//adauga la inceputul listei dublu inlantuite o noua functionalitate pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->functionalitate = functionalitateNoua;
	nou->prev = NULL;
	nou->next = lista->prim;

	if (lista->prim == NULL)
	{
		lista->prim = nou;
		lista->ultim = nou;
	}
	else
	{
		lista->prim->prev = nou;
		lista->prim = nou;
	}
}

ListaDubla citireLDFunctionalitatiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate functionalitatile din fisier
	//prin apelul repetat al functiei citireFunctionalitateDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	ListaDubla lista;
	lista.prim = NULL;
	lista.ultim = NULL;
	
	while (!feof(file))
	{
		Functionalitate functionalitate = citireFunctionalitateDinFisier(file);
		adaugaFunctionalitateInLista(&lista, functionalitate);
	}
	fclose(file);

	return lista;
}

void dezalocareLDFunctionalitati(/*lista dubla de functionalitati*/ListaDubla* lista) {
	//sunt dezalocate toate functionalitatile si lista dublu inlantuita de elemente
	struct Nod* current = lista->prim;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->functionalitate.nume);
		free(temp->functionalitate.descriere);
		free(temp);
	}

	lista->prim = NULL;
	lista->ultim = NULL;
}

float calculeazaTimpMediuImplementare(/*lista de functionalitati*/ListaDubla lista) {
	//calculeaza timpul mediu de implementare al functionalitatilor din lista.
	struct Nod* current = lista.prim;
	float sumaTimp = 0;
	int nr = 0;

	while (current)
	{
		sumaTimp += current->functionalitate.timpImplementare;
		nr++;
		current = current->next;
	}

	if (nr)
	{
		return sumaTimp / nr;
	}
	else
	{
		return sumaTimp;
	}
}

void stergeFunctionalitateDupaID(/*lista functionalitati*/ListaDubla* lista, int id) {
	//sterge functionalitatea cu id-ul primit.
	//tratati situatia ca functionalitatea se afla si pe prima pozitie, si pe ultima pozitie

	// deci daca idul corespunde cu primul element => trebuie sters primul element
	// asta inseamna ca:
	// prev ul lu al doilea e null
	// si prim e vechiul prim 

	if (lista == NULL || lista->prim == NULL) return;

	struct Nod* current = lista->prim;

	while (current != NULL && current->functionalitate.id != id) {
		current = current->next;
	}

	if (current == NULL) return;

	if (current->prev != NULL)
		current->prev->next = current->next;
	else
		lista->prim = current->next;

	if (current->next != NULL)
		current->next->prev = current->prev;
	else
		lista->ultim = current->prev;

	free(current->functionalitate.nume);
	free(current->functionalitate.descriere);
	free(current);
}

char* getNumeFunctionalitateCeaMaiComplexa(/*lista dublu inlantuita*/ListaDubla* lista) {
	//cauta functionalitatea cea mai complexa si
	//returneaza numele acestei functionalitati.
	int maxim = -1;
	struct Nod* current = lista->prim;
	int ok = 0;
	char* nume = NULL;
	while (current)
	{
		if (maxim < current->functionalitate.complexitate)
		{
			maxim = current->functionalitate.complexitate;

			if (ok == 1) {
				free(nume);
			}

			nume = (char*)malloc(strlen(current->functionalitate.nume) + 1);
			strcpy(nume, current->functionalitate.nume);

			ok = 1;
		}
		current = current->next;
	}

	return nume;
}

int main() {
	ListaDubla lista = citireLDFunctionalitatiDinFisier("mecanici.txt");

	afisareListaFunctionalitati(lista);

	printf("Timp mediu: %.2f\n", calculeazaTimpMediuImplementare(lista));

	char* nume = getNumeFunctionalitateCeaMaiComplexa(&lista);
	if (nume != NULL) {
		printf("Cea mai complexa functionalitate: %s\n", nume);
		free(nume);
	}

	dezalocareLDFunctionalitati(&lista);

	return 0;
}