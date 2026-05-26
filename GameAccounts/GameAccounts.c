#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
Brief teoretic ABC:

ABC = Arbore Binar de Cautare.

Fiecare nod contine:
- informatia utila
- pointer spre copilul stang
- pointer spre copilul drept

Regula:
- cheia mai mica decat cheia nodului curent merge in stanga
- cheia mai mare decat cheia nodului curent merge in dreapta

In aceasta problema, cheia este id-ul contului.

Parcurgeri:
1. Preordine: Radacina - Stanga - Dreapta
2. Inordine: Stanga - Radacina - Dreapta
3. Postordine: Stanga - Dreapta - Radacina

Pentru ABC, parcurgerea inordine afiseaza elementele sortate dupa cheie.

Cele mai multe functii pe arbore sunt recursive:
- daca arborele este gol, returnam valoarea de baza
- altfel procesam nodul curent
- apoi continuam pe stanga si dreapta

Dezalocarea se face in postordine:
stanga, dreapta, nod curent.
*/

struct StructuraCont {
	int id;
	float oreJucate;
	char* username;
	char* jocPreferat;
	unsigned char rank;
};
typedef struct StructuraCont Cont;

struct NodArbore {
	Cont cont;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

Cont citireContDinFisier(FILE* file) {
	char buffer[256];
	char sep[3] = ",\n";

	Cont c;
	c.id = -1;
	c.oreJucate = 0;
	c.username = NULL;
	c.jocPreferat = NULL;
	c.rank = 'N';

	fgets(buffer, 256, file);

	char* aux = strtok(buffer, sep);
	c.id = atoi(aux);

	c.oreJucate = atof(strtok(NULL, sep));

	aux = strtok(NULL, sep);
	c.username = malloc(strlen(aux) + 1);
	strcpy(c.username, aux);

	aux = strtok(NULL, sep);
	c.jocPreferat = malloc(strlen(aux) + 1);
	strcpy(c.jocPreferat, aux);

	c.rank = *strtok(NULL, sep);

	return c;
}

void afisareCont(Cont c) {
	printf("Id: %d\n", c.id);
	printf("Ore jucate: %.2f\n", c.oreJucate);
	printf("Username: %s\n", c.username);
	printf("Joc preferat: %s\n", c.jocPreferat);
	printf("Rank: %c\n\n", c.rank);
}

// 1. Inserare in ABC dupa id
void adaugaContInArbore(NodArbore** radacina, Cont contNou) {
	// TODO:
	// daca *radacina == NULL -> aloci nod nou
	// altfel daca contNou.id < (*radacina)->cont.id -> mergi in stanga
	// altfel daca contNou.id > (*radacina)->cont.id -> mergi in dreapta
	if ((*radacina) == NULL)
	{
		NodArbore* nodNou = malloc(sizeof(NodArbore));
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		nodNou->cont = contNou;

		*radacina = nodNou;

		return;
	}
	else
	{
		if (contNou.id < (*radacina)->cont.id)
		{
			adaugaContInArbore(&((*radacina)->stanga), contNou);
		}
		else if (contNou.id > (*radacina)->cont.id)
		{
			adaugaContInArbore(&((*radacina)->dreapta), contNou);
		}
	}
}

// 2. Citire arbore din fisier
NodArbore* citireArboreDeConturiDinFisier(const char* numeFisier) {
	// TODO:
	
	FILE* file = fopen(numeFisier, "r");
	NodArbore* radacina = NULL;
	if (file == NULL) return radacina;

	while (!feof(file))
	{
		Cont c = citireContDinFisier(file);
		adaugaContInArbore(&radacina, c);
	}
	fclose(file);
	return radacina;
}

// 3. Afisare preordine: R - S - D
void afisarePREORDINEConturi(NodArbore* radacina) {
	// TODO
	if (radacina != NULL)
	{
		afisareCont(radacina->cont);
		afisarePREORDINEConturi(radacina->stanga);
		afisarePREORDINEConturi(radacina->dreapta);
	}
}

// 4. Afisare inordine: S - R - D
void afisareINORDINEConturi(NodArbore* radacina) {
	// TODO
	if (radacina != NULL)
	{
		afisareINORDINEConturi(radacina->stanga);
		afisareCont(radacina->cont);
		afisareINORDINEConturi(radacina->dreapta);
	}
}

// 5. Afisare postordine: S - D - R
void afisarePOSTORDINEConturi(NodArbore* radacina) {
	// TODO
	if (radacina != NULL)
	{
		afisarePOSTORDINEConturi(radacina->stanga);
		afisarePOSTORDINEConturi(radacina->dreapta);
		afisareCont(radacina->cont);
	}
}

// 6. Cautare cont dupa id
Cont getContByID(NodArbore* radacina, int id) {
	Cont c;
	c.id = -1;
	c.oreJucate = 0;
	c.username = NULL;
	c.jocPreferat = NULL;
	c.rank = 'N';

	if (radacina == NULL) return c;

	if (radacina != NULL)
	{
		if (id < radacina->cont.id)
		{
			c = getContByID(radacina->stanga, id);
		}
		else if (id > radacina->cont.id)
		{
			c = getContByID(radacina->dreapta, id);
		}
		else if (id == radacina->cont.id) return radacina->cont;
	}

	// TODO:
	// daca radacina == NULL return c
	// daca id == radacina->cont.id return radacina->cont
	// daca id < radacina->cont.id cauta in stanga
	// daca id > radacina->cont.id cauta in dreapta

	return c;
}

// 7. Numar total noduri
int determinaNumarNoduri(NodArbore* radacina) {
	int nrNoduriStanga = 0;
	int nrNoduriDreapta = 0;
	if (radacina != NULL)
	{
		nrNoduriStanga += determinaNumarNoduri(radacina->stanga);
		nrNoduriDreapta += determinaNumarNoduri(radacina->dreapta);
		return 1 + nrNoduriDreapta + nrNoduriStanga;
	}
	// TODO:
	// daca radacina == NULL return 0
	// return 1 + nr(stanga) + nr(dreapta)
	return 0;
}

// 8. Inaltime arbore
int calculeazaInaltimeArbore(NodArbore* radacina) {
	if (radacina != NULL)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	// TODO:
	// daca radacina == NULL return 0
	// int st = inaltime(stanga)
	// int dr = inaltime(dreapta)
	// return 1 + max(st, dr)
	return 0;
}

// 9. Suma orelor jucate pentru toate conturile
float calculeazaOreTotale(NodArbore* radacina) {
	int sumaOre = 0;
	if (radacina != NULL)
	{
		sumaOre += radacina->cont.oreJucate + calculeazaOreTotale(radacina->stanga) + calculeazaOreTotale(radacina->dreapta);
		return sumaOre;
	}
	// TODO:
	// daca radacina == NULL return 0
	// return radacina->cont.oreJucate + suma(stanga) + suma(dreapta)
	return 0;
}

// 10. Suma orelor jucate pentru un anumit joc
float calculeazaOrePentruJoc(NodArbore* radacina, const char* joc) {

	if (radacina != NULL)
	{
		int sumaOrePtJoc = 0;
		if (strcmp(radacina->cont.jocPreferat, joc) == 0)
		{
			sumaOrePtJoc += radacina->cont.oreJucate;
		}

		sumaOrePtJoc += calculeazaOrePentruJoc(radacina->stanga, joc);
		sumaOrePtJoc += calculeazaOrePentruJoc(radacina->dreapta, joc);
		return sumaOrePtJoc;
	}
	// TODO:
	// daca radacina == NULL return 0
	// suma = suma(stanga) + suma(dreapta)
	// daca strcmp(radacina->cont.jocPreferat, joc) == 0
	//      suma += radacina->cont.oreJucate
	// return suma
	return 0;
}

// 11. Dezalocare arbore
void dezalocareArboreDeConturi(NodArbore** radacina) {
	if (radacina != NULL && * radacina != NULL)
	{
		// dezalocarea se face cu postordine ca sa dezloc intai copiii si dupa parintele
		dezalocareArboreDeConturi(&((*radacina)->stanga));
		dezalocareArboreDeConturi(&((*radacina)->dreapta));

		free((*radacina)->cont.jocPreferat);
		free((*radacina)->cont.username);
		free(*radacina);
		*radacina = NULL;
	}
}

int main() {
	// optional test minimal

	return 0;
}