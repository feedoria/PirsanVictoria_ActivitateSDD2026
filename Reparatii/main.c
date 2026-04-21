#include <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <unistd.h>

/*
*Cerințe
1. Să se definească structurile necesare pentru reprezentarea unei tabele de dispersie cu chaining,
în care cheia de căutare este numele clientului.
2. Să se implementeze o funcție care citește din fișierul Reparatii.txt mai multe reparații și le
inserează în tabela de dispersie.
3. Să se scrie o funcție care afișează doar pozițiile ocupate din tabela de dispersie, împreună cu
elementele din listele corespunzătoare.
-- done
4. Să se scrie o funcție care calculează numărul de reparații efectuate pentru un client dat.
5. Să se scrie o funcție care șterge ultimul element din lista aflată pe o poziție primită ca parametru
din tabela de dispersie.
-- done
6. Să se scrie o funcție care returnează un vector alocat dinamic cu toate reparațiile a căror durată
depășește un prag dat ca parametru. Lungimea vectorului va fi returnată prin parametru.
7. Să se dezaloce complet tabela de dispersie.
 */

struct Reparatie {
    int id;
    char* tipReparatie;
    char* numeClient;
    float cost;
    unsigned int durata;
};
typedef struct Reparatie Reparatie;

struct Nod {
    Reparatie reparatie;
    struct Nod* next;
};

struct HashTable {
    int dim;
    struct Nod** head;
};
typedef struct HashTable HashTable;

int calculeazaHash(const char* nume, int dimensiune) {
    int suma = 0;
    for (int i = 0; nume[i] != '\0'; i++) {
        suma += nume[i];
    }
    return suma % dimensiune;
}

Reparatie citireReparatieDinFisier(FILE* file) {
    char linie[256];
    fgets(linie, 256, file);
    char sep[3] = ";\n";

    Reparatie r;
    r.id = atoi(strtok(linie, sep));

    char* buffer = strtok(NULL, sep);
    r.tipReparatie = malloc(sizeof(char) * (strlen(buffer) +1 ));
    strcpy(r.tipReparatie, buffer);

    buffer = strtok(NULL, sep);
    r.numeClient = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(r.numeClient, buffer);

    r.cost = atof(strtok(NULL, sep));
    r.durata = atoi(strtok(NULL, sep));

    return r;
}


void adaugaReparatieInLista(struct Nod** lista, Reparatie reparatie) {
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->reparatie = reparatie;
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
        return;
    }

    struct Nod* current = *lista;

    while (current->next) {
        current = current->next;
    }
    current->next = nou;
}

struct Nod* citesteListaDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) {
        printf("Nu s-a deschis fisierul %s\n", numeFisier);
        return NULL;
    }

    struct Nod* lista = NULL;

    while (!feof(file)) {
        Reparatie r = citireReparatieDinFisier(file);
        if (r.id != -1)
        adaugaReparatieInLista(&lista, r);
    }
    fclose(file);

    return lista;
}

void inserareInTabela(HashTable ht, Reparatie reparatie) {
    int pozitie = calculeazaHash(reparatie.numeClient, ht.dim);
    adaugaReparatieInLista(&ht.head[pozitie], reparatie);
}

HashTable initializareHash(int dimensiune)
{
    HashTable ht;
    ht.dim = dimensiune;
    ht.head = malloc(sizeof(struct Nod*) * dimensiune);

    for (int i = 0; i < ht.dim; i++) {
        ht.head[i] = NULL;
    }
    return ht;
}

HashTable citireHashTableDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    HashTable ht = initializareHash(5);

    if (file == NULL ) {
        printf("Nu s-a deschis fisierul %s\n", numeFisier);
        perror("Motiv");
        return ht;
    }

    while (!feof(file)) {
        Reparatie r = citireReparatieDinFisier(file);
        if (r.id != -1) {
            inserareInTabela(ht, r);
        }
    }
    fclose(file);

    return ht;
}

void afisareReparatie(Reparatie r) {
    printf("Id %d\n", r.id);
    printf("Tip rep %s\n", r.tipReparatie);
    printf("Nume client %s\n", r.numeClient);
    printf("Cost %.2f\n", r.cost);
    printf("Durata %d\n", r.durata);
}

void afisareLista(struct Nod* lista) {
    struct Nod* current = lista;
    while (current) {
        afisareReparatie(current->reparatie);
        printf("\n");
        current = current->next;
    }
}

void afisareTabelaOcupata(HashTable ht) {
    for (int i = 0; i < ht.dim; i ++) {
        if (ht.head[i] != NULL) {
            printf("Pozitia %d\n", i +1);
            afisareLista(ht.head[i]);
            printf("==================\n");
        }
    }
}

int nrReparatiiPerClient(HashTable ht, const char* client) {
    int nr = 0;
    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (strcmp(client, current->reparatie.numeClient) == 0) {
                nr++;
            }
            current = current->next;
        }
    }

    return nr;
}

void stergeUltimulDinLista(struct Nod** lista) {
    struct Nod* ultimul = *lista;

    while (ultimul->next) {
        ultimul = ultimul->next;
    }

    struct Nod* current = *lista;
    struct Nod* stanga = NULL;

    while (current) {
        struct Nod* dreapta = current->next;

        if (/*e ultimul nod*/ ultimul->reparatie.id == current->reparatie.id) {
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }
            free(current->reparatie.numeClient);
            free(current->reparatie.tipReparatie);
            free(current);
            return;
        }
        else {
            stanga = current;
        }
        current = dreapta;
    }

}

void stergeUltimulDinHashDupaIndex(HashTable ht, int indexLista) {
    for (int i = 0; i < ht.dim; i++) {
        if (indexLista == i)
            stergeUltimulDinLista(&ht.head[i]);
    }

}

Reparatie* reparatiiTimpPestePrag(HashTable ht, int prag, int *nr) {
    (*nr) = 0;
    for (int i = 0; i < ht.dim; i ++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (current->reparatie.durata > prag)
                (*nr)++;
            current = current->next;
        }
    }
    Reparatie* reparatiiPesteDurata = malloc(sizeof(Reparatie) * (*nr));
    int k = 0;
    for (int i = 0; i < ht.dim; i ++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (current->reparatie.durata > prag)
            {
                reparatiiPesteDurata[k++] = current->reparatie;

            }
            current = current->next;
        }
    }
    return reparatiiPesteDurata;
}

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        struct Nod* current = ht->head[i];

        while (current) {
            struct Nod* temp = current;

            current = current->next;

            free(temp->reparatie.numeClient);
            free(temp->reparatie.tipReparatie);
            free(temp);
        }
        ht->head[i] = NULL;
    }
    free(ht->head);
    ht->head = NULL;
    ht->dim = 0;
}

void afisareVectorReparatii(Reparatie* v, int nr) {
    for (int i = 0; i < nr; i++) {
        afisareReparatie(v[i]);
        printf("\n");
    }
}

int main(void) {
    char path[1024];

    getcwd(path, sizeof(path));

    printf("Ruleaza din: %s\n", path);

    HashTable ht = citireHashTableDinFisier("Reparatii.txt");

    printf("=========== TABELA OCUPATA ===========\n");
    afisareTabelaOcupata(ht);

    printf("\n=========== NR REPARATII CLIENT ===========\n");
    printf("Ionescu Vlad -> %d reparatii\n",
           nrReparatiiPerClient(ht, "Ionescu Vlad"));

    printf("\n=========== STERGERE ULTIM DIN LISTA 0 ===========\n");
    stergeUltimulDinHashDupaIndex(ht, 0);
    afisareTabelaOcupata(ht);

    printf("\n=========== REPARATII PESTE PRAG ===========\n");
    int nr = 0;
    Reparatie* v = reparatiiTimpPestePrag(ht, 40, &nr);
    afisareVectorReparatii(v, nr);
    free(v);

    dezalocareHashTable(&ht);

    return 0;
}