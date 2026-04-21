#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct Serviciu {
    int id;
    char* tipServiciu;
    char* numeClient;
    float sumaPlata;
    unsigned int timpEstimat;
};
typedef struct Serviciu Serviciu;

struct Nod {
    Serviciu serviciu;
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

void adaugaServiciunInLista(struct Nod** lista, Serviciu serviciuNou) {
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->serviciu = serviciuNou;
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

//metoda chaining
void inserareInHashTable(HashTable ht, Serviciu serviciu) {
    int pozitie = calculeazaHash(serviciu.numeClient, ht.dim);
    adaugaServiciunInLista(&ht.head[pozitie], serviciu);
}

Serviciu citireServiciuDinFisier(FILE* file) {
    char linie[256];
    fgets(linie, 256, file);
    char sep[3] = ";\n";

    Serviciu s;
    s.id = atoi(strtok(linie, sep));

    char* buffer = strtok(NULL, sep);
    s.tipServiciu = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(s.tipServiciu, buffer);

    buffer = strtok(NULL, sep);
    s.numeClient = malloc(sizeof(char ) * (strlen(buffer) + 1));
    strcpy(s.numeClient, buffer);

    s.sumaPlata = atof(strtok(NULL, sep));
    s.timpEstimat = atoi(strtok(NULL, sep));

    return s;
}

struct Nod* citireListaServiciiDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) return NULL;
    struct Nod* serviciiLista = NULL;

    while (!feof(file)) {
        Serviciu s = citireServiciuDinFisier(file);
        if (s.id != -1) {
            adaugaServiciunInLista(&serviciiLista, s);
        }
    }
    fclose(file);

    return serviciiLista;
}

HashTable initializareHashTable(int dimensiune) {
    HashTable ht;
    ht.dim = dimensiune;
    ht.head = malloc(sizeof(struct Nod* ) * dimensiune);

    for (int i = 0; i < ht.dim; i++) {
        ht.head[i] = NULL;
    }
    return ht;
}

HashTable citireHashTable(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    HashTable ht = initializareHashTable(5);
    if (file == NULL ) return ht;

    while (!feof(file)) {
        Serviciu s = citireServiciuDinFisier(file);
        if (s.id != -1)
            inserareInHashTable(ht, s);
    }
    fclose(file);

    return ht;
}

int nrServiciiPerClient(HashTable ht, const char* numeClient) {
    int nrServicii = 0;
    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (strcmp(numeClient, current->serviciu.numeClient)==0)
                nrServicii++;
            current = current->next;
        }
    }
    return nrServicii;
}

/*void stergeServiciuDinListaDinTabel(struct Nod** lista) {
    if (*lista == NULL) {
        return;
    }

    if ((*lista)->next == NULL) {
        free((*lista)->serviciu.numeClient);
        free((*lista)->serviciu.tipServiciu);
        free(*lista);
        *lista = NULL;
        return;
    }

    struct Nod* current = *lista;

    while (current->next->next) {
        current = current->next;
    }

    struct Nod* temp = current->next;
    current->next = NULL;

    free(temp->serviciu.numeClient);
    free(temp->serviciu.tipServiciu);
    free(temp);
}
*/

void stergeServiciuDinListaDinTabel(struct Nod** lista) {
    struct Nod* ultimul = *lista;
    while (ultimul->next) {
        ultimul = ultimul->next;
    }

    struct Nod* current = *lista;
    struct Nod* stanga = NULL;

    while (current) {
        struct Nod* dreapta = current->next;
        if (/*e ultimul*/ ultimul->serviciu.id == current->serviciu.id) {
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }
            free(current->serviciu.numeClient);

            free(current->serviciu.tipServiciu);

            free(current);
        }
        else {
            stanga = current;
        }
        current = dreapta;
    }
}

void stergeDinTabelaUnServiciuDinLista(HashTable ht, int indexLista) {

    for (int i = 0; i < ht.dim; i++) {
        if (i == indexLista) {
            stergeServiciuDinListaDinTabel(&ht.head[i]);
        }
    }
}

Serviciu* serviciiOvertime(HashTable ht, int prag, int *nr) {
    int nrServOvertime = 0;
    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (current->serviciu.timpEstimat > prag) {
                nrServOvertime++;
            }
            current = current->next;
        }
    }
    int k = 0;
    Serviciu* vectorServiciiOvertime = malloc(sizeof(Serviciu) * nrServOvertime);
    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (current->serviciu.timpEstimat > prag) {
                vectorServiciiOvertime[k++] = current->serviciu;
            }
            current = current->next;
        }
    }
    *nr = nrServOvertime;
    return vectorServiciiOvertime;
}

void afisareServiciu(Serviciu s) {

    printf("Id: %d\n", s.id);

    printf("Tip serviciu: %s\n", s.tipServiciu);

    printf("Client: %s\n", s.numeClient);

    printf("Suma plata: %.2f\n", s.sumaPlata);

    printf("Timp estimat: %u\n", s.timpEstimat);

}

void afisareLista(struct Nod* lista) {

    struct Nod* current = lista;

    while (current) {

        afisareServiciu(current->serviciu);

        printf("\n");

        current = current->next;

    }

}

void afisareHashTable(HashTable ht) {

    for (int i = 0; i < ht.dim; i++) {

        if (ht.head[i] != NULL) {

            printf("Lista %d:\n", i);

            afisareLista(ht.head[i]);

            printf("====================\n");

        }

    }

}

int main(void) {

    HashTable ht = citireHashTable("Servicii.txt");

    printf("=========== HASH TABLE INITIAL ===========\n");

    afisareHashTable(ht);

    printf("\n=========== CERINTA 2 ===========\n");

    int nr = nrServiciiPerClient(ht, "Popescu Ana");

    printf("Numar servicii pentru Popescu Ana: %d\n", nr);

    printf("\n=========== CERINTA 3 ===========\n");

    printf("Inainte de stergere:\n");

    afisareHashTable(ht);

    stergeDinTabelaUnServiciuDinLista(ht, 0);

    printf("Dupa stergere ultimului nod din lista 0:\n");

    afisareHashTable(ht);

    printf("\n=========== CERINTA 4 ===========\n");

    int nrVector = 0;

    Serviciu* vector = serviciiOvertime(ht, 60, &nrVector);

    for (int i = 0; i < nrVector; i++) {

        afisareServiciu(vector[i]);

        printf("\n");

    }

    free(vector);

    return 0;

}
