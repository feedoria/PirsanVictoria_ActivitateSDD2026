#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct RezervariHotel
{
	int id;
	int codRezervare;
	char* numeClient;
	float pretNoapte;
};
typedef struct RezervariHotel Rezervare;

struct NodArbore
{
	Rezervare rezervare;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore Nod;

void afisareRezervare(Rezervare r)
{
	printf("Id: %d\n", r.id);
	printf("cod: %d\n", r.codRezervare);
	printf("Nume: %s\n", r.numeClient);
	printf("pret: %.2f\n", r.pretNoapte);
}
//2. Implementati functia de citire a unei rezervari din fisierul rezervari.txt.
Rezervare citireRezervareDinFisier(FILE* file)
{
	char linie[256];
	Rezervare r;
	r.id = -1;
	r.codRezervare = -1;
	r.numeClient = NULL;
	r.pretNoapte = -1;

	if (file == NULL) return r;

	fgets(linie, 256, file);
	char sep[3] = ",\n";

	char* buffer;

	r.id = atoi(strtok(linie, sep));
	r.codRezervare = atoi(strtok(NULL, sep));

	buffer = strtok(NULL, sep);
	r.numeClient = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(r.numeClient, buffer);

	r.pretNoapte = atof(strtok(NULL, sep));

	return r;
}

// functiile pentru un avl 
int calculeazaInaltimeArbore(Nod* radacina)
{
	if (radacina != NULL)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	return 0;
}

int calculeazaGradEchilibru(Nod* radacina)
{
	if (radacina != NULL)
	{
		return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
	}
	return 0;
}

void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

// inserare echilibrata dupa cod rezervare 
void adaugaRezervareInArbore(Nod** radacina, Rezervare rezervareNoua)
{
	if ((*radacina) == NULL)
	{
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		nodNou->rezervare = rezervareNoua;

		*radacina = nodNou;
		return;
	}
	else
	{
		if (rezervareNoua.codRezervare < (*radacina)->rezervare.codRezervare)
		{
			adaugaRezervareInArbore(&((*radacina)->stanga), rezervareNoua);
		}
		else
		{
			adaugaRezervareInArbore(&((*radacina)->dreapta), rezervareNoua);
		}

		int gradEchilibru = calculeazaGradEchilibru(*radacina);

		if (gradEchilibru == 2)
		{
			if (calculeazaGradEchilibru((*radacina)->stanga) == -1)
			{
				rotireStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		else if (gradEchilibru == -2)
		{
			if (calculeazaGradEchilibru((*radacina)->dreapta) == 1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotireStanga(radacina);
		}
	}
}

Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	if (file == NULL) return arbore;

	while (!feof(file))
	{
		Rezervare r = citireRezervareDinFisier(file);
		adaugaRezervareInArbore(&arbore, r);
	}
	fclose(file);
	return arbore;
}

// afisare inordine
void afiseazaArboreInordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afiseazaArboreInordine(radacina->stanga);
		afisareRezervare(radacina->rezervare);
		afiseazaArboreInordine(radacina->dreapta);
	}
}

/*
6. Implementati o functie care modifica pretNoapte pentru o rezervare cautata dupa codRezervare.
*/
void modificaRezervareDupaCod(Nod** radacina, int cod, float pretDat)
{
	if ((*radacina) != NULL)
	{
		if (cod == (*radacina)->rezervare.codRezervare)
		{
			(*radacina)->rezervare.pretNoapte = pretDat;
		}
		else if (cod < (*radacina)->rezervare.codRezervare)
		{
			modificaRezervareDupaCod(&((*radacina)->stanga), cod, pretDat);
		}
		else
		{
			modificaRezervareDupaCod(&((*radacina)->dreapta), cod, pretDat);
		}
	}
}

/*
7. Implementati o functie care cauta si returneaza prin deep-copy o rezervare dupa codRezervare.
*/
Rezervare deepCopyRezervare(Rezervare r)
{
	Rezervare rez;
	rez.id = r.id;
	rez.codRezervare = r.codRezervare;
	rez.numeClient = malloc(sizeof(char) * (strlen(r.numeClient) + 1));
	strcpy(rez.numeClient, r.numeClient);
	rez.pretNoapte = r.pretNoapte;

	return rez;
}

Rezervare returneazaRezervareDupaCod(Nod* radacina, int cod)
{
	Rezervare r;
	r.id = -1;
	r.codRezervare = -1;
	r.numeClient = NULL;
	r.pretNoapte = -1;
	if (radacina != NULL)
	{
		if (cod == (radacina)->rezervare.codRezervare)
		{
			return deepCopyRezervare(radacina->rezervare);
		}
		else if (cod < (radacina)->rezervare.codRezervare)
		{
			r = returneazaRezervareDupaCod(radacina->stanga, cod);
		}
		else if (cod > (radacina)->rezervare.codRezervare)
		{
			r = returneazaRezervareDupaCod(radacina->dreapta, cod);
		}
	}
	return r; // returnam ceva invalid daca e gol arborele 
}

/*
8. Implementati o functie care salveaza prin deep-copy intr-o stiva toate rezervarile 
cu tipCamera egal cu o valoare primita ca parametru. -> o sa fac cu nume client 
*/

struct NodStiva
{
	Rezervare r;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

void adaugaInStiva(NodStiva** stiva, Rezervare rNou)
{
	NodStiva* nodNou = malloc(sizeof(NodStiva));
	nodNou->next = *stiva;
	nodNou->r = rNou;
	*stiva = nodNou;
}

void salveazaInStiva(Nod* radacina, const char* nume, NodStiva** stiva)
{
	if (radacina != NULL)
	{
		if (strcmp(nume, radacina->rezervare.numeClient) == 0)
		{
			adaugaInStiva(stiva, deepCopyRezervare(radacina->rezervare));
		}

		salveazaInStiva(radacina->stanga, nume, stiva);
		salveazaInStiva(radacina->dreapta, nume, stiva);
	}
}

void afisareStiva(NodStiva* stiva)
{
	NodStiva* current = stiva;

	while (current)
	{
		afisareRezervare(current->r);
		printf("\n");
		current = current->next;
	}
}

void dezalocareStiva(NodStiva** stiva)
{
	NodStiva* current = *stiva;

	while (current)
	{
		NodStiva* temp = current;
		current = current->next;

		free(temp->r.numeClient);
		free(temp);
	}
	*stiva = NULL;
}

void dezalocareArbore(Nod** radacina)
{
	if (radacina != NULL && (*radacina) != NULL)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		free((*radacina)->rezervare.numeClient);
		free((*radacina));
		*radacina = NULL;
	}
}
int main()
{
	Nod* arbore = citireArboreDinFisier("rezervari.txt");

	printf("===== AFISARE ARBORE INORDINE =====\n");
	afiseazaArboreInordine(arbore);

	printf("\n===== MODIFICARE PRET REZERVARE COD 300 =====\n");
	modificaRezervareDupaCod(&arbore, 300, 999.99f);
	afiseazaArboreInordine(arbore);

	printf("\n===== CAUTARE REZERVARE DUPA COD 300 =====\n");
	Rezervare r = returneazaRezervareDupaCod(arbore, 300);
	if (r.id != -1)
	{
		afisareRezervare(r);
		free(r.numeClient);
	}
	else
	{
		printf("Rezervarea nu a fost gasita.\n");
	}

	printf("\n===== SALVARE IN STIVA DUPA NUME CLIENT =====\n");
	NodStiva* stiva = NULL;
	salveazaInStiva(arbore, "Mara Ionescu", &stiva);

	printf("\n===== AFISARE STIVA =====\n");
	afisareStiva(stiva);

	dezalocareStiva(&stiva);
	dezalocareArbore(&arbore);

	return 0;
}