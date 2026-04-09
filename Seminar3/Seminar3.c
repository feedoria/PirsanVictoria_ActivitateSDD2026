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

void afisareMasina(Masina masina) {
	printf("%d %d %.2f %s %s %c\n",
		masina.id,
		masina.nrUsi,
		masina.pret,
		masina.model,
		masina.numeSofer,
		masina.serie);
}

void afisareVectorMasini(Masina* masini, int nrMasini) {
	for (int i = 0; i < nrMasini; i++)
	{
		afisareMasina(masini[i]);
	}
}

void adaugaMasinaInVector(Masina** masini, int * nrMasini, Masina masinaNoua) {
	//adauga in vectorul primit o noua masina pe care o primim ca parametru
	//ATENTIE - se modifica numarul de masini din vector;
	(*nrMasini)++;
	Masina* temp = (Masina*)malloc(sizeof(Masina) * (*nrMasini));
	
	for (int i = 0; i < *(nrMasini) - 1; i++)
	{
		temp[i] = (*masini)[i];
	}

	temp[*nrMasini - 1] = masinaNoua;
	
	if (*masini != NULL)
		free(*masini);
	*masini = temp;
}

Masina citireMasinaFisier(FILE* file) {
	//functia citeste o masina dintr-un strceam deja deschis
	//masina citita este returnata;
	char linie[50];
	fgets(linie, 50, file);

	char delim[3] = ",\n";
	Masina m;

	m.id = atoi(strtok(linie, delim));
	m.nrUsi = atoi(strtok(NULL, delim));
	m.pret = atof(strtok(NULL, delim));

	char* buffer = strtok(NULL, delim);
	m.model = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(m.model, buffer);

	buffer = strtok(NULL, delim);
	m.numeSofer = (char*)malloc((strlen(buffer) + 1) * sizeof(char));
	strcpy(m.numeSofer, buffer);

	buffer = strtok(NULL, delim);
	m.serie = buffer[0];

	return m;
}

Masina* citireVectorMasiniFisier(const char* numeFisier, int* nrMasiniCitite) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaFisier()
	//numarul de masini este determinat prin numarul de citiri din fisier
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");

	Masina* masini = NULL;
	*nrMasiniCitite = 0;

	while (!feof(file))
	{
		Masina masina = citireMasinaFisier(file);
		adaugaMasinaInVector(&masini, nrMasiniCitite, masina);
	}

	fclose(file);

	return masini;

}

void dezalocareVectorMasini(Masina** vector, int* nrMasini) {
	//este dezalocat intreg vectorul de masini
	for (int i = 0; i < nrMasini; i++) 
	{
		free((*vector)[i].model);
		free((*vector)[i].numeSofer);
	}
}

int main() {
	FILE* file = fopen("masini.txt", "r");
	Masina masina = citireMasinaFisier(file);
	//afisareMasina(masina);

	Masina* masini;
	int nr = 0;
	masini = citireVectorMasiniFisier("masini.txt", &nr);
	afisareVectorMasini(masini, nr);

	return 0;
}