#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Comanda {
    int cod;
    char* nume;
    int nrProduse;
    float sumaPlata;
};
typedef struct Comanda Comanda;

struct Nod {
    Comanda comanda;
    struct Nod* prev;
    struct Nod* next;
};

struct Queue {
    struct Nod* start;
    struct Nod* end;
};

void enqueue(struct Queue* queue, Comanda comanda) {
    // FIFO -> first in first out -> deci inseram la final
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->comanda = comanda;
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

Comanda dequeue(struct Queue* queue) {
    // extragem primul nod FIFO
    Comanda c;
    c.cod = -1;

    if (queue->start == NULL) {
        return c;
    }

    struct Nod* current = queue->start;

    c = current->comanda;

    queue->start = queue->start->next;

    if (queue->start != NULL) {
        queue->start->prev = NULL;
    }
    else {
        queue->end = NULL;
    }

    free(current);

    return c;
}

void afisareComanda(Comanda c) {
    printf("Cod: %d\n",c.cod);
    printf("Nume client: %s\n", c.nume);
    printf("Nr produse: %d\n", c.nrProduse);
    printf("Suma plata: %.2f\n", c.sumaPlata);
}

void afisareCoadaComanda(struct Queue queue) {
    struct Nod* current = queue.start;

    while (current) {
        afisareComanda(current->comanda);
        printf("\n");
        current = current->next;
    }
}

Comanda* comenziDepasescOSumaData(struct Queue queue, float sumaLimita, int *nr) {
    struct Nod* current = queue.start;
    *nr = 0;
    while (current) {
        if (sumaLimita <= current->comanda.sumaPlata)
            (*nr)++;
        current = current->next;
    }
    Comanda* comenziSumaDepasita = malloc(sizeof(Comanda) * (*nr));
    int k = 0;
    current = queue.start;

    while (current) {
        if (sumaLimita <= current->comanda.sumaPlata) {
            comenziSumaDepasita[k++] = current->comanda;
        }
        current = current->next;
    }

    return comenziSumaDepasita;
}

void sortareDescrescatorVector(Comanda** vector, int* nr) {
    float aux;
    for (int i = 0; i < (*nr) - 1; i++) {
        for (int j = i + 1; j < (*nr); j++) {
            if ((*vector)[i].nrProduse < (*vector)[j].nrProduse) {
                aux = (*vector)[i].nrProduse;
                (*vector)[i].nrProduse = (*vector)[j].nrProduse;
                (*vector)[j].nrProduse = aux;
            }
        }
    }
}

Comanda creareComanda(int cod, const char* nume, int nrProduse, float sumaPlata) {

    Comanda c;

    c.cod = cod;

    c.nume = malloc(strlen(nume) + 1);

    strcpy(c.nume, nume);

    c.nrProduse = nrProduse;

    c.sumaPlata = sumaPlata;

    return c;

}

int main(void) {
    struct Queue queue;
    queue.start = NULL;
    queue.end = NULL;

    Comanda c1 = creareComanda(1234, "Ionescu Vasile", 12, 310.5f);
    Comanda c2 = creareComanda(2345, "Popescu Ana", 5, 80.0f);
    Comanda c3 = creareComanda(3456, "Marin Alex", 9, 150.0f);
    Comanda c4 = creareComanda(4567, "Dumitru Elena", 15, 420.0f);
    Comanda c5 = creareComanda(5678, "Georgescu Radu", 3, 50.0f);

    enqueue(&queue, c1);
    enqueue(&queue, c2);
    enqueue(&queue, c3);
    enqueue(&queue, c4);
    enqueue(&queue, c5);

    printf("=========== COADA INITIALA ===========\n");
    afisareCoadaComanda(queue);

    printf("=========== DEQUEUE ===========\n");
    Comanda extras = dequeue(&queue);
    afisareComanda(extras);
    printf("\n");

    printf("=========== COADA DUPA DEQUEUE ===========\n");
    afisareCoadaComanda(queue);

    int nr = 0;
    Comanda* vector = comenziDepasescOSumaData(queue, 100.0f, &nr);

    printf("=========== COMENZI CU SUMA > 100 ===========\n");
    for (int i = 0; i < nr; i++) {
        afisareComanda(vector[i]);
        printf("\n");
    }

    sortareDescrescatorVector(&vector, &nr);

    printf("=========== VECTOR SORTAT ===========\n");
    for (int i = 0; i < nr; i++) {
        afisareComanda(vector[i]);
        printf("\n");
    }

    free(vector);

    // eliberare extras
    free(extras.nume);

    // eliberare coada ramasa
    while (queue.start) {
        Comanda c = dequeue(&queue);
        free(c.nume);
    }

    return 0;
}
