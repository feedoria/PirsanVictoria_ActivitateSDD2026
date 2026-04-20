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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod {
	Model model;
	struct Nod* next;
};
//typedef struct Nod Nod;

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

void afisareListaModele(/*lista de modele*/struct Nod* lista) {
	//afiseaza toate elemente de tip model din lista simplu inlantuita
	//prin apelarea functiei afisareModel()
	struct Nod* current = lista;

	while (current)
	{
		afisareModel(current->model);
		current = current->next;
	}

}

void adaugaModelInLista(/*lista de modele*/struct Nod** lista, Model modelNou) {
	//adauga la final in lista primita un nou model pe care il primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->model = modelNou;
	nou->next = NULL;

	if (*lista == NULL)
	{
		*lista = nou;
		return;
	}

	struct Nod* current = *lista;

	while (current->next)
	{
		current = current->next;
	}

	current->next = nou;
}

void adaugaLaInceputInLista(/*lista de modele*/struct Nod** lista, Model modelNou) {
	//adauga la inceputul listei un nou model pe care il primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->model = modelNou;
	nou->next = *lista;
	*lista = nou;
}

struct Nod* citireListaModeleDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate modelele din fisier
	//prin apelul repetat al functiei citireModelDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	struct Nod* lista = NULL;

	while (!feof(file))
	{
		adaugaModelInLista(&lista, citireModelDinFisier(file));
	}
	fclose(file);

	return lista;
}

void dezalocareListaModele(/*lista de modele*/struct Nod** lista) {
	//sunt dezalocate toate modelele si lista de elemente
	struct Nod* current = *lista;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->model.nume);
		free(temp->model.outfit);
		free(temp);
	}
	*lista = NULL;
}

float calculeazaDurataMedie(/*lista de modele*/struct Nod* lista) {
	//calculeaza durata medie a modelelor din lista.
	struct Nod* current = lista;
	float sumaDurata = 0;
	int nrModele = 0;

	while (current)
	{
		nrModele++;
		sumaDurata += current->model.durataWalk;
		current = current->next;
	}

	if (nrModele)
	{
		return sumaDurata / nrModele;
	}
	else
	{
		return sumaDurata;
	}
}

void stergeModeleDinStil(/*lista modele*/struct Nod** lista, char stilCautat) {
	//sterge toate modelele din lista care au stilul primit ca parametru.
	//tratati situatia ca modelul se afla si pe prima pozitie, si pe ultima pozitie
	struct Nod* stanga = NULL; 
	struct Nod* current = *lista;

	while (current)
	{
		struct Nod* dreapta = current->next;

		if (stilCautat == current->model.stil)
		{
			// daca e fix primul nod 
			if (stanga == NULL)
			{
				*lista = dreapta;
			}
			// daca e la mijoc 
			else
			{
				stanga->next = dreapta;
			}

			free(current->model.nume);
			free(current->model.outfit);
			free(current);
		}
		else
		{
			stanga = current;
		}
		current = dreapta;
	}
}

float calculeazaDurataModelelorDintrUnStil(/*lista modele*/struct Nod* lista, char stilCautat) {
	//calculeaza durata tuturor modelelor care au stilul primit ca parametru.
	struct Nod* current = lista;
	float totalDurataStil = 0;

	while (current)
	{
		if (current->model.stil == stilCautat)
			totalDurataStil += current->model.durataWalk;
		current = current->next;
	}
	return totalDurataStil;
}

int main() {
	struct Nod* lista = citireListaModeleDinFisier("modele.txt");
	afisareListaModele(lista);
	stergeModeleDinStil(&lista, 'E');
	printf("\nDupa ce am sters modelele cu outfituri elegante:\n");
	afisareListaModele(lista);
	printf("\nDurata medie: %.2f\n", calculeazaDurataMedie(lista));
	printf("\nTotal durata per stil dat: %.2f\n", calculeazaDurataModelelorDintrUnStil(lista, 'A'));
	FILE* file = fopen("modele.txt", "r");
	adaugaLaInceputInLista(&lista, citireModelDinFisier(file));
	printf("\nDupa ce am adaugat la inceput in lista primul model din fisier:\n");
	afisareListaModele(lista);
	return 0;
}