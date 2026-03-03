#include<stdio.h>
#include<cstring>
#include<malloc.h>

struct Film {
	int id;
	int durata;
	char* nume;
	float buget;
	char varstaMinima;
};

struct Film initializare(int _id, int _durata, const char* _nume, float _buget, char _varstaMinima) {
	struct Film s;
	s.id = _id;
	s.durata = _durata;
	s.nume = malloc(sizeof(char) * (strlen(_nume) + 1));
	strcpy(s.nume, _nume);
	s.buget = _buget;
	s.varstaMinima = _varstaMinima;
	return s;
}

struct Film copiere(struct Film f) {
	return initializare(f.id, f.durata, f.nume, f.buget, f.varstaMinima);
}

void afisare(struct Film s) {
	printf("\n\nid: %d", s.id);
	printf("\ndurata: %d", s.durata);
	printf("\nnume: %s", s.nume);
	printf("\nbuget: %5.2f", s.buget);
	printf("\nvarsta minima: %d", s.varstaMinima);
}

void afisareVector(struct Film* vector, int nrElemente) {
	for (int i = 0; i < nrElemente; i++)
		afisare(vector[i]);
}

struct Film* copiazaPrimeleNElemente(struct Film* vector, int nrElemente, int nrElementeCopiate) {
	struct Film* vectorNou = malloc(nrElementeCopiate * sizeof(struct Film));
	for (int i = 0; i < nrElementeCopiate; i++)
		vectorNou[i] = copiere(vector[i]);

	return vectorNou;
}

void dezalocare(struct Film** vector, int* nrElemente) {
	for (int i = 0; i < *nrElemente; i++)
		free((*vector)[i].nume);
	free(*vector);
	*vector = NULL;
	*nrElemente = 0;
}

void copiazaAnumiteElemente(struct Film* vector, char nrElemente, float bugetMaxim, struct Film** vectorNou, int* dimensiune) {
	*dimensiune = 0;
	for (int i = 0; i < nrElemente; i++)
		if (vector[i].buget <= bugetMaxim)
			(*dimensiune)++;
	(*vectorNou) = malloc(sizeof(struct Film) * (*dimensiune));
	int k = 0;
	for (int i = 0; i < nrElemente; i++) {
		(*vectorNou)[k] = copiere(vector[i]);
		k++;
	}
}

struct Film getPrimulElementConditionat(struct Film* vector, int nrElemente, const char* numeCautat) {
	struct Film s;
	s.id = -1;
	s.nume = NULL;
	for (int i = 0; i < nrElemente; i++)
		if (strcmp(numeCautat, vector[i].nume) == 0) {
			s = copiere(vector[i]);
			return vector[i];
		}
	return s;
}



int main() {
	struct Film f;
	f = initializare(1, 120, "Dune", 20.6, 14);
	afisare(f);

	int nrFilme = 3;
	struct Film* filme = malloc(nrFilme * sizeof(struct Film));
	filme[0] = f;
	*(filme + 1) = initializare(2, 360, "James Bond", 16, 12);
	filme[2] = initializare(3, 240, "Star Wars", 50.6, 12);
	afisareVector(filme, 3);

	struct Film* vectorNou;
	int nrElementeCopiate = 2;
	vectorNou = copiazaPrimeleNElemente(filme, 3, nrElementeCopiate);
	printf("\nprimele 2 elemente copiate: ");
	afisareVector(vectorNou, nrElementeCopiate);

	dezalocare(&vectorNou, &nrElementeCopiate);
	printf("\n\nelemente ramase dupa stergere: ");
	afisareVector(vectorNou, nrElementeCopiate);

	struct Film* filmeIeftine;
	int dimensiune;
	copiazaAnumiteElemente(filme, nrFilme, 40, &filmeIeftine, &dimensiune);
	printf("\n\nfilme ieftine: ");
	afisareVector(filmeIeftine, dimensiune);

	struct Film s;
	printf("\nFilm dupa nume: ");
	s = getPrimulElementConditionat(filme, nrFilme, "Dune");
	afisare(s);

	dezalocare(&filme, &nrFilme);
	return 0;
}