#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

/*

BRIEF TEORETIC:

HASHTABLE:

HashTable = structura care iti permite sa gasesti foarte rapid un element dupa o cheie

Functia de Hash -> cea mai importanta:
index = hash(cheie) % dimensiune

exemple:
- pt int:
int hash(int id, int dim) {
	return id % dim;
}

-pt string:
int hash(char* nume, int dim) {
	int suma = 0;
	for (int i = 0; i < strlen(nume); i++) {
		suma += nume[i];
	}
	return suma % dim;
}

PROBLEMA: 
- coliziuni
- le rez cu liste -> fiecare poz din vector contine o lista

OPERATII DE BAZA:
Inserare:
	1. calculezi hash
	2. mergi la pozitie
	3. adaugi în lista
Stergere:
	1. calculezi hash
	2. mergi la lista
	3. stergi din lista
Cautare:
	1. calculezi hash
	2. mergi la pozitie
	3. cauti în lista

Complexitate: best case O(1) worst case O(n)

Analogie simpla
	 HashTable = catalog de biblioteca

	 nu cauti cartea prin toata biblioteca
	 folosesti indexul → mergi direct la raft
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

//creare structura pentru un nod dintr-o lista simplu inlantuita

//creare structura pentru tabela de dispersie
// aceasta este un vector de liste


//STACK
//Alegeti prin ce veti reprezenta stiva si creati structura necesara acestei stive
//putem reprezenta o stiva prin LSI, LDI sau vector

struct HashTable {
	int dim;
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

void afisareListaMasini(/*lista de masini*/) {
	//afiseaza toate elemente de tip masina din lista dublu inlantuita
	//prin apelarea functiei afisareMasina()
}

void adaugaMasinaInLista(/*lista de masini*/ Masina masinaNoua) {
	//adauga la final in lista primita o noua masina pe care o primim ca parametru
}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	return ht;
}

int calculeazaHash(/*atribut al masini pentru clusterizare*/ int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
}

void inserareMasinaInTabela(HashTable hash, Masina galerie) {
	//este folosit mecanismul CHAINING
	//este determinata pozitia si se realizeaza inserarea pe pozitia respectiva
}

HashTable citireMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void afisareTabelaDeMasini(HashTable ht) {
	//sunt afisate toate masinile cu evidentierea clusterelor realizate
}

void dezalocareTabelaDeMasini(HashTable *ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht /*valoarea pentru masina cautata*/) {
	Masina m;
	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	return m;
}

int main() {


	return 0;
}