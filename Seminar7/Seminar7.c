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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Masina masina;
	struct Nod* next;
};

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste
struct HashTable {
	int dim;
	struct Nod** head;
};
typedef struct HashTable HashTable;

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

void afisareListaMasini(/*lista de masini*/struct Nod* lista) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
	struct Nod* current = lista;

	while (current)
	{
		afisareMasina(current->masina);
		printf("\n");
		current = current->next;
	}
}

void adaugaMasinaInLista(/*lista de masini*/struct Nod** lista, Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
	struct Nod* current = *lista;
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->masina = masinaNoua;
	nou->next = NULL;

	if (*lista == NULL)
	{
		*lista = nou;
		return;
	}

	while (current->next)
	{
		current = current->next;
	}
	current->next = nou;
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	ht.dim = dimensiune;
	ht.head = malloc(sizeof(struct Nod*) * dimensiune);

	for (int i = 0; i < dimensiune; i++)
	{
		ht.head[i] = NULL;
	}
	return ht;
}

int calculeazaHash(/*atribut al masini pentru clusterizare*/int nrUsi, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	return nrUsi % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
	int pozitiaListei = calculeazaHash(masina.nrUsi, hash.dim);

	if (hash.head[pozitiaListei] != NULL)
	{
		adaugaMasinaInLista(&hash.head[pozitiaListei], masina);
		return;
	}
	// daca n are nmc in ea
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->masina = masina;
	nou->next = NULL;
	hash.head[pozitiaListei] = nou;
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	
	HashTable hashTable = initializareHashTable(5);
	if (file == NULL) return hashTable;

	while (!feof(file))
	{
		inserareMasinaInTabela(hashTable, citireMasinaDinFisier(file));
	}
	fclose(file);

	return hashTable;
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
	for (int i = 0; i < ht.dim; i++)
	{
		printf("LISTA %d\n", i+1);

		afisareListaMasini(ht.head[i]);

		printf("=============================\n");
	}
}

void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
	for (int i = 0; i < ht->dim; i++)
	{
		struct Nod* current = ht->head[i];

		while (current)
		{
			struct Nod* temp = current;

			current = current->next;

			free(temp->masina.numeSofer);
			free(temp->masina.model);
			free(temp);
		}
		ht->head[i] = NULL;
	}
	free(ht->head);
	ht->head = NULL;
	ht->dim = 0;
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	*nrClustere = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		if (ht.head[i] != NULL)
		{
			(*nrClustere)++;
		}
	}
	float* totalPreturi = malloc(sizeof(float) * (*nrClustere));
	int k = 0;
	for (int i = 0; i < ht.dim; i++)
	{
		struct Nod* current = ht.head[i];
		
		if (current != NULL)
		{
			float totalPret = 0;
			float medie = 0;
			int nr = 0;
			while (current)
			{
				totalPret += current->masina.pret;
				nr++;
				current = current->next;
			}

			if (nr)
			{
				medie = totalPret / nr;
				totalPreturi[k++] = medie;
			}
			else
			{
				totalPreturi[k++] = totalPret;
			}
		}
	}
	return totalPreturi;
}

Masina getMasinaDupaCheie(HashTable ht /*valoarea pentru masina cautata*/, int nrUsi) {
	Masina m;
	m.id = -1;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	for (int i = 0; i < ht.dim; i++)
	{
		struct Nod* current = ht.head[i];

		while (current)
		{
			if (nrUsi == current->masina.nrUsi)
			{
				m = current->masina;
			}
			current = current->next;
		}
	}
	return m;
}

int main() {
	HashTable ht = citireMasiniDinFisier("masini.txt");
	//afisareTabelaDeMasini(ht);

	//afisareMasina(getMasinaDupaCheie(ht, 3));

	int nr;
	float* medii = calculeazaPreturiMediiPerClustere(ht, &nr);

	for (int i = 0; i < nr; i++) {
		printf("Pret mediu cluster %d: %.2f\n", i + 1, medii[i]);
	}

	dezalocareTabelaDeMasini(&ht);

	return 0;
}