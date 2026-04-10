#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul functionalitati.txt
//sau va creati un alt fisier cu alte date

struct StructuraFunctionalitate {
	int id;
	int complexitate; //ex: 1-10
	float timpImplementare; //in ore
	char* nume;
	char* descriere;
	unsigned char tip; //ex: I - Interaction, U - UI, G - Gameplay
};
typedef struct StructuraFunctionalitate Functionalitate;

void afisareFunctionalitate(Functionalitate f) {
	//afiseaza toate atributele unei functionalitati
	printf("Id: %d\n", f.id);
	printf("Complexitate: %d\n", f.complexitate);
	printf("Timpul de implementare: %.2f\n", f.timpImplementare);
	printf("Nume: %s\n", f.nume);
	printf("Descriere: %s\n", f.descriere);
	printf("Tip: %c\n", f.tip);
	printf("\n");
}

void afisareVectorFunctionalitati(Functionalitate* f, int nr) {
	//afiseaza toate elemente de tip functionalitate din vector
	//prin apelarea functiei afisareFunctionalitate()
	for (int i = 0; i < nr; i++)
	{
		afisareFunctionalitate(f[i]);
	}
}

void adaugaFunctionalitateInVector(Functionalitate** vector, int* nr, Functionalitate fNou) {
	//adauga in vectorul primit o noua functionalitate pe care o primim ca parametru
	//ATENTIE - se modifica numarul de elemente din vector;
	(*nr)++;
	Functionalitate* vectorCopie = malloc(sizeof(Functionalitate) * (*nr));
	for (int i = 0; i < (*nr) - 1; i++)
	{
		vectorCopie[i] = (*vector)[i];
	}

	vectorCopie[(*nr) - 1] = fNou;

	free(*vector);
	
	*vector = vectorCopie;
}

Functionalitate citireFunctionalitateFisier(FILE* file) {
	char linie[256];
	fgets(linie, 256, file);

	char delim[3] = ",\n";

	Functionalitate f;

	f.id = atoi(strtok(linie, delim));
	f.complexitate = atoi(strtok(NULL, delim));
	f.timpImplementare = atof(strtok(NULL, delim));

	char* buffer = strtok(NULL, delim);
	f.nume = malloc(strlen(buffer) + 1);
	strcpy(f.nume, buffer);

	buffer = strtok(NULL, delim);
	f.descriere = malloc(strlen(buffer) + 1);
	strcpy(f.descriere, buffer);

	f.tip = strtok(NULL, delim)[0];

	return f;
}

Functionalitate* citireVectorFunctionalitatiFisier(const char* numeFisier, int* nrCitite) {
	FILE* file = fopen(numeFisier, "r");

	if (file == NULL) {
		return NULL;
	}

	Functionalitate* vectorF = NULL;
	*nrCitite = 0;

	while (!feof(file))
	{
		Functionalitate functionalitate = citireFunctionalitateFisier(file);
		adaugaFunctionalitateInVector(&vectorF, nrCitite, functionalitate);
	}

	fclose(file);
	return vectorF;
}

void dezalocareVectorFunctionalitati(Functionalitate** vector, int* nr) {
	for (int i = 0; i < *nr; i++)
	{
		free((*vector)[i].nume);
		free((*vector)[i].descriere);
	}
	free(*vector);

	*vector = NULL;
	*nr = 0;
}

int main() {
	/*Functionalitate f;
	f.id = 1;
	f.complexitate = 8;
	f.timpImplementare = 3;

	f.nume = (char*)malloc(strlen("RaycastInteraction") + 1);
	strcpy(f.nume, "RaycastInteraction");

	f.descriere = (char*)malloc(strlen("Interactiunea se efectueaza prin lovirea unui collider cu un ray, folosind concepte de fizica din engine.") + 1);
	strcpy(f.descriere, "Interactiunea se efectueaza prin lovirea unui collider cu un ray, folosind concepte de fizica din engine.");

	f.tip = 'I';

	afisareFunctionalitate(f);*/

	Functionalitate* functionalitati = NULL;
	int nr = 0;

	functionalitati = citireVectorFunctionalitatiFisier("mecanici.txt", &nr);
	afisareVectorFunctionalitati(functionalitati, nr);
	dezalocareVectorFunctionalitati(&functionalitati, &nr);

	return 0;
}