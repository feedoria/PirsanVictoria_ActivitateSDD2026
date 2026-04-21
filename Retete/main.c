#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Reteta {
    int id;
    char* doctor;
    char* medicament;
    float valoareBruta;
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

//==================================================
// FUNCTII UTILE
//==================================================

Reteta citireRetetaDinFisier(FILE* file) {
    char buffer[256];
    char sep[3] = ";\n";

    Reteta r;
    r.id = -1;
    r.doctor = NULL;
    r.medicament = NULL;
    r.valoareBruta = 0;
    r.discount = 0;

    if (fgets(buffer, 256, file) == NULL) {
        return r;
    }

    char* token = strtok(buffer, sep);
    if (token == NULL) return r;
    r.id = atoi(token);

    token = strtok(NULL, sep);
    r.doctor = (char*)malloc(strlen(token) + 1);
    strcpy(r.doctor, token);

    token = strtok(NULL, sep);
    r.medicament = (char*)malloc(strlen(token) + 1);
    strcpy(r.medicament, token);

    token = strtok(NULL, sep);
    r.valoareBruta = (float)atof(token);

    token = strtok(NULL, sep);
    r.discount = atoi(token);

    return r;
}

void afisareReteta(Reteta r) {
    printf("Id: %d\n", r.id);
    printf("Doctor: %s\n", r.doctor);
    printf("Medicament: %s\n", r.medicament);
    printf("Valoare bruta: %.2f\n", r.valoareBruta);
    printf("Discount: %d\n", r.discount);
}

void afisareLista(struct Nod* lista) {
    struct Nod* current = lista;
    while (current) {
        afisareReteta(current->reteta);
        printf("\n");
        current = current->next;
    }
}

void adaugaRetetaInLista(struct Nod** lista, Reteta r) {
    struct Nod* nou = (struct Nod*)malloc(sizeof(struct Nod));
    nou->reteta = r;
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

HashTable initializareHashTable(int dimensiune) {
    HashTable ht;
    ht.dim = dimensiune;
    ht.head = (struct Nod**)malloc(sizeof(struct Nod*) * dimensiune);

    for (int i = 0; i < dimensiune; i++) {
        ht.head[i] = NULL;
    }

    return ht;
}

int calculeazaHash(int id, int dimensiune) {
    return id % dimensiune;
}

void inserareRetetaInTabela(HashTable ht, Reteta r) {
    int pozitie = calculeazaHash(r.id, ht.dim);
    adaugaRetetaInLista(&ht.head[pozitie], r);
}

HashTable citireTabelaDinFisier(const char* numeFisier, int dimensiune) {
    FILE* file = fopen(numeFisier, "r");
    HashTable ht = initializareHashTable(dimensiune);

    if (file == NULL) {
        return ht;
    }

    while (!feof(file)) {
        Reteta r = citireRetetaDinFisier(file);
        if (r.id != -1) {
            inserareRetetaInTabela(ht, r);
        }
    }

    fclose(file);
    return ht;
}

//==================================================
// CERINTA 3
// afisare completa doar pentru pozitiile ocupate
//==================================================

void afisareHashTable(HashTable ht) {
    for (int i = 0; i < ht.dim; i++) {
        if (ht.head[i] != NULL) {
            printf("Pozitia %d:\n", i);
            afisareLista(ht.head[i]);
            printf("=========================\n");
        }
    }
}

//==================================================
// CERINTA 4
// calculeaza valorile nete pentru toate retetele
// unui doctor primit ca parametru
//==================================================

void afisareValoriNeteDoctor(HashTable ht, const char* doctorCautat) {
    int gasit = 0;

    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            if (strcmp(current->reteta.doctor, doctorCautat) == 0) {
                float valoareNeta =
                    current->reteta.valoareBruta -
                    current->reteta.valoareBruta * current->reteta.discount / 100.0f;

                printf("Reteta ID %d | Doctor %s | Medicament %s | Valoare neta %.2f\n",
                    current->reteta.id,
                    current->reteta.doctor,
                    current->reteta.medicament,
                    valoareNeta);

                gasit = 1;
            }

            current = current->next;
        }
    }

    if (!gasit) {
        printf("Nu exista retete pentru doctorul %s.\n", doctorCautat);
    }
}

//==================================================
// CERINTA 5
// verifica daca reteta cautata dupa cheie are
// discount peste un prag dat
//==================================================

int hasDiscountOverLevel(HashTable ht, int idCautat, int level) {
    int pozitie = calculeazaHash(idCautat, ht.dim);
    struct Nod* current = ht.head[pozitie];

    while (current) {
        if (current->reteta.id == idCautat) {
            return current->reteta.discount > level;
        }
        current = current->next;
    }

    return 0;
}

//==================================================
// CERINTA 6
// medicamentul prescris cel mai des
//==================================================

char* celMaiFrecventMedicament(HashTable ht) {
    int maxFrecventa = 0;
    char* medicamentMax = NULL;

    for (int i = 0; i < ht.dim; i++) {
        struct Nod* current = ht.head[i];

        while (current) {
            int frecventa = 0;

            for (int j = 0; j < ht.dim; j++) {
                struct Nod* aux = ht.head[j];

                while (aux) {
                    if (strcmp(current->reteta.medicament, aux->reteta.medicament) == 0) {
                        frecventa++;
                    }
                    aux = aux->next;
                }
            }

            if (frecventa > maxFrecventa) {
                maxFrecventa = frecventa;
                medicamentMax = current->reteta.medicament;
            }

            current = current->next;
        }
    }

    return medicamentMax;
}

//==================================================
// DEZALOCARE
//==================================================

void dezalocareHashTable(HashTable* ht) {
    for (int i = 0; i < ht->dim; i++) {
        struct Nod* current = ht->head[i];

        while (current) {
            struct Nod* temp = current;
            current = current->next;

            free(temp->reteta.doctor);
            free(temp->reteta.medicament);
            free(temp);
        }

        ht->head[i] = NULL;
    }

    free(ht->head);
    ht->head = NULL;
    ht->dim = 0;
}

//==================================================
// MAIN
//==================================================

int main() {
    HashTable ht = citireTabelaDinFisier("Retete.txt", 7);

    printf("=========== CERINTA 3 ===========\n");
    afisareHashTable(ht);

    printf("\n=========== CERINTA 4 ===========\n");
    afisareValoriNeteDoctor(ht, "Dr.Popescu");

    printf("\n=========== CERINTA 5 ===========\n");
    int idCautat = 4;
    int prag = 15;
    if (hasDiscountOverLevel(ht, idCautat, prag)) {
        printf("Reteta cu ID %d are discount peste %d.\n", idCautat, prag);
    } else {
        printf("Reteta cu ID %d NU are discount peste %d.\n", idCautat, prag);
    }

    printf("\n=========== CERINTA 6 ===========\n");
    char* medicament = celMaiFrecventMedicament(ht);
    if (medicament != NULL) {
        printf("Cel mai frecvent medicament prescris este: %s\n", medicament);
    } else {
        printf("Nu exista medicamente in tabela.\n");
    }

    dezalocareHashTable(&ht);

    return 0;
}