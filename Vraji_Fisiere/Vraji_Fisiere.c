#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul vraji.txt
//sau va creati un alt fisier cu alte date

struct StructuraVraja {
	int id;
	int manaCost;
	float damage;
	char* nume;
	char* efect;
	unsigned char element;
};
typedef struct StructuraVraja Vraja;

void afisareVraja(Vraja vraja) {
	//afiseaza toate atributele unei vraji
	printf("Id: %d\n", vraja.id);
	printf("Costul Mana: %d\n", vraja.manaCost);
	printf("Damage: %.2f\n", vraja.damage);
	printf("Nume: %s\n", vraja.nume);
	printf("Efect: %s\n", vraja.efect);
	printf("Element: %c\n", vraja.element);
}

void afisareVectorVraji(Vraja* vraji, int nrVraji) {
	//afiseaza toate elemente de tip vraja din vector
	//prin apelarea functiei afisareVraja()
	for (int i = 0; i < nrVraji; i++)
	{
		afisareVraja(vraji[i]);
		printf("\n");
	}
}

void adaugaVrajaInVector(Vraja** vraji, int* nrVraji, Vraja vrajaNoua) {
	//adauga in vectorul primit o noua vraja pe care o primim ca parametru
	//ATENTIE - se modifica numarul de vraji din vector;
	(*nrVraji)++;
	Vraja* vrajiNou = malloc(sizeof(Vraja) * (*nrVraji));
	
	for (int i = 0; i < (*nrVraji) - 1; i++)
	{
		vrajiNou[i] = (*vraji)[i];
	}

	vrajiNou[(*nrVraji) - 1] = vrajaNoua;

	free(*vraji);

	*vraji = vrajiNou;
}

Vraja citireVrajaFisier(FILE* file) {
	//functia citeste o vraja dintr-un stream deja deschis
	//vraja citita este returnata;
	char linie[256];
	fgets(linie, 256, file);
	char sep[3] = ",\n";

	Vraja v;
	v.id = atoi(strtok(linie, sep));
	v.manaCost = atoi(strtok(NULL, sep));
	v.damage = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	v.nume = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(v.nume, buffer);

	buffer = strtok(NULL, sep);
	v.efect = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(v.efect, buffer);

	v.element = strtok(NULL, sep)[0];

	return v;
}

Vraja* citireVectorVrajiFisier(const char* numeFisier, int* nrVrajiCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate vrajile din fisier
	//prin apelul repetat al functiei citireVrajaFisier()
	//numarul de vraji este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Vraja* vraji = NULL;
	*nrVrajiCitite = 0;
	
	while (!feof(file))
	{
		adaugaVrajaInVector(&vraji, nrVrajiCitite, citireVrajaFisier(file));
	}
	fclose(file);

	return vraji;
}

void dezalocareVectorVraji(Vraja** vector, int* nrVraji) {
	//este dezalocat intreg vectorul de vraji
	for (int i = 0; i < (*nrVraji); i++)
	{
		free((*vector)[i].nume);
		free((*vector)[i].efect);
	}
	free(*vector);
	*vector = NULL;
	*nrVraji = 0;
}

int main() {
	FILE* file = fopen("vraji.txt", "r");
	Vraja vraja = citireVrajaFisier(file);
	afisareVraja(vraja);

	Vraja* vraji;
	int nr = 0;
	vraji = citireVectorVrajiFisier("vraji.txt", &nr);
	afisareVectorVraji(vraji, nr);

	dezalocareVectorVraji(&vraji, &nr);
	return 0;
}