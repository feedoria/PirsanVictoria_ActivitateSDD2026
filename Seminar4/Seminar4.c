#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

/*
BRIEF TEORETIC:

1. Un vector:
✔️ elementele sunt unul lângă altul în memorie
❌ dimensiune fixă
❌ trebuie să copiezi tot când adaugi
[Masina][Masina][Masina]

2. Lista înlănțuită:

Fiecare element știe unde e următorul
[Masina | next] → [Masina | next] → [Masina | next] → NULL

Structura de bază
În C, fiecare element se numește nod
Masina info

→ datele tale (exact ca în vector)

struct Nod* next

→ pointer către următorul nod

!!!
👉 lista NU e o zonă continuă de memorie
👉 fiecare element e alocat separat cu malloc
👉 ele sunt legate prin pointeri

In memorie:
nod1 → nod2 → nod3 → NULL

In capul listei:
Nod* cap = NULL;

-> Avantaje vs vector
✔️ Avantaje
nu ai dimensiune fixă
adaugi ușor (fără copiere masivă)
flexibil
❌ Dezavantaje
acces lent (nu ai v[i])
consumă mai multă memorie (pointeri)
mai greu de implementat (pointeri)


Operațiile de bază:
1.creare nod
2.inserare (început / sfârșit)
3.parcurgere
4.ștergere
5.dezalocare


Mini-rezumat

👉 lista = lanț de noduri
👉 fiecare nod = (date + pointer la următor)
👉 ultimul nod → NULL
👉 începi de la cap

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

struct Nod {
	Masina masina;
	struct Nod* next;
} Nod;

//creare structura pentru un nod dintr-o lista simplu inlantuita

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

void afisareListaMasini(/*lista de masini*/ struct Nod* lista) {
	//afiseaza toate elemente de tip masina din lista simplu inlantuita
	//prin apelarea functiei afisareMasina()
	if ( lista == NULL) {
		printf("Lista este goala.\n");
		return;
	}

	struct Nod* current = lista;
	while (current != NULL) {
		afisareMasina(current->masina);
		current = current->next;
	}
}

void adaugaMasinaInLista(/*lista de masini*/struct Nod** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	struct Nod* nodNou = malloc(sizeof(struct Nod));
	nodNou->masina = masinaNoua; //shallow copy, daca masinaNoua are pointeri, trebuie sa facem deep copy
	nodNou->next = NULL;

	if (*lista == NULL)
	{
		*lista = nodNou;
		return;	
	}

	struct Nod* current = *lista;
	while (current->next != NULL) {
		current = current->next;
	}

	current->next = nodNou;
}

void adaugaLaInceputInLista(/*lista de masini*/struct Nod** lista, Masina masinaNoua) {
	//adauga la inceputul listei o noua masina pe care o primim ca parametru
	struct Nod* nodInceput = malloc(sizeof(struct Nod));
	nodInceput->masina = masinaNoua;
	nodInceput->next = *lista;
	*lista = nodInceput;
}

void* citireListaMasiniDinFisier(const char* numeFisier) {
	// !!! void* este pointer la orice, putem lasa asa sau sa modificam in Nod*

	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul

	FILE* file = fopen(numeFisier, "r");
	struct nod* lista = NULL;

	while (!feof(file))
	{
		Masina masina = citireMasinaDinFisier(file);
		adaugaMasinaInLista(&lista, masina);
	}
	fclose(file);
	return lista;
}

void dezalocareListaMasini(/*lista de masini*/struct Nod** lista) {
	//sunt dezalocate toate masinile si lista de elemente
	struct Nod* head = *lista;
	struct Nod* next = NULL;

	while (head != NULL)
	{
		next = head->next;
		free(head->masina.model);
		free(head->masina.numeSofer);
		free(head);
		head = next;
	}

	*lista = NULL;

}

float calculeazaPretMediu(/*lista de masini*/struct Nod* lista) {
	//calculeaza pretul mediu al masinilor din lista.
	float pretMediu = 0;
	int nr = 0;

	while (lista)
	{
		pretMediu += lista->masina.pret;
		nr++;

		lista = lista->next;
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



void stergeMasiniDinSeria(struct Nod** lista, char serieCautata) {
	//sterge toate masinile din lista care au seria primita ca parametru.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
	struct Nod* current = *lista;
	struct Nod* prev = NULL;

	while (current != NULL) {

		if (current->masina.serie == serieCautata) {

			struct Nod* deSters = current;

			// 🔹 cazul 1: primul element
			if (prev == NULL) {
				*lista = current->next;
				current = *lista;
			}
			else {
				prev->next = current->next;
				current = current->next;
			}

			// eliberare memorie
			free(deSters->masina.model);
			free(deSters->masina.numeSofer);
			free(deSters);
		}
		else {
			prev = current;
			current = current->next;
		}
	}
}

//void stergeMasiniDinSeria(struct Nod** lista, char serieCautata) {
//	struct Nod* curent = *lista;
//	struct Nod* prev = NULL;
//	struct Nod* next = curent->next;
//
//	while (curent != NULL) {
//		if (curent->masina.serie == serieCautata) {
//			if (prev != NULL) {
//				prev->next = next;
//			}
//			else {
//				*lista = next;
//			}
//
//			free(curent->masina.model);
//			free(curent->masina.numeSofer);
//			free(curent);
//
//			curent = NULL;
//		}
//		prev = curent;
//		curent = next;
//
//		if (curent != NULL) {
//			next = curent->next;
//		}
//		else {
//			next = NULL;
//		}
//	}
//}

float calculeazaPretulMasinilorUnuiSofer(/*lista masini*/struct Nod* lista, const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	float pret = 0;
	struct Nod* current = lista;

	while (current != NULL)
	{
		if (strcmp(numeSofer, current->masina.numeSofer) == 0)
			pret += current->masina.pret;
		current = current->next;
	}
	return pret;
}

int main() {
	struct Nod* lista = citireListaMasiniDinFisier("masini.txt");
	//afisareListaMasini(lista);
	/*printf("PretMediu: %.2f\n", calculeazaPretMediu(lista));
	dezalocareListaMasini(&lista);*/
	stergeMasiniDinSeria(&lista, 'A');
	//printf("Am sters\n");
	afisareListaMasini(lista);

	return 0;
}