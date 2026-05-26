#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Rezervare {
	int cod;
	float pret;
	char* client;
};
typedef struct Rezervare Rezervare;

struct Nod {
	Rezervare rezervare;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;

Rezervare citireRezervareDinFisier(FILE* file) {
	char linie[256];
	char sep[3] = ",\n";

	Rezervare r;
	r.cod = -1;
	r.pret = -1;
	r.client = NULL;

	if (file == NULL) return r;
	if (fgets(linie, 256, file) == NULL) return r;

	r.cod = atoi(strtok(linie, sep));
	r.pret = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	r.client = malloc(strlen(buffer) + 1);
	strcpy(r.client, buffer);

	return r;
}

void afisareRezervare(Rezervare r) {
	printf("Cod: %d\n", r.cod);
	printf("Pret: %.2f\n", r.pret);
	printf("Client: %s\n\n", r.client);
}

// TODO: deepCopyRezervare
Rezervare deepCopyRezervare(Rezervare r)
{
	Rezervare rez;
	rez.cod = r.cod;
	rez.pret = r.pret;
	rez.client = malloc(sizeof(char) * (strlen(r.client) + 1));
	strcpy(rez.client, r.client);

	return rez;
}

// TODO: max -> asta exista deja functia nu inteleg de ce ma pui mereu sa o fac

// TODO: calculeazaInaltimeArbore

int calculeazaInaltimeArbore(Nod* radacina)
{
	if (radacina != NULL)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	return 0;
}

// TODO: calculeazaGradEchilibru

int calculeazaGradEchilibru(Nod* radacina)
{
	if (radacina != NULL)
	{
		return calculeazaInaltimeArbore(radacina->stanga) - calculeazaInaltimeArbore(radacina->dreapta);
	}
	return 0;
}

// TODO: rotireStanga

void rotireStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

// TODO: rotireDreapta

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

// TODO: adaugaRezervareInAVL

void adaugaRezervareInAVL(Nod** radacina, Rezervare rezNoua)
{
	if (*radacina == NULL)
	{
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		nodNou->rezervare = rezNoua;
		*radacina = nodNou;
		return;
	}
	else if ((*radacina) != NULL)
	{
		// vom adauga in functie de cod 
		if (rezNoua.cod < (*radacina)->rezervare.cod)
		{
			adaugaRezervareInAVL(&((*radacina)->stanga), rezNoua);
		}
		else
		{
			adaugaRezervareInAVL(&((*radacina)->dreapta), rezNoua);
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

// TODO: citireArboreDinFisier

Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	if (file == NULL) return arbore;

	while (!feof(file))
	{
		Rezervare r = citireRezervareDinFisier(file);
		adaugaRezervareInAVL(&arbore, r);
	}
	fclose(file);
	return arbore;
}

// TODO: afisareInordine
void afisareArboreInordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisareArboreInordine(radacina->stanga);
		afisareRezervare(radacina->rezervare);
		afisareArboreInordine(radacina->dreapta);
	}
}

// TODO: modificaPretDupaCod
void modificaPretDupaCod(Nod** radacina, int cod, float pretNou)
{
	if ((*radacina) != NULL)
	{
		if (cod == (*radacina)->rezervare.cod)
		{
			(*radacina)->rezervare.pret = pretNou;
		}
		else if (cod < (*radacina)->rezervare.cod)
		{
			modificaPretDupaCod(&(*radacina)->stanga, cod, pretNou);
		}
		else
		{
			modificaPretDupaCod(&(*radacina)->dreapta, cod, pretNou);
		}
	}
}

// TODO: cautaRezervareDupaCod deep-copy
Rezervare cautaRezDupaCod(Nod* radacina, int cod)
{
	Rezervare r;

	r.cod = -1;
	r.pret = -1;
	r.client = NULL;
	if (radacina == NULL)
	{
		return r;
	}
	if (radacina != NULL)
	{
		Rezervare r;
		if (cod == radacina->rezervare.cod)
		{
			return deepCopyRezervare(radacina->rezervare);
		}
		else if (cod < radacina->rezervare.cod)
		{
			r = cautaRezDupaCod(radacina->stanga, cod);
		}
		else
		{
			r = cautaRezDupaCod(radacina->dreapta, cod);
		}
	}
}

// TODO: structura stiva
struct NodStiva
{
	Rezervare rezervare;
	struct NodStiva* next;
};
typedef struct NodStiva NodStiva;

// TODO: adaugaInStiva

// TODO: salveazaInStiva

// TODO: afisareStiva

// TODO: dezalocareStiva

// TODO: dezalocareArbore

void dezalocareArbore(Nod** radacina)
{
	if (radacina != NULL && (*radacina) != NULL)
	{
		dezalocareArbore(&(*radacina)->stanga);
		dezalocareArbore(&(*radacina)->dreapta);

		free((*radacina)->rezervare.client);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	Nod* arbore = citireArboreDinFisier("rezervari.txt");
	afisareArboreInordine(arbore);

	modificaPretDupaCod(&arbore, 300, 10.2);
	Rezervare rez = cautaRezDupaCod(arbore, 300);
	afisareRezervare(rez);

	free(rez.client);

	dezalocareArbore(&arbore);

	return 0;
}