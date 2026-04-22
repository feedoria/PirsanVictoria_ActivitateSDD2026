#include <stdio.h>
#include <sys/resource.h>
#include  <stdlib.h>
#include <string.h>

struct Task {
    int id;
    char* nume;
    char* tip;
    float timp;
};
typedef struct Task Task;

// CERINȚE
//
// 1. Listă simplă
//
// Să se definească structurile pentru o listă simplu înlănțuită de task-uri.

struct NodSimplu {
    Task task;
    struct NodSimplu* next;
};

// 2. Ștergere ultim nod listă simplă
//
// Să se implementeze funcția care șterge ultimul nod din listă.

void stergeUltimNodLS(struct NodSimplu** lista) {
    if (*lista == NULL) {
        return;
    }

    struct NodSimplu* ultim = *lista;

    while (ultim->next) {
        ultim = ultim->next;
    }

    struct NodSimplu* current = *lista;
    struct NodSimplu* stanga = NULL;

    while (current) {
        struct NodSimplu* dreapta = current->next;

        if (ultim->task.id == current->task.id) {
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }

            free(current->task.nume);
            free(current->task.tip);
            free(current);
            return;
        }
        else {
            stanga = current;
        }
        current = dreapta;
    }
}

// 3. Ștergere nod după id listă simplă
//
// Să se implementeze funcția care șterge task-ul cu id dat.

void stergeNodDupaID(struct NodSimplu** lista, int idCautat) {
    if (*lista == NULL) {
        return;
    }

    struct NodSimplu* current = *lista;
    struct NodSimplu* stanga = NULL;

    while (current) {
        struct NodSimplu* dreapta = current->next;

        if (idCautat == current->task.id) {
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }

            free(current->task.nume);
            free(current->task.tip);
            free(current);
            return;
        }
        else {
            stanga = current;
        }
        current = dreapta;
    }
}

// 4. Listă dublă
//
// Să se definească structurile pentru o listă dublu înlănțuită.

struct Nod {
    Task task;
    struct Nod* prev;
    struct Nod* next;
};

struct ListaDubla {
    struct Nod* start;
    struct Nod* end;
};
typedef struct ListaDubla ListaDubla;

// 5. Ștergere ultim nod LDI
//
// Să se implementeze funcția care șterge ultimul nod.

void stergeUltimLD(ListaDubla* listaDubla) {
    if (listaDubla->end == NULL) {

        return;

    }

    struct Nod* ultimul = listaDubla->end;

    struct Nod* current = listaDubla->start;

    while (current) {
        struct Nod* stanga = current->prev;
        struct Nod* dreapta = current->next;

        // verificam sa fie ultimul comparand id urile
        if (current->task.id == ultimul->task.id) {

            // verificam nodul din stanga
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                listaDubla->start = dreapta;
            }

            // verificam nodul din dreapta
            if (dreapta != NULL) {
                dreapta->prev = stanga;
            }
            else {
                listaDubla->end = stanga;
            }

            free(current->task.nume);
            free(current->task.tip);
            free(current);
            current = dreapta;
        }
        else {
            current = current->next;
        }
    }
}

// 6. Ștergere după id LDI
//
// Să se implementeze funcția care șterge nodul cu id dat.
//
// Se tratează:
//
// * primul
// * ultimul
// * mijloc
// * singur nod

void stergeDupaIdLD(ListaDubla* listaDubla, int id) {
    struct Nod* current = listaDubla->start;

    while (current) {
        struct Nod* stanga = current->prev;
        struct Nod* dreapta = current->next;

        if (id == current->task.id) {
            // verific nodul din stanga
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                listaDubla->start = dreapta;
            }

            // verific nodul din dreapta
            if (dreapta != NULL) {
                dreapta->prev = stanga;
            }
            else {
                listaDubla->end = stanga;
            }

            free(current->task.nume);
            free(current->task.tip);
            free(current);

            current = dreapta;
        }
        else {
            current = current->next;
        }
    }
}

// 7. Queue
//
// Să se definească structura cozii.

// avem Nod
struct Queue {
    struct Nod* start;
    struct Nod* end;
};
typedef struct Queue Queue;

// am facut si enqueue

void enqueue(Queue* queue, Task taskNou) {
    // FIFO -> first in first out -> adaugam la final
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->task = taskNou;
    nou->prev = queue->end;
    nou->next = NULL;

    if (queue->start == NULL) {
        queue->start = nou;
    }
    else {
        queue->end->next = nou;
    }
    queue->end = nou;
}

// 8. Dequeue
//
// Să se implementeze funcția care extrage primul element.
//
// Dacă se golește coada, se actualizează corect și end.

Task dequeue(Queue* queue) {
    // il scoatem pe primul
    Task t;
    t.id = -1;

    if (queue->start == NULL) {
        return t;
    }

    struct Nod* current = queue->start;

    t = current->task;

    queue->start = queue->start->next;

    if (queue->start == NULL) {
        queue->end = NULL;
    }
    else {
        queue->start->prev = NULL;
    }

    free(current);

    return t;
}


// 9. Vector filtrat
//
// Să se returneze vector alocat dinamic cu task-urile care au timp > prag.
//
// Dimensiunea prin parametru.


// 10. Deep copy
//
// Să se returneze task-ul cu timpul maxim prin deep copy.


// 11. Hash table chaining
//
// Cheia = id.
//
// Să se insereze task-uri într-o tabelă de dispersie cu chaining.

//definim hashtable

struct HashTable {
    int dim;
    struct NodSimplu** head;
};
typedef struct HashTable HashTable;

int calculeazaHash(int id, int dimensiune) {
    return id % dimensiune;
}

void adaugaTaskInLS(struct NodSimplu** lista, Task taskNou) {

    struct NodSimplu* nou = malloc(sizeof(struct NodSimplu));
    nou->task = taskNou;
    nou->next = NULL;

    if (*lista == NULL) {
        *lista = nou;
        return;
    }

    struct NodSimplu* current = *lista;

    while (current->next) {
        current = current->next;
    }
    current->next = nou;
}

void insereazaInTabela(HashTable ht, Task taskNou) {
    int pozite = calculeazaHash(taskNou.id, ht.dim);
    adaugaTaskInLS(&ht.head[pozite], taskNou);
}

// 12. Hash table probing
//
// Cheia = id, dimensiune = 7.
//
// Să se insereze folosind linear probing:
// poz = id % dim;
// daca e ocupat:
// poz = (poz + 1) % dim;

struct HashProbing {
    int dim;
    Task* vector;
};
typedef  struct HashProbing HashProbing;

int calculeazaHashProbing(int id, int dimensiune) {
    return id % dimensiune;
}

HashProbing initializeazaHashProbing(int dim) {
    HashProbing ht;
    ht.dim = dim;
    ht.vector = malloc(sizeof(Task) * dim);

    for (int i = 0; i < ht.dim; i ++) {
        ht.vector[i].id = -1;
        ht.vector[i].nume = NULL;
        ht.vector[i].tip = NULL;
        ht.vector[i].timp = -1;
    }
    return ht;
}

void insereazaInHashProbingTask(HashProbing ht, Task taskNou) {
    int pozitieInitiala = calculeazaHashProbing(taskNou.id, ht.dim);

    for (int i = 0; i < ht.dim; i++) {
        int pozitie = (pozitieInitiala + i) % ht.dim;

        if (ht.vector[i].id != -1) {
            ht.vector[pozitie] = taskNou;
            return;
        }
    }
}

Task* cautaDupaID(HashProbing ht, int id) {
    int pozitieInit = calculeazaHashProbing(id, ht.dim);

    for (int i = 0; i < ht.dim; i++) {
        int pozitie = (pozitieInit + i) % ht.dim;
        if (ht.vector[i].id == -1) {
            return NULL;
        }

        if (ht.vector[pozitie].id == id) {
            return &ht.vector[i];
        }
    }
}

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
