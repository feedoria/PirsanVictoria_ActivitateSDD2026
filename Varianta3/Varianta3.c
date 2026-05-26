#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

struct Zbor
{
	int id;
	char* companie;
	int indicativ;
	float durataOre;
	char* destinatie;
};
typedef struct Zbor Zbor;

struct Nod
{
	Zbor zbor;
	struct Nod* stanga;
	struct Nod* dreapta;
};
typedef struct Nod Nod;


int calculeazaInaltimeGraf(Nod* radacina)
{
	if (radacina != NULL)
	{
		return 1 + max(
			calculeazaInaltimeGraf(radacina->stanga),
			calculeazaInaltimeGraf(radacina->dreapta)
		);
	}
	return 0;
}

int calculeazaGradEchilibru(Nod* radacina)
{
	if (radacina != NULL)
	{
		return calculeazaInaltimeGraf(radacina->stanga) - calculeazaInaltimeGraf(radacina->dreapta);
	}
	return 0;
}

void rotatieStanga(Nod** radacina)
{
	Nod* aux = (*radacina)->dreapta;
	(*radacina)->dreapta = aux->stanga;
	aux->stanga = (*radacina);
	(*radacina) = aux;
}

void rotireDreapta(Nod** radacina)
{
	Nod* aux = (*radacina)->stanga;
	(*radacina)->stanga = aux->dreapta;
	aux->dreapta = (*radacina);
	(*radacina) = aux;
}

void adaugareZborInArbore(Nod** radacina, Zbor zborNou)
{
	if (*radacina == NULL)

	{
		Nod* nodNou = malloc(sizeof(Nod));
		nodNou->zbor = zborNou;
		nodNou->stanga = NULL;
		nodNou->dreapta = NULL;
		*radacina = nodNou;
		return;
	}
	if (*radacina != NULL)
	{
		if (zborNou.id < (*radacina)->zbor.id)
		{
			adaugareZborInArbore(&((*radacina)->stanga), zborNou);
		}
		else
		{
			adaugareZborInArbore(&((*radacina)->dreapta), zborNou);
		}

		int gradEchilibru = calculeazaGradEchilibru(*radacina);
		if (gradEchilibru == 2) 
		{
			if (calculeazaGradEchilibru((*radacina)->stanga) == -1)
			{
				rotatieStanga(&(*radacina)->stanga);
			}
			rotireDreapta(radacina);
		}
		else if (gradEchilibru == -2) 
		{
			if (calculeazaGradEchilibru((*radacina)->dreapta) == 1)
			{
				rotireDreapta(&(*radacina)->dreapta);
			}
			rotatieStanga(radacina);
		}
	}

}

Zbor citesteZborDinFisier(FILE* file)
{
	char linie[256];
	Zbor z;
	z.id = -1;
	z.companie = NULL;
	z.indicativ = -1;
	z.durataOre = -1;
	z.destinatie = NULL;
	if (file == NULL) return z;
	fgets(linie, 256, file);
	char sep[3] = ",\n";

	char* buffer;
	z.id = atoi(strtok(linie, sep));
	buffer = strtok(NULL, sep);
	z.companie = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(z.companie, buffer);
	z.indicativ = atoi(strtok(NULL, sep));
	z.durataOre = atof(strtok(NULL, sep));
	buffer = strtok(NULL, sep);
	z.destinatie = malloc(sizeof(char) * (strlen(buffer) + 1));
	strcpy(z.destinatie, buffer);
	return z;
}

Nod* citesteArboreDinFisier(const char* numeFisier)
{
	FILE* file = fopen(numeFisier, "r");
	Nod* arbore = NULL;
	if (file == NULL) {
		printf("Nu s a deschis fisierul.");
		return arbore;
	}
	while (!feof(file))
	{
		Zbor z = citesteZborDinFisier(file);
		adaugareZborInArbore(&arbore, z);
	}
	fclose(file);
	return arbore;
}

void afisareZbor(Zbor z)
{
	printf("Id: %d\n", z.id);
	printf("Companie: %s\n", z.companie);
	printf("Indicativ: %d\n", z.indicativ);
	printf("Durata ore: %.2f\n", z.durataOre);
	printf("Destinatie: %s\n", z.destinatie);
}

void afisareInordine(Nod* radacina)
{
	if (radacina != NULL)
	{
		afisareInordine(radacina->stanga);
		afisareZbor(radacina->zbor);
		afisareInordine(radacina->dreapta);
	}
}

void modificaDurataZborCautat(Nod** radacina, int id, float durataZborNoua)
{
	if (*radacina != NULL)
	{
		if (id == (*radacina)->zbor.id)
		{
			(*radacina)->zbor.durataOre = durataZborNoua;
		}
		else if (id < (*radacina)->zbor.id)
		{
			modificaDurataZborCautat(&((*radacina)->stanga), id, durataZborNoua);
		}
		else if (id > (*radacina)->zbor.id)
		{
			modificaDurataZborCautat(&((*radacina)->dreapta), id, durataZborNoua);
		}
	}
}

Zbor deepCopyZbor(Zbor z)
{
	Zbor zbor;
	zbor.id = z.id;
	zbor.companie = malloc(sizeof(char) * (strlen(z.companie) + 1));
	strcpy(zbor.companie, z.companie);
	zbor.indicativ = z.indicativ;
	zbor.durataOre = z.durataOre;
	zbor.destinatie = malloc(sizeof(char) * (strlen(z.destinatie) + 1));
	strcpy(zbor.destinatie, z.destinatie);
	return zbor;
}

Zbor extrageZbor(Nod* radacina, int idCautat)
{
	// nu am facut stergere deci am fact asa aceasta functie doar returnez un zbor cautat prin deep copy 
//	Zbor z;
	if (radacina != NULL)
	{
		if (idCautat == radacina->zbor.id)
		{
			return deepCopyZbor(radacina->zbor);
		}
		else if (idCautat < radacina->zbor.id)
		{
			extrageZbor(radacina->stanga, idCautat);
		}
		else
		{
			extrageZbor(radacina->dreapta, idCautat);
		}
	}
//	return z;
}

// am stiva deci am nev de un nod pt o lista simpla 
struct NodSimplu
{
	Zbor zbor;
	struct NodSimplu* next;
};
typedef struct NodSimplu NodSimplu;

// la stiva avem LIFO -> ultimu bagat iese primu 

void adaugaZborInStiva(NodSimplu** stiva, Zbor zborNou)
{
	NodSimplu* nodNou = malloc(sizeof(NodSimplu));
	nodNou->next = *stiva;
	nodNou->zbor = deepCopyZbor(zborNou);
	*stiva = nodNou;
}

//NodSimplu* toateZborurileLaDestinatieData(Nod* radacina, const char* destinatieCautata)
//{
//	NodSimplu* stiva = NULL;
//	if (radacina != NULL)
//	{
//		if (strcmp(destinatieCautata, radacina->zbor.destinatie) == 0)
//		{
//			adaugaZborInStiva(&stiva, radacina->zbor);
//		}
//
//		toateZborurileLaDestinatieData(radacina->stanga, destinatieCautata);
//		toateZborurileLaDestinatieData(radacina->dreapta, destinatieCautata);
//
//		return stiva;
//	}
//}

void salveazaZboruriLaDestinatieInStiva(Nod* radacina, const char* destinatie, NodSimplu** stiva)
{
	if (radacina != NULL)
	{
		if (strcmp(radacina->zbor.destinatie, destinatie) == 0)
		{
			adaugaZborInStiva(stiva, radacina->zbor);
		}

		salveazaZboruriLaDestinatieInStiva(radacina->stanga, destinatie, stiva);
		salveazaZboruriLaDestinatieInStiva(radacina->dreapta, destinatie, stiva);
	}
}

void afisareStiva(NodSimplu* stiva)
{
	NodSimplu* current = stiva;

	while (current)
	{
		afisareZbor(current->zbor);
		printf("\n");
		current = current->next;
	}
}

void dezalocareStiva(NodSimplu** stiva)
{
	NodSimplu* current = *stiva;

	while (current)
	{
		NodSimplu* temp = current;
		current = current->next;

		free(temp->zbor.destinatie);
		free(temp->zbor.companie);
		free(temp);
	}
	*stiva = NULL;
}

void dezalocareArbore(Nod** radacina)
{
	if (radacina != NULL && *radacina != NULL)
	{
		dezalocareArbore(&((*radacina)->stanga));
		dezalocareArbore(&((*radacina)->dreapta));

		free((*radacina)->zbor.destinatie);
		free((*radacina)->zbor.companie);
		free(*radacina);
	}
	//*radacina = NULL;
}


int main()
{
	Nod* arbore = citesteArboreDinFisier("D:\\Programming\\SDD\\Varianta3\\zboruri.txt");

	printf("Afisare avl inordine:\n");
	afisareInordine(arbore);

	modificaDurataZborCautat(&arbore, 1, 10.2);
	printf("Dupa modificarea zborului cu id-ul 1:\n");
	afisareInordine(arbore);

	//Zbor z = extrageZbor(arbore, 1);
	//afisareZbor(z);

	/*NodSimplu* stiva = toateZborurileLaDestinatieData(arbore, "Paris");
	afisareStiva(stiva);*/
	NodSimplu* stiva = NULL;
	salveazaZboruriLaDestinatieInStiva(arbore, "Paris", &stiva);
	afisareStiva(stiva);

	//free(z.companie);
	//free(z.destinatie);

	
	//dezalocareStiva(stiva);
	dezalocareArbore(&arbore);

	return 0;
}
