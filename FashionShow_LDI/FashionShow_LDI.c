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

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod
{
	Model model;
	struct Nod* prev;
	struct Nod* next;
};

//creare structura pentru Lista Dubla
struct ListaDubla
{
	struct Nod* start;
	struct Nod* end;
};

Model citireModelDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Model m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.experienta = atoi(strtok(NULL, sep));
	m1.durataWalk = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	m1.nume = malloc(strlen(aux) + 1);
	strcpy_s(m1.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.outfit = malloc(strlen(aux) + 1);
	strcpy_s(m1.outfit, strlen(aux) + 1, aux);

	m1.stil = *strtok(NULL, sep);
	return m1;
}

void afisareModel(Model model) {
	printf("Id: %d\n", model.id);
	printf("Experienta: %d\n", model.experienta);
	printf("Durata walk: %.2f\n", model.durataWalk);
	printf("Nume: %s\n", model.nume);
	printf("Outfit: %s\n", model.outfit);
	printf("Stil: %c\n\n", model.stil);
}

void afisareListaModele(/*lista dubla de modele*/struct ListaDubla modele) {
	//afiseaza toate elemente de tip model din lista dublu inlantuita
	//prin apelarea functiei afisareModel()
	struct Nod* current = modele.start;

	while (current)
	{
		afisareModel(current->model);
		current = current->next;
	}
}

void adaugaModelInLista(struct ListaDubla* lista, Model modelNou) {
	// adauga la final 
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->model = modelNou;
	nou->prev = lista->end;
	nou->next = NULL;

	if (lista->end != NULL)
	{
		lista->end->next = nou;
	}
	else
	{
		lista->start = nou;
	}

	lista->end = nou;
}

void adaugaLaInceputInLista(/*lista dubla de modele*/struct ListaDubla* lista, Model modelNou) {
	//adauga la inceputul listei dublu inlantuite un nou model pe care il primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	struct Nod* current = lista->start;
	nou->model = modelNou;
	nou->prev = NULL;
	nou->next = lista->start;

	if (lista->start != NULL)
	{
		lista->start->prev = nou;
	}
	else
	{
		lista->end = nou;
	}

	lista->start = nou;
}

struct ListaDubla* citireLDModeleDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate modelele din fisier
	//prin apelul repetat al functiei citireModelDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct ListaDubla* lista = malloc(sizeof(struct ListaDubla));
	lista->start = NULL;
	lista->end = NULL;

	while (!feof(file))
	{
		adaugaModelInLista(lista, citireModelDinFisier(file));
	}
	fclose(file);
	return lista;
}

void dezalocareLDModele(/*lista dubla de modele*/struct ListaDubla* lista) {
	//sunt dezalocate toate modelele si lista dublu inlantuita de elemente
	struct Nod* current = lista->start;

	while (current)
	{
		struct Nod* temp = current;
		current = current->next;
		free(temp->model.nume);
		free(temp->model.outfit);
		free(temp);
	}
	free(lista);
	//lista = NULL;
}

float calculeazaDurataMedie(/*lista de modele*/struct ListaDubla lista) {
	//calculeaza durata medie a modelelor din lista.
	float durataTotala = 0;
	struct Nod* current = lista.start;
	int nrModele = 0;
	while (current)
	{
		nrModele++;
		durataTotala += current->model.durataWalk;
		current = current->next;
	}
	
	if (nrModele)
	{
		return durataTotala / nrModele;
	}
	else
	{
		return durataTotala;
	}
}

void stergeModelDupaID(/*lista modele*/struct ListaDubla* lista, int id) {
	//sterge modelul cu id-ul primit.
	//tratati situatia ca modelul se afla si pe prima pozitie, si pe ultima pozitie

	struct Nod* current = lista->start;

	while (current != NULL && current->model.id != id)
	{
		current = current->next;
	}

	if (current == NULL)
	{
		return;
	}

	if (current->prev != NULL)
	{
		current->prev->next = current->next;
	}
	else
	{
		lista->start = current->next;
	}

	if (current->next != NULL)
	{
		current->next->prev = current->prev;
	}
	else
	{
		lista->end = current->prev;
	}

	free(current->model.nume);
	free(current->model.outfit);
	free(current);
}

char* getNumeModelCuDurataMaxima(struct ListaDubla lista) {
	float durataMaxima = -1;

	struct Nod* current = lista.start;

	// gasim durata maxima
	while (current)
	{
		if (durataMaxima < current->model.durataWalk)
		{
			durataMaxima = current->model.durataWalk;
		}
		current = current->next;
	}

	// RESETARE !!!
	current = lista.start;

	// cautam modelul cu durata maxima
	while (current)
	{
		if (durataMaxima == current->model.durataWalk)
		{
			return current->model.nume;
		}
		current = current->next;
	}

	return NULL;
}

int main() {
	struct ListaDubla* lista = citireLDModeleDinFisier("modele.txt");
	afisareListaModele(*lista);

	return 0;
}