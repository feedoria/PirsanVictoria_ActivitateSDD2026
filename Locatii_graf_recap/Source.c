#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Locatie {
	int id;
	float distanta;
	char* nume;
};
typedef struct Locatie Locatie;

Locatie citireLocatieDinFisier(FILE* file) {
	char linie[256];
	char sep[3] = ",\n";

	Locatie l;
	l.id = -1;
	l.distanta = -1;
	l.nume = NULL;

	if (file == NULL) return l;
	if (fgets(linie, 256, file) == NULL) return l;

	l.id = atoi(strtok(linie, sep));
	l.distanta = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	l.nume = malloc(strlen(buffer) + 1);
	strcpy(l.nume, buffer);

	return l;
}

void afisareLocatie(Locatie l) {
	printf("Id: %d\n", l.id);
	printf("Distanta: %.2f\n", l.distanta);
	printf("Nume: %s\n\n", l.nume);
}

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP
{
	Locatie locatie;
	NodP* next;
	NodS* vecini;
};

struct NodS
{
	NodP* info;
	NodS* next;
};

// TODO: inserareListaPrincipala
void inserareListaPrincipala(NodP** graf, Locatie locNou)
{
	NodP* nodNou = malloc(sizeof(NodP));
	nodNou->locatie = locNou;
	nodNou->next = *graf;
	nodNou->vecini = NULL;
	*graf = nodNou;
}

// TODO: cautaNodDupaID
NodP* cautaNodDupaId(NodP* graf, int id)
{
	NodP* current = graf;

	while (current)
	{
		if (id == current->locatie.id)
			return current;
		current = current->next;
	}
}

// TODO: inserareListaSecundara
void inserareListaSecundara(NodS** graf, NodP* vecin)
{
	NodS* nodNou = malloc(sizeof(NodS));
	nodNou->info = vecin;
	nodNou->next = *graf;
	*graf = nodNou;
}

// TODO: inserareMuchie
void inserareMuchie(NodP* listaNoduriPrincipale, int idStart, int idStop)
{
	NodP* start = cautaNodDupaId(listaNoduriPrincipale, idStart);
	NodP* stop = cautaNodDupaId(listaNoduriPrincipale, idStop);

	if (start != NULL && stop != NULL)
	{
		inserareListaSecundara(&(start)->vecini, stop);
		inserareListaSecundara(&(stop)->vecini, start);
	}
}

// TODO: citireNoduriDinFisier
NodP* citireNoduriDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	NodP* listaNoduri = NULL;

	while (!feof(file))
	{
		Locatie l = citireLocatieDinFisier(file);
		inserareListaPrincipala(&listaNoduri, l);
	}
	fclose(file);
	return listaNoduri;
}

// TODO: citireMuchiiDinFisier
void citireMuchiiDinFisier(const char* numeFisier, NodP* graf)
{
	FILE* file = fopen(numeFisier, "r");

	while (!feof(file))
	{
		int idStart = 0;
		int idStop = 0;

		fscanf(file, "%d %d", &idStart, &idStop);

		while (idStart != 0 && idStop != 0)
		{
			inserareMuchie(graf, idStart, idStop);
		}
	}
	fclose(file);
}
// TODO: afisareListaVecini
void afisareListaVecini(NodP* graf, int id) 
{
	NodP* current = cautaNodDupaId(graf, id);

	if (current != NULL)
	{
		NodS* currentS = current->vecini;

		while (currentS)
		{
			afisareLocatie(currentS->info->locatie);
			currentS = currentS->next;
		}
	}
}

// TODO: afisareGraf

void afisareGraf(NodP* graf)
{
	NodP* current = graf;

	while (current)
	{
		printf("Afisare nod principal:\n");
		afisareLocatie(current->locatie);

		printf("Afisare vecini:\n");
		NodS* currentS = current->vecini;
		afisareListaVecini(current, current->locatie.id);

		current = current->next;
	}
}

// TODO: dezalocareGraf

void dezalocareGraf(NodP** graf)
{
	NodP* current = *graf;

	while (current)
	{
		NodP* temp = current;
		current = current->next;

		while (current->vecini)
		{
			NodS* currentS = current->vecini;
			current->vecini = current->vecini->next;

			free(currentS->info->locatie.nume);
			free(currentS);
		}
		free(temp->locatie.nume);
		free(temp);
	}
	*graf = NULL;
}

int main() {
	return 0;
}