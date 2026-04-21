#include <stdio.h>
#include <string.h>
#include <stdlib.h>

struct BugReport {
    int id;
    char* titluBug;
    char* tester;
    float timpEstimareFix;
    unsigned int severitate;
};
typedef struct BugReport BugReport;

// 1.
//
// Să se definească structurile necesare pentru reprezentarea unei cozi de bug reports
// folosind o structură de tip listă.

struct Nod {
    BugReport bugReport;
    struct Nod* prev;
    struct Nod* next;
};

struct Queue {
    struct Nod* start;
    struct Nod* end;
};
typedef struct Queue Queue;

// 2.
//
// Să se implementeze o funcție care citește din fișierul BugReports.txt mai multe bug reports
// și le inserează într-o coadă.

BugReport citesteBugReport(FILE* file) {
    char linie[256];
    BugReport b;
    b.id = -1;
    fgets(linie, 256, file);
    char sep[3] = ";\n";

    b.id = atoi(strtok(linie, sep));

    char* buffer = strtok(NULL, sep);
    b.titluBug = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(b.titluBug, buffer);

    buffer = strtok(NULL, sep);
    b.tester = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(b.tester, buffer);

    b.timpEstimareFix = atof(strtok(NULL, sep));
    b.severitate = atoi(strtok(NULL, sep));

    return b;
}

void enqueue(Queue* queue, BugReport bug) {
    // FIFO -> first in first out deci -> scoatem primul nod din lista
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->bugReport = bug;
    nou->prev = NULL;
    nou->next = queue->start;

    if (queue->start != NULL) {
        queue->end->next = nou;
    }
    else {
        queue->start = nou;
    }
    queue->end = nou;
}

Queue* citesteQueueDeBugReports(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL) return NULL;
    Queue* queue = malloc(sizeof(Queue));
    queue->start = NULL;
    queue->end = NULL;

    while (!feof(file)) {
        BugReport b = citesteBugReport(file);
        if (b.id != -1) {
            enqueue(queue, b);
        }
    }
    fclose(file);

    return queue;
}

// 3.
//
// Să se scrie o funcție care afișează toate bug report-urile din coadă.

void afisareBugReport(BugReport b) {
    printf("Id: %d\n", b.id);
    printf("Titlu bug: %s\n", b.titluBug);
    printf("Tester: %s\n", b.tester);
    printf("Timp estimare: %.2f\n", b.timpEstimareFix);
    printf("Severitate: %d\n", b.severitate);
}

void afisareQueueDeBugReports(Queue queue) {
    struct Nod* current = queue.start;

    while (current) {
        afisareBugReport(current->bugReport);
        printf("\n");
        current = current->next;
    }
}

// 4.
//
// Să se scrie o funcție care extrage primul bug report din coadă și îl returnează.
// Dacă coada este goală, funcția va returna un bug report cu id = -1.

BugReport dequeue(Queue* queue) {
    BugReport b;
    b.id = -1;

    if (queue->start == NULL) {
        return b;
    }

    struct Nod* current = queue->start;

    queue->start = queue->start->next;

    if (queue->end != NULL)
        queue->start->prev = NULL;
    else
        queue->end = NULL;

    b = queue->start->bugReport;

    free(current);

    return b;
}

// 5.
//
// Să se scrie o funcție care calculează numărul de bug report-uri trimise de un tester dat.




// 6.
//
// Să se scrie o funcție care returnează un vector alocat dinamic cu toate bug report-urile
// care au severitate mai mare sau egală cu un prag dat ca parametru.
// Dimensiunea vectorului va fi returnată prin parametru.



// 7.
//
// Să se scrie o funcție care calculează timpul total estimat de rezolvare pentru toate bug
// report-urile trimise de un tester specificat.




// 8.
//
// Să se scrie o funcție care determină bug report-ul cu timpul estimat de rezolvare maxim
// și îl returnează prin deep copy.



// 9.
//
// Să se dezaloce complet coada.



int main(void) {
    printf("Hello, World!\n");
    return 0;
}
