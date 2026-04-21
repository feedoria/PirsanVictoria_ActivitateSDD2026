#include <stdio.h>
#include  <stdlib.h>
#include <string.h>

struct Jucator {
    int id;
    char* nickname;
    char* joc;
    float scor;
    unsigned int meciuriJucate;
};
typedef struct Jucator Jucator;

// CERINȚE
//
// 1. Queue
//
// Să se definească structurile necesare pentru reprezentarea unei cozi de jucători
// care așteaptă să intre în următorul meci, folosind o listă dublu înlănțuită.

struct Nod {
    Jucator jucator;
    struct Nod* prev;
    struct Nod* next;
};

struct Queue {
    struct Nod* start;
    struct Nod* end;
};
typedef struct Queue Queue;
// 2. Enqueue
//
// Să se implementeze funcția care adaugă un jucător în coadă respectând regula FIFO.

void enqueue(Queue* queue, Jucator jucatorNou) {
    // FIFO -> first in first out -> punem la final
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->jucator = jucatorNou;
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

// 3. Dequeue
//
// Să se implementeze funcția care extrage primul jucător din coadă și îl returnează.

Jucator dequeue(Queue* queue) {
    // fifo -> il returnam pe primul si acum primul->next va deveni start
    Jucator j;
    j.id = -1;

    if (queue->start == NULL) {
        return j;
    }

    struct Nod* current = queue->start;

    j = current->jucator;

    queue->start = queue->start->next;

    if (queue->end == NULL) {
        queue->end = NULL;
    }
    else {
        queue->start->prev = NULL;
    }
    free(current);

    return j;
}

// 4. Afișare coadă
//
// Să se implementeze funcția care afișează toți jucătorii din coadă.





int main(void) {
    printf("Hello, World!\n");
    return 0;
}
