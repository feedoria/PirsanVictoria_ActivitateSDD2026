#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

/*

BRIEF TEORETIC:

Lista simplă:
[Masina | next] → [Masina | next] → NULL
stiu doar de urmatorul

Lista dublu înlănțuită
NULL ← [prev | Masina | next] ⇄ [prev | Masina | next] ⇄ [prev | Masina | next] → NULL
fiecare nod stie de celd dinainte si cel de dupa 

In memorie: 
nod1 <-> nod2 <-> nod3
nod1: [NULL | info | addr nod2]
nod2: [addr nod1 | info | addr nod3]
nod3: [addr nod2 | info | NULL]

-> Diferență majoră față de lista simplă

La simplă:
👉 doar cap

La dublă:
👉 de multe ori ai și coada

*/

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-o lista dublu inlantuita
struct Nod {
	Masina info;
	struct Nod* next;
	struct Nod* prev;
} Nod;

//creare structura pentru Lista Dubla 
typedef struct ListaDubla {
	struct Nod* prim;
	struct Nod* ultim;
}ListaDubla;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
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

void afisareListaMasini(/*lista dubla de masini*/struct ListaDubla lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	struct Nod* p = lista.prim;

	while (p != NULL)
	{
		afisareMasina(p->info);
		p = p->next;
	}
}

void adaugaMasinaInLista(/*lista dubla de masini*/struct ListaDubla* lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->info = masinaNoua;
	nou->next = NULL;
	nou->prev = lista->ultim;

	if (lista->ultim != NULL)
	{
		lista->ultim->next = nou;
	}
	else
	{
		lista->prim = nou;
	}
	lista->ultim = nou;
}

void adaugaLaInceputInLista(/*lista dubla de masini*/struct ListaDubla* lista, Masina masinaNoua) {
	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->info = masinaNoua;
	nou->prev = NULL;
	nou->next = lista->prim;

	if (lista->prim != NULL)
	{
		lista->prim->prev = nou;
	}
	else
	{
		lista->ultim = nou;
	}
	lista->prim = nou;
}

ListaDubla citireLDMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");

	if (file)
	{
		struct ListaDubla ld;
		ld.prim = ld.ultim = NULL;

		while (!feof(file))
		{
			adaugaMasinaInLista(&ld, citireMasinaDinFisier(file));
		}
		fclose(file);

		return ld;
	}
}


void dezalocareLDMasini(ListaDubla* lista) {
	struct Nod* nou = lista->prim;

	while (nou != NULL)
	{
		struct Nod* temp = nou;
		// avem nev de acest temp pentru ca nu avem cum sa stergem si nodul in sine altfel
		// dupa ce am sters continutul (model, numeSofer)
		nou = nou->next;

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
	}

	lista->prim = NULL;
	lista->ultim = NULL;
}

float calculeazaPretMediu(/*lista de masini*/ListaDubla lista) {
	//calculeaza pretul mediu al masinilor din lista.
	struct Nod* nou = lista.prim;
	float pretMediu = 0;
	int nr = 0;

	while (nou != NULL)
	{
		pretMediu += nou->info.pret;
		nr++;
		nou = nou->next;
	}
	if (nr)
	{
		return pretMediu / nr;
	}
	else
	{
		return pretMediu;
	}
}


void stergeMasinaDupaID(ListaDubla* lista, int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie

	// aici gestionam cazul in care nodul cautat este chiar primul 
	if (lista->prim != NULL && lista->prim->info.id == id) {
		struct Nod* temp = lista->prim;

		lista->prim = lista->prim->next;

		if (lista->prim != NULL) {
			lista->prim->prev = NULL;
		}
		else {
			lista->ultim = NULL;
		}

		free(temp->info.model);
		free(temp->info.numeSofer);
		free(temp);
		return;
	}
	
	struct Nod* current = lista->prim;

	while (current != NULL && current->info.id != id) {
		current = current->next;
	}

	if (current != NULL) {

		if (current->prev != NULL) {
			current->prev->next = current->next;
		}
		else {
			lista->prim = current->next;
		}

		if (current->next != NULL) {
			current->next->prev = current->prev;
		}
		else {
			lista->ultim = current->prev;
		}

		free(current->info.model);
		free(current->info.numeSofer);
		free(current);
	}
}

char* getNumeSoferMasinaScumpa(ListaDubla* lista) {
	// cauta masina cea mai scumpa si retuneaza numele soferului acesteia 
	struct Nod* current = lista->prim;
	if (current == NULL)
		return NULL;

	float pretMaxim = current->info.pret;

	char* nume = NULL;

	if (current->info.numeSofer != NULL)
	{
		nume = (char*)malloc(strlen(current->info.numeSofer) + 1);
		strcpy(nume, current->info.numeSofer);
	}

	int amGasitPretMaiMare = 0;

	while (current != NULL)
	{
		if (pretMaxim < current->info.pret)
		{
			pretMaxim = current->info.pret;
			amGasitPretMaiMare = 1;
		}
		else
		{
			amGasitPretMaiMare = 0;
		}

		if (amGasitPretMaiMare == 1)
		{
			if (current->info.numeSofer != NULL)
			{
				if (nume != NULL)
				{
					free(nume);
				}
				nume = (char*)malloc(strlen(current->info.numeSofer) + 1);
				strcpy(nume, current->info.numeSofer);
			}
		}

		current = current->next;
	}

	return nume;
}

int main() {
	ListaDubla lista = citireLDMasiniDinFisier("masini.txt");
	afisareListaMasini(lista);

	/*printf("\n%f", calculeazaPretMediu(lista));
	printf("\n%s", getNumeSoferMasinaScumpa(lista));*/
	stergeMasinaDupaID(&lista, 1);
	printf("\nMasini dupa stergere\n");
	afisareListaMasini(lista);

	return 0;
}