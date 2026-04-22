#define CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Task {
    int id;
    char* nume;
    float durata;
};
typedef struct Task Task;

struct NodSimplu {
    Task task;
    struct NodSimplu* next;
};

void stergeUltimul(struct NodSimplu** lista) {
    if (*lista == NULL) return;

    struct NodSimplu* ultimul = *lista;

    while (ultimul->next) {
        ultimul = ultimul->next;
    }

    struct NodSimplu* current = *lista;
    struct NodSimplu* stanga = NULL;

    while (current) {
        struct NodSimplu* dreapta = current->next;
        if (ultimul->task.id == current->task.id) {

            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }

            free(current->task.nume);
            free(current);
            return;
        }
        else {
            stanga = current;
        }
        current = dreapta;
    }
}

struct Nod {
    Task task;
    struct Nod* prev;
    struct Nod* next;
};

struct Queue {
    struct Nod* start;
    struct Nod* end;
};
typedef struct Queue Queue;

Task dequeue(Queue* queue) {
    Task t;
    t.id = -1;

    if (queue->start == NULL) return t;

    struct Nod* current = queue->start;

    t = queue->start->task;

    queue->start = queue->start->next;

    if (queue->start != NULL) {
        queue->start->prev = NULL;
    }
    else {
        queue->end = NULL;
    }
    free(current);
    return t;
}

// hash probing inserare si cautare dupa cheie
struct HashProbing {
    int dim;
    Task* vector;
};
typedef  struct HashProbing HashProbing;

// inserare
int calculeazaHash(const char* nume, int dimensiune) {
    int suma = 0;
    for (int i =0; nume[i] != '\0'; i++) {
        suma += nume[i];
    }
    return suma % dimensiune;
}

HashProbing initializareHash(int dimensiune) {
    HashProbing ht;
    ht.dim = dimensiune;
    ht.vector = malloc(sizeof(Task) * dimensiune);

    for (int i = 0; i < ht.dim; i++) {
        ht.vector[i].id = -1;
        ht.vector[i].nume = NULL;
        ht.vector[i].durata = -1.0f;
    }
    return ht;
}

void inserareHashTableProbing(HashProbing ht, Task taskNou) {
    int pozInitiala = calculeazaHash(taskNou.nume, ht.dim);

    for (int i = 0; i < ht.dim; i ++) {
        int pozitie = (pozInitiala + i) % ht.dim;

        if (ht.vector[pozitie].id == -1) {
            ht.vector[pozitie] = taskNou;
            return;
        }
     }
}

Task* cautareDupaNume(HashProbing ht, const char* nume) {
    int pozInitiala = calculeazaHash(nume, ht.dim);

    for (int i = 0; i < ht.dim; i++) {
        int pozitie = (pozInitiala + i) % ht.dim;

        if (ht.vector[pozitie].id == -1) return NULL;

        if (strcmp(ht.vector[pozitie].nume,nume)==0) {
            return &ht.vector[pozitie];
        }
    }
}

Task durataMaxima(struct NodSimplu* lista) {
    Task t;
    t.id = -1;
    t.nume = NULL;
    t.durata = -1.0f;

    float durMax = -1.0f;

    struct NodSimplu* current = lista;

    while (current) {
        if (current->task.durata > durMax) {
            durMax = current->task.durata;
        }
        current = current->next;
    }

    current = lista;

    while (current) {
        if (durMax == current->task.durata) {
            t.id = current->task.id;
            t.nume = malloc(sizeof(char) * (strlen(current->task.nume) +1));
            strcpy(t.nume, current->task.nume);
            t.durata = current->task.durata;
            return t;
        }
        current = current->next;
    }
    return t;
}

void afisareTask(Task t) {
    printf("Id: %d\n", t.id);
    printf("Nume: %s\n", t.nume);
    printf("Durata: %.2f\n", t.durata);
}

void afisareHashProbing(HashProbing ht) {
    for (int i = 0; i < ht.dim; i++) {
        if (ht.vector[i].id != -1) {
            printf("Pozitia %d:\n", i);
            afisareTask(ht.vector[i]);
            printf("\n");
        }
    }
}

void dezalocareHashProbing(HashProbing* ht) {
    for (int i = 0; i < ht->dim; i++) {
        if (ht->vector[i].id != -1) {
            free(ht->vector[i].nume);
        }
    }

    free(ht->vector);
    ht->vector = NULL;
    ht->dim = 0;
}

Task creareTask(int id, const char* nume, float durata) {
    Task t;
    t.id = id;
    t.nume = malloc(strlen(nume) + 1);
    strcpy(t.nume, nume);
    t.durata = durata;
    return t;
}

int main(void) {
    // Task t1 = {1, "spala", 10.4};
    // Task t2 = {2, "fa cafea", 8.6};
    //
    // HashProbing ht = initializareHash(2);
    // inserareHashTableProbing(ht, t1);
    // inserareHashTableProbing(ht, t2);
    //
    // struct Task* task = cautareDupaNume(ht,"spala");

    Task t1 = creareTask(1, "spala", 10.4f);

    Task t2 = creareTask(2, "fa cafea", 8.6f);

    HashProbing ht = initializareHash(2);

    inserareHashTableProbing(ht, t1);

    inserareHashTableProbing(ht, t2);

    printf("=== HASH ===\n");

    afisareHashProbing(ht);

    Task* task = cautareDupaNume(ht, "spala");

    if (task != NULL) {

        printf("=== GASIT ===\n");

        afisareTask(*task);

    } else {

        printf("Nu a fost gasit.\n");

    }



    dezalocareHashProbing(&ht);

    return 0;

}