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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Functionalitate functionalitate;
	struct Nod* next;
};

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

void afisareListaFunctionalitati(/*lista de functionalitati*/struct Nod* lista) {
	//afiseaza toate elemente de tip functionalitate din lista simplu inlantuita
	//prin apelarea functiei afisareFunctionalitate()
	if (lista == NULL) return;

	struct Nod* current = lista;
	while (current)
	{
		afisareFunctionalitate(current->functionalitate);
		current = current->next;
	}

}

void adaugaFunctionalitateInLista(struct Nod** lista, Functionalitate functionalitateNoua) {
	struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
	nou->functionalitate = functionalitateNoua;
	nou->next = NULL;

	// lista goala
	if (*lista == NULL) {
		*lista = nou;
		return;
	}

	struct Nod* current = *lista;

	while (current->next != NULL)
	{
		current = current->next;
	}

	current->next = nou;
}

void adaugaLaInceputInLista(/*lista de functionalitati*/struct Nod** lista, Functionalitate functionalitateNoua) {
	//adauga la inceputul listei o noua functionalitate pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->functionalitate = functionalitateNoua;
	nou->next = *lista;
	*lista = nou;
}

struct Nod* citireListaFunctionalitatiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate functionalitatile din fisier
	//prin apelul repetat al functiei citireFunctionalitateDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	struct Nod* lista = NULL;

	while (!feof(file))
	{
		Functionalitate f = citireFunctionalitateDinFisier(file);
		adaugaFunctionalitateInLista(&lista, f);
	}
	fclose(file);

	return lista;
}

void dezalocareListaFunctionalitati(/*lista de functionalitati*/struct Nod** lista) {
	//sunt dezalocate toate functionalitatile si lista de elemente
	struct Nod* current = *lista;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->functionalitate.nume);
		free(temp->functionalitate.descriere);
		free(temp);
	}

	*lista = NULL;
}

float calculeazaTimpMediuImplementare(/*lista de functionalitati*/struct Nod* lista) {
	//calculeaza timpul mediu de implementare al functionalitatilor din lista.
	struct Nod* current = lista;
	float timpMediuImplementare = 0;
	float suma = 0;
	int nr = 0;
	while (current)
	{
		nr++;
		suma += current->functionalitate.timpImplementare;
		current = current->next;
	}
	if (nr)
	{
		timpMediuImplementare = suma / nr;
		return timpMediuImplementare;
	}
	else
	{
		return suma;
	}
}

void stergeFunctionalitatiDinTip(/*lista functionalitati*/struct Nod** lista, char tipCautat) {
	//sterge toate functionalitatile din lista care au tipul primit ca parametru.
	//tratati situatia ca functionalitatea se afla si pe prima pozitie, si pe ultima pozitie
	struct Nod* current = *lista;
	struct Nod* prev = NULL;

	while (current != NULL)
	{
		struct Nod* next = current->next;

		if (current->functionalitate.tip == tipCautat)
		{
			if (prev == NULL)
			{
				*lista = next;
			}
			else
			{
				prev->next = next;
			}

			free(current->functionalitate.nume);
			free(current->functionalitate.descriere);
			free(current);
		}
		else
		{
			prev = current;
		}
		current = next;
	}
}

float calculeazaTimpulFunctionalitatilorDeUnTip(/*lista functionalitati*/struct Nod* lista, const char tipCautat) {
	//calculeaza timpul total de implementare al functionalitatilor de un anumit tip.
	struct Nod* current = lista;
	float timpTotalPerTipCautat = 0;

	while (current)
	{
		if (current->functionalitate.tip == tipCautat)
		{
			timpTotalPerTipCautat += current->functionalitate.timpImplementare;
		}
		
		current = current->next;
	}
	return timpTotalPerTipCautat;
}

int main() {
	struct Nod* lista = citireListaFunctionalitatiDinFisier("mecanici.txt");
	//afisareListaFunctionalitati(lista);

	printf("Timp mediu: %.2f\n", calculeazaTimpMediuImplementare(lista));
	printf("Total timp pentru tipul %c cautat: %.2f\n", 'I', calculeazaTimpulFunctionalitatilorDeUnTip(lista, 'I'));

	stergeFunctionalitatiDinTip(&lista, 'G');
	afisareListaFunctionalitati(lista);

	dezalocareListaFunctionalitati(&lista);

	return 0;
}