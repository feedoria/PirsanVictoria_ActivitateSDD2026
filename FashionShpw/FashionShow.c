#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul modele.txt
//sau va creati un alt fisier cu alte date

struct StructuraModel {
	int id;
	int experienta;
	float durataWalk;
	char* nume;
	char* outfit;
	unsigned char stil;
};
typedef struct StructuraModel Model;

void afisareModel(Model model) {
	//afiseaza toate atributele unui model
	printf("Id: %d\n", model.id);
	printf("Experienta: %d\n", model.experienta);
	printf("Durata catwalk: %.2f\n", model.durataWalk);
	printf("Nume: %s\n", model.nume);
	printf("Outfit: %s\n", model.outfit);
	printf("Stil: %c (C = Casual --- E = elegant --- A = avant-garde)\n", model.stil);
}

void afisareVectorModele(Model* modele, int nrModele) {
	//afiseaza toate elemente de tip model din vector
	//prin apelarea functiei afisareModel()
	for (int i = 0; i < nrModele; i++)
	{
		afisareModel(modele[i]);
		printf("\n");
	}
}

void adaugaModelInVector(Model** modele, int* nrModele, Model modelNou) {
	//adauga in vectorul primit un nou model pe care il primim ca parametru
	//ATENTIE - se modifica numarul de modele din vector;

	(*nrModele)++;
	Model* temp = malloc(sizeof(Model) * (*nrModele));

	for (int i = 0; i < (*nrModele) - 1; i++)
	{
		temp[i] = (*modele)[i];
	}

	temp[(*nrModele) - 1] = modelNou;

	free(*modele);

	*modele = temp;
}

Model citireModelFisier(FILE* file) {
	//functia citeste un model dintr-un stream deja deschis
	//modelul citit este returnat;
	char linie[256];
	fgets(linie, 256, file);
	char sep[3] = ",\n";
	Model model;

	model.id = atoi(strtok(linie, sep));
	model.experienta = atoi(strtok(NULL, sep));
	model.durataWalk = atof(strtok(NULL, sep));

	char* buffer = strtok(NULL, sep);
	model.nume = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(model.nume, buffer);

	buffer = strtok(NULL, sep);
	model.outfit = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(model.outfit, buffer);

	model.stil = strtok(NULL, sep)[0];
	
	return model;
}

Model* citireVectorModeleFisier(const char* numeFisier, int* nrModeleCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate modelele din fisier
	//prin apelul repetat al functiei citireModelFisier()
	//numarul de modele este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Model* vector = NULL;
	(*nrModeleCitite) = 0;

	while (!feof(file))
	{
		adaugaModelInVector(&vector, nrModeleCitite, citireModelFisier(file));
	}
	fclose(file);

	return vector;
}

void dezalocareVectorModele(Model** vector, int* nrModele) {
	//este dezalocat intreg vectorul de modele
	for (int i = 0; i < (*nrModele); i++)
	{
		free((*vector)[i].nume);
		free((*vector)[i].outfit);
	}
	free(*vector);
	*vector = NULL;
	*nrModele = 0;
}

int main() {
	Model* modele = NULL;
	int nrModele = 0;

	// citire din fisier
	modele = citireVectorModeleFisier("modele.txt", &nrModele);

	if (modele == NULL)
	{
		printf("Fisierul nu a putut fi deschis.\n");
		return 0;
	}

	// afisare vector
	printf("VECTOR MODELE:\n");
	afisareVectorModele(modele, nrModele);

	// adaugare model nou
	Model modelNou;

	modelNou.id = 100;
	modelNou.experienta = 5;
	modelNou.durataWalk = 18.5;

	modelNou.nume = malloc(strlen("Victoria") + 1);
	strcpy(modelNou.nume, "Victoria");

	modelNou.outfit = malloc(strlen("Black Gothic Dress") + 1);
	strcpy(modelNou.outfit, "Black Gothic Dress");

	modelNou.stil = 'E';

	adaugaModelInVector(&modele, &nrModele, modelNou);

	// afisare dupa adaugare
	printf("\nVECTOR DUPA ADAUGARE:\n");
	afisareVectorModele(modele, nrModele);

	// dezalocare
	dezalocareVectorModele(&modele, &nrModele);

	return 0;
}