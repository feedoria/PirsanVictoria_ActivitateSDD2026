#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Reteta {
    unsigned int id;
    int nrMedicamente;
    char** listaMedicamente;
    char* doctor;
    float valBruta;
    int discount;
};
typedef struct Reteta Reteta;

struct Nod {
    Reteta reteta;
    struct Nod* next;
};

struct HashTable {
    int dim;
    struct Nod** head;
};
typedef struct HashTable HashTable;

int calculeazaHash(int nrMedicamente, int dimensiune) {
    return nrMedicamente % dimensiune;
}

void adaugaRetetaInListDinTabel(struct Nod** lista, Reteta retetaNoua) {
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->reteta = retetaNoua;
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

void insereazaInHash(HashTable ht, Reteta reteta) {
    int pozitie = calculeazaHash(reteta.nrMedicamente, ht.dim);
    adaugaRetetaInListDinTabel(&ht.head[pozitie], reteta);
}

void afisareReteta(Reteta r) {
    printf("Id: %d\n", r.id);
    printf("Nr medicamente: %d\n", r.nrMedicamente);
    for (int i = 0; i < r.nrMedicamente; i++) {
        printf("\tMedicamentul %d:\n", i +1 );
        printf("\t\t%s\n", r.listaMedicamente[i]);
    }
    printf("Doctor: %s\n", r.doctor);
    printf("Val bruta: %.2f\n", r.valBruta);
    printf("Discount : %d\n", r.discount);
}

void afisareLista(struct Nod* lista) {
    struct Nod* current = lista;
    while (current) {
        afisareReteta(current->reteta);
        printf("\n");
        current = current->next;
    }
}

void afisareHashTable(HashTable ht) {
    for (int i = 0 ; i < ht.dim; i++) {
        printf("Lista %d\n", i + 1);
        afisareLista(ht.head[i]);
        printf("===================\n");
    }
}

double valoareReteteDupaNrMedicamente(HashTable ht, int nrMed) {
    //int pozitie = calculeazaHash(nrMed, ht.dim);
    double valTotala = 0;
    for (int i = 0; i < ht.dim; i++) {
        if (ht.head[i] != NULL) {
            if (nrMed == ht.head[i]->reteta.nrMedicamente) {
                struct Nod* current = ht.head[i];

                while (current) {
                    valTotala += ht.head[i]->reteta.valBruta * (1 - ht.head[i]->reteta.discount * 100.0f);
                    current = current->next;
                }
            }
        }
    }
    return valTotala;
}

int nrRetetePerDoctor(HashTable ht, const char* doctor, int nrMed) {
    int nrRetete = 0;
    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (strcmp(doctor, ht.head[i]->reteta.doctor) == 0 &&
                nrMed == ht.head[i]->reteta.nrMedicamente) {
                nrRetete++;
            }
            current = current->next;
        }
    }
    return nrRetete;
}

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
