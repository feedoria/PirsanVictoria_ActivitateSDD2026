/*
Cum gandesc orice functie pe arbore?
	Ce fac daca arborele e gol?
	Ce fac cu nodul curent?
	Ce fac pe stanga?
	Ce fac pe dreapta?

brief teoretic

Arbore binar de cautare - ABC

Un arbore binar de cautare este o structura de date formata din noduri.
Fiecare nod contine o informatie si doi pointeri:
- stanga
- dreapta

Pentru fiecare nod se respecta regula:
- elementele cu cheia mai mica decat cheia nodului curent se afla in subarborele stang
- elementele cu cheia mai mare decat cheia nodului curent se afla in subarborele drept

In aceasta problema cheia folosita pentru ordonare este id-ul masinii.

Structura unui nod din arbore:
struct NodArbore {
	Masina masina;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};

Inserarea se face recursiv:
- daca arborele este gol, se creeaza un nod nou
- daca id-ul nou este mai mic decat id-ul nodului curent, se insereaza in stanga
- daca id-ul nou este mai mare decat id-ul nodului curent, se insereaza in dreapta

Parcurgerile unui arbore sunt:
1. Preordine: Radacina - Stanga - Dreapta
2. Inordine: Stanga - Radacina - Dreapta
3. Postordine: Stanga - Dreapta - Radacina

Pentru un ABC, parcurgerea inordine afiseaza elementele sortate dupa cheia folosita.

Cautarea dupa id se face folosind regula arborelui:
- daca id-ul cautat este egal cu id-ul nodului curent, elementul a fost gasit
- daca id-ul cautat este mai mic, cautam in stanga
- daca id-ul cautat este mai mare, cautam in dreapta

Numarul de noduri se calculeaza recursiv:
1 + numarNoduri(stanga) + numarNoduri(dreapta)

Inaltimea arborelui reprezinta numarul de noduri de pe cel mai lung drum
de la radacina pana la cea mai indepartata frunza.
Formula:
1 + max(inaltime(stanga), inaltime(dreapta))

Pretul total se calculeaza recursiv:
pret nod curent + pret total stanga + pret total dreapta

Dezalocarea se face de obicei in postordine:
- dezalocam subarborele stang
- dezalocam subarborele drept
- dezalocam campurile dinamice din nod
- dezalocam nodul curent

Ideea principala:
ABC = arbore cu regula de cautare dupa cheie + recursivitate pe stanga si dreapta.

*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
struct NodArbore
{
	Masina masina;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore NodArbore;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret= atof(strtok(NULL, sep));
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


void adaugaMasinaInArbore(NodArbore** radacina, Masina masinaNoua) {
	if ((*radacina) == NULL)
	{
		NodArbore* nodNou = malloc(sizeof(NodArbore));
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		nodNou->masina = masinaNoua;

		*radacina = nodNou;
		return;
	}

	if (masinaNoua.id > (*radacina)->masina.id)
	{
		adaugaMasinaInArbore(&((*radacina)->dreapta), masinaNoua);
	}
	else if (masinaNoua.id < (*radacina)->masina.id)
	{
		adaugaMasinaInArbore(&((*radacina)->stanga), masinaNoua);
	}
}

NodArbore* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	FILE* file = fopen(numeFisier, "r");
	NodArbore* arbore = NULL;

	if (file == NULL) return arbore;

	while (!feof(file))
	{
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArbore(&arbore, m);
	}

	fclose(file);
	return arbore;
}

void afisarePREORDINEMasiniDinArbore(NodArbore* radacina) {
	if (radacina != NULL)
	{
		afisareMasina(radacina->masina);
		afisarePREORDINEMasiniDinArbore(radacina->stanga);
		afisarePREORDINEMasiniDinArbore(radacina->dreapta);
	}
}

void afisareINORDINEMasiniDinArbore(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		afisareINORDINEMasiniDinArbore(radacina->stanga);
		afisareMasina(radacina->masina);
		afisareINORDINEMasiniDinArbore(radacina->dreapta);
	}
}

void afisarePOSTORDINEMasiniDinArbore(NodArbore* radacina)
{
	if (radacina != NULL)
	{
		afisarePOSTORDINEMasiniDinArbore(radacina->stanga);
		afisarePOSTORDINEMasiniDinArbore(radacina->dreapta);
		afisareMasina(radacina->masina);
	}
}

void dezalocareArboreDeMasini(/*arbore de masini*/NodArbore** radacina) {
	//sunt dezalocate toate masinile si arborele de elemente
	if (*radacina != NULL)
	{
		NodArbore* stanga = (*radacina)->stanga;
		NodArbore* dreapta = (*radacina)->dreapta;

		free((*radacina)->masina.numeSofer);
		free((*radacina)->masina.model);
		free(*radacina);
		*radacina = NULL;

		dezalocareArboreDeMasini(&stanga);
		dezalocareArboreDeMasini(&dreapta);
	}

}

// chatu asa zice mai bine cu postordine:
void dezalocareArboreDeMasini(NodArbore** radacina) {
	if (*radacina != NULL)
	{
		dezalocareArboreDeMasini(&((*radacina)->stanga));
		dezalocareArboreDeMasini(&((*radacina)->dreapta));

		free((*radacina)->masina.numeSofer);
		free((*radacina)->masina.model);
		free(*radacina);

		*radacina = NULL;
	}
}

//aici face shallow copy
Masina getMasinaByID(/*arborele de masini*/NodArbore* radacina, int id) {
	Masina m;
	m.id = -1;
	m.nrUsi = -1;
	m.pret = -1;
	m.numeSofer = NULL;
	m.model = NULL;
	m.serie = 'n';
	if (radacina == NULL) return m;
	if (id < radacina->masina.id)
	{
		m = getMasinaByID(radacina->stanga, id);
	}
	else if (id > radacina->masina.id)
	{
		m = getMasinaByID(radacina->dreapta, id);
	}
	else
	{
		m = radacina->masina;
	}
	return m;
}

/*
la get masina daca VREA DEEP COPY fac asa:

if (id == radacina->masina.id)
{
	m.id = radacina->masina.id;
	m.nrUsi = radacina->masina.nrUsi;
	m.pret = radacina->masina.pret;
	m.serie = radacina->masina.serie;

	m.model = malloc(strlen(radacina->masina.model) + 1);
	strcpy(m.model, radacina->masina.model);

	m.numeSofer = malloc(strlen(radacina->masina.numeSofer) + 1);
	strcpy(m.numeSofer, radacina->masina.numeSofer);

	return m;
}
*/

int determinaNumarNoduri(/*arborele de masini*/NodArbore* radacina) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	if (radacina == NULL) return 0;
	int nrNoduriStanga = 0;
	int nrNoduriDreapta = 0;

	if (radacina != NULL)
	{
		nrNoduriStanga = determinaNumarNoduri(radacina->stanga);
		nrNoduriDreapta = determinaNumarNoduri(radacina->dreapta);

		return 1 + nrNoduriDreapta + nrNoduriStanga;
	}
}

int calculeazaInaltimeArbore(/*arbore de masini*/NodArbore* radacina) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	if (radacina != NULL)
	{
		return 1 + max(calculeazaInaltimeArbore(radacina->stanga), calculeazaInaltimeArbore(radacina->dreapta));
	}
	return 0;
}

float calculeazaPretTotal(/*arbore de masini*/NodArbore* radacina) {
	//calculeaza pretul tuturor masinilor din arbore.
	int pret = 0;
	if (radacina != NULL)
	{
		pret += radacina->masina.pret + calculeazaPretTotal(radacina->stanga) + calculeazaInaltimeArbore(radacina->dreapta);
		return pret;
	}
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/NodArbore* radacina, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	if (radacina != NULL)
	{
		int pret = 0;
		if (strcmp(radacina->masina.numeSofer, numeSofer) == 0)
		{
			pret += radacina->masina.pret + calculeazaPretulMasinilorUnuiSofer(radacina->stanga, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->dreapta, numeSofer);
			return pret;
		}
	}
	return 0;
}

int main() {


	return 0;
}