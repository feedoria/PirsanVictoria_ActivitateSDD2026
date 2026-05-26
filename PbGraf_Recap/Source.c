#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Student {
	int id;
	float medie;
	char* nume;
};
typedef struct Student Student;

Student citireStudentDinFisier(FILE* file) {
	char linie[256];
	char sep[3] = ",\n";

	Student s;
	s.id = -1;
	s.medie = -1;
	s.nume = NULL;

	if (file == NULL) return s;
	if (fgets(linie, 256, file) == NULL) return s;

	s.id = atoi(strtok(linie, sep));
	s.medie = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	s.nume = malloc(strlen(buffer) + 1);
	strcpy(s.nume, buffer);

	return s;
}

void afisareStudent(Student s) {
	printf("Id: %d\n", s.id);
	printf("Medie: %.2f\n", s.medie);
	printf("Nume: %s\n\n", s.nume);
}

typedef struct NodP NodP;
typedef struct NodS NodS;

struct NodP {
	Student student;
	NodP* next;
	NodS* vecini;
};

struct NodS {
	NodP* info;
	NodS* next;
};

// TODO: inserareListaPrincipala
void inserareListaPrincipala(NodP** graf, Student s)
{
	NodP* nodNou = malloc(sizeof(NodP));
	nodNou->next = *graf;
	nodNou->student = s;
	nodNou->vecini = NULL;
	*graf = nodNou;
}

// TODO: cautaNodDupaID
NodP* cautaNodDupaId(NodP* graf, int id)
{
	NodP* current = graf;

	while (current)
	{
		if (id == current->student.id)
			return current;
		current = current->next;
	}
	return NULL;
}

// TODO: inserareListaSecundara

void inserareListaSecundara(NodS** lista, NodP* vecin)
{
	NodS* nodNou = malloc(sizeof(NodS));
	nodNou->info = vecin;
	nodNou->next = *lista;
	*lista = nodNou;
}

// TODO: inserareMuchie
void inserareMuchie(NodP* listaPrincipala, int idStart, int idStop)
{
	NodP* start = cautaNodDupaId(listaPrincipala, idStart);
	NodP* stop = cautaNodDupaId(listaPrincipala, idStop);

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
	NodP* graf = NULL;

	while (!feof(file))
	{
		Student s = citireStudentDinFisier(file);
		inserareListaPrincipala(&graf, s);
	}
	fclose(file);
	return graf;
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

		if (idStart != 0 && idStop != 0)
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

	if (current)
	{
		NodS* currentS = current->vecini;

		while (currentS)
		{
			afisareStudent(currentS->info->student);
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
		printf("afisare nod principal:\n");
		afisareStudent(current->student);
		NodS* currentS = current->vecini;
		printf("\nafisare vecini noduri:\n");
		afisareListaVecini(current, current->student.id);
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

		NodS* currentS = temp->vecini;
		while (currentS)
		{
			NodS* tempS = currentS;
			currentS = currentS->next;

			free(tempS);

		}
		free(temp->student.nume);
		free(temp);
	}
	*graf = NULL;
}

int main() {
	return 0;
}