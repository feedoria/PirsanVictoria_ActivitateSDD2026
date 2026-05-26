#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct StructuraProdus
{
	int id; // am adaugat id
	int codBare;
	char* numeProdus;
	float pret;
	char* categorie;
};
typedef struct StructuraProdus Produs;

struct NodArbore
{
	Produs produs;
	struct NodArbore* stanga;
	struct NodArbore* dreapta;
};
typedef struct NodArbore Nod;

Produs citireProdusDinFisier(FILE* file)
{
	char linie[256];

	Produs p;
	p.id = -1;
	p.codBare = -1;
	p.numeProdus = NULL;
	p.pret = -1;
	p.categorie = NULL;

	if (file == NULL)
	{
		return p;
	}

	fgets(linie, 256, file);
	char sep[3] = ",\n";

	char* buffer;

	p.id = atoi(strtok(linie, sep));
	p.codBare = atoi(strtok(NULL, sep));

	buffer = strtok(NULL, sep);
	p.numeProdus = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(p.numeProdus, buffer);

	p.pret = atof(strtok(NULL, sep));

	buffer = strtok(NULL, sep);
	p.categorie = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(p.categorie, buffer);

	return p;
}

void afisareProdus(Produs p)
{
	printf("Id: %d\n", p.id);
	printf("Cod bare: %d\n", p.codBare);
	printf("Nume produs: %s\n", p.numeProdus);
	printf("Pret: %.2f\n", p.pret);
	printf("Categorie: %s\n", p.categorie);
}

// inserare intr un abc -> cheia e codul de bare
void adaugaProdusInABC(Nod** radacina, Produs produsNou)
{
	if ((*radacina) == NULL)
	{
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		nodNou->produs = produsNou;

		*radacina = nodNou;
		return;
	}
	else
	{
		if ((*radacina)->produs.codBare > produsNou.codBare)
		{
			adaugaProdusInABC(&((*radacina)->stanga), produsNou);
		}
		else
		{
			adaugaProdusInABC(&((*radacina)->dreapta), produsNou);
		}
	}
}

Nod* citireArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	if (file == NULL) return NULL;
	Nod* arbore = NULL;

	while (!feof(file))
	{
		Produs p = citireProdusDinFisier(file);
		adaugaProdusInABC(&arbore, p);
	}
	fclose(file);
	return arbore;
}

//afisare preordine 
void afisareArborePreordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisareProdus(radacina->produs);
		afisareArborePreordine(radacina->stanga);
		afisareArborePreordine(radacina->dreapta);
	}
}

float calculeazaValoareaTotalaCategorie(Nod* radacina, const char* categorie)
{
	if (radacina != NULL)
	{
		float pret = 0;
		if (strcmp(categorie, radacina->produs.categorie) == 0)
			pret += radacina->produs.pret;

		pret += calculeazaValoareaTotalaCategorie(radacina->stanga, categorie);
		pret += calculeazaValoareaTotalaCategorie(radacina->dreapta, categorie);

		return pret;
	}
	return 0;
}

Produs produsDeepCopy(Produs p)
{
	Produs pr;
	pr.id = p.id;
	pr.codBare = p.codBare;
	pr.numeProdus = malloc(sizeof(char) * strlen(p.numeProdus) + 1);
	strcpy(pr.numeProdus, p.numeProdus);
	pr.categorie = malloc(sizeof(char) * strlen(p.categorie) + 1);
	strcpy(pr.categorie, p.categorie);
	pr.pret = p.pret;

	return pr;
}

Produs extrageProdus(Nod* radacina, int codBare)
{
	Produs p;
	p.id = -1;
	p.codBare = -1;
	p.categorie = NULL;
	p.numeProdus = NULL;
	p.pret = -1;
	if (radacina != NULL) return p;
	if (radacina != NULL)
	{
		if (radacina->produs.codBare > codBare)
		{
			p = extrageProdus(radacina->stanga, codBare);
		}
		else if (radacina->produs.codBare < codBare)
		{
			p = extrageProdus(radacina->dreapta, codBare);
		}
		else if (radacina->produs.codBare == codBare)
		{
			return produsDeepCopy(radacina->produs);
		}
	}
	return p;
}

struct VectorDinamic
{
	Produs* vector;
	int nrElemente;
};
typedef struct VectorDinamic VectorDinamic;

//VectorDinamic produseleDinArboreCuPretDat(Nod* radacina, int* nrElemente, float pretCautat)
//{
//	//produsle care depastesc pretCautat le bag in vector 
//	VectorDinamic v;
//	if (radacina != NULL)
//	{
//		
//		if (pretCautat > radacina->produs.pret)
//		{
//			(*nrElemente)++;
//		}
//		else if (pretCautat <= radacina->produs.pret)
//		{
//			v = produseleDinArboreCuPretDat(radacina->stanga, (*nrElemente), pretCautat);
//		}
//	}
//
//	VectorDinamic* vectorProduse = malloc(sizeof(Produs) * (*nrElemente));
//	vectorProduse->nrElemente = (*nrElemente);
//
//	if (radacina != NULL)
//	{
//		int k = 0;
//		if (pretCautat > radacina->produs.pret)
//		{
//			vectorProduse->vector[k++] = produsDeepCopy(radacina->produs);
//		}
//		else
//		{
//			v = produseleDinArboreCuPretDat(radacina->stanga, k, pretCautat);
//		}
//	}
//	return *vectorProduse;
//}

int numaraProdusePestePret(Nod* radacina, float pretCautat)
{
	if (radacina == NULL) return 0;

	int nr = 0;

	if (radacina->produs.pret > pretCautat)
	{
		nr++;
	}

	nr += numaraProdusePestePret(radacina->stanga, pretCautat);
	nr += numaraProdusePestePret(radacina->dreapta, pretCautat);

	return nr;
}

void adaugaProdusePestePretInVector(Nod* radacina, Produs* vector, int* k, float pretCautat)
{
	if (radacina != NULL)
	{
		if (radacina->produs.pret > pretCautat)
		{
			vector[*k] = produsDeepCopy(radacina->produs);
			(*k)++;
		}

		adaugaProdusePestePretInVector(radacina->stanga, vector, k, pretCautat);
		adaugaProdusePestePretInVector(radacina->dreapta, vector, k, pretCautat);
	}
}

Produs* produseDinArboreCuPretPeste(Nod* radacina, float pretCautat, int* nrElemente)
{
	*nrElemente = numaraProdusePestePret(radacina, pretCautat);

	Produs* vector = malloc(sizeof(Produs) * (*nrElemente));

	int k = 0;
	adaugaProdusePestePretInVector(radacina, vector, &k, pretCautat);

	return vector;
}

void dezalocareArbore(Nod** radacina)
{
	if (radacina != NULL && *radacina != NULL)
	{
		dezalocareArbore(&((*radacina)->stanga));
		dezalocareArbore(&((*radacina)->dreapta));

		free((*radacina)->produs.numeProdus);
		free((*radacina)->produs.categorie);
		free(*radacina);

		*radacina = NULL;
	}
}

void dezalocareVectorProduse(Produs* vector, int nr)
{
	for (int i = 0; i < nr; i++)
	{
		free(vector[i].numeProdus);
		free(vector[i].categorie);
	}

	free(vector);
}


int main()
{
	Nod* arbore = citireArboreDinFisier("produse.txt");

	afisareArborePreordine(arbore);
	return 0;
}