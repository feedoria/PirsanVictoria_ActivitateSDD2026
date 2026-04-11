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

//creare structura pentru un nod dintr-o lista simplu inlantuita
struct Nod
{
	Vraja vraja;
	struct Nod* next;
};

Vraja citireVrajaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Vraja v1;
	aux = strtok(buffer, sep);
	v1.id = atoi(aux);
	v1.manaCost = atoi(strtok(NULL, sep));
	v1.damage = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	v1.nume = malloc(strlen(aux) + 1);
	strcpy_s(v1.nume, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	v1.efect = malloc(strlen(aux) + 1);
	strcpy_s(v1.efect, strlen(aux) + 1, aux);

	v1.element = *strtok(NULL, sep);
	return v1;
}

void afisareVraja(Vraja vraja) {
	printf("Id: %d\n", vraja.id);
	printf("Cost mana: %d\n", vraja.manaCost);
	printf("Damage: %.2f\n", vraja.damage);
	printf("Nume: %s\n", vraja.nume);
	printf("Efect: %s\n", vraja.efect);
	printf("Element: %c\n\n", vraja.element);
}

void afisareListaVraji(/*lista de vraji*/struct Nod* lista) {
	//afiseaza toate elemente de tip vraja din lista simplu inlantuita
	//prin apelarea functiei afisareVraja()
	struct Nod* current = lista;

	while (current)
	{
		afisareVraja(current->vraja);
		printf("\n");
		current = current->next;
	}
}

void adaugaVrajaInLista(/*lista de vraji*/struct Nod** lista, Vraja vrajaNoua) {
	//adauga la final in lista primita o noua vraja pe care o primim ca parametru
	struct Nod* nou = malloc(sizeof(struct Nod));
	nou->vraja = vrajaNoua;
	nou->next = NULL;

	if (*lista == NULL)
	{
		*lista = nou;
		return;
	}

	struct Nod* current = *lista;

	while (current->next)
	{
		current = current->next;
	}

	current->next = nou;
}

void adaugaLaInceputInLista(/*lista de vraji*/struct Nod** lista, Vraja vrajaNoua) {
	//adauga la inceputul listei o noua vraja pe care o primim ca parametru
	struct Nod* nodNou = malloc(sizeof(struct Nod));
	nodNou->vraja = vrajaNoua;
	nodNou->next = *lista;

	*lista = nodNou;
}

Vraja* citireListaVrajiDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate vrajile din fisier
	//prin apelul repetat al functiei citireVrajaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Vraja* vraji = NULL;

	while (!feof(file))
	{
		Vraja vraja = citireVrajaDinFisier(file);
		adaugaVrajaInLista(&vraji, vraja);
	}
	fclose(file);

	return vraji;
}

void dezalocareListaVraji(/*lista de vraji*/struct Nod** lista) {
	//sunt dezalocate toate vrajile si lista de elemente
	struct Nod* current = *lista;

	while (current)
	{
		struct Nod* temp = current;

		current = current->next;

		free(temp->vraja.nume);
		free(temp->vraja.efect);
		free(temp);
	}
	
	*lista = NULL;
}

float calculeazaDamageMediu(/*lista de vraji*/struct Nod* lista) {
	//calculeaza damage-ul mediu al vrajilor din lista.
	struct Nod* current = lista;
	float sumaDamage = 0;
	int nr = 0; 
	while (current)
	{
		sumaDamage += current->vraja.damage;
		nr++;
		current = current->next;
	}
	
	if (nr)
	{
		return sumaDamage / nr;
	}
	else
	{
		return sumaDamage;
	}
}

void stergeVrajiDinElement(/*lista vraji*/struct Nod** lista, char elementCautat) {
	//sterge toate vrajile din lista care au elementul primit ca parametru.
	//tratati situatia ca vraja se afla si pe prima pozitie, si pe ultima pozitie

	//deci cand e vorba de stergere lucram si cu prev si cu next ca sa refacem legatura 
	struct Nod* current = *lista;
	struct Nod* prev = NULL;
	struct Nod* next = current->next;

	while (current)
	{
		if (elementCautat == current->vraja.element)
		{
			if (prev != NULL)
			{
				prev->next = next;
			}
			else
			{
				*lista = next;
			}

			free(current->vraja.nume);
			free(current->vraja.efect);
			free(current);

			current = next; // daca l vreau doar pe primul := NULL;
		}
		else
		{
			prev = current;
			current = next;
		}

		if (current != NULL)
		{
			next = current->next;
		}
		else
		{
			next = NULL;
		}
	}
}

float calculeazaDamageulVrajilorDintrUnElement(/*lista vraji*/struct Nod* lista, char elementCautat) {
	//calculeaza damage-ul tuturor vrajilor care au elementul primit ca parametru.
	struct Nod* current = lista;
	float sumaDamage = 0;
	while (current)
	{
		if (current->vraja.element == elementCautat)
		{
			sumaDamage += current->vraja.damage;
		}
		current = current->next;
	}
	return sumaDamage;
}

int main() {
	Vraja* lista = citireListaVrajiDinFisier("vraji.txt");
	afisareListaVraji(lista);
	printf("\n%.2f\n", calculeazaDamageMediu(lista));
	printf("%.2f\n", calculeazaDamageulVrajilorDintrUnElement(lista, 'F'));
	stergeVrajiDinElement(&lista, 'F');
	printf("Afisare dupa stergerea vrajilor cu element \'F\'\n");
	afisareListaVraji(lista);
	//dezalocareListaVraji(&lista);

	return 0;
}