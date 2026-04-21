#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Cerințe
//
// 1. Să se definească structurile necesare pentru reprezentarea unei stive de task-uri folosind
// o listă simplu înlănțuită.

// 2. Să se implementeze o funcție care citește din fișierul Taskuri.txt mai multe task-uri și
// le inserează într-o stivă.

// 3. Să se scrie o funcție care afișează toate task-urile din stivă.

// 4. Să se scrie o funcție care extrage din stivă task-ul aflat în vârf și îl returnează.
// Dacă stiva este goală, funcția va returna un task cu id = -1.

// 5. Să se scrie o funcție care calculează numărul de task-uri ale unui programator dat.

// 6. Să se scrie o funcție care returnează un vector alocat dinamic cu toate task-urile care au
// estimareOre mai mare decât un prag dat ca parametru.
// Dimensiunea vectorului va fi returnată prin parametru.

// 7. Să se scrie o funcție care determină task-ul cu prioritatea maximă din stivă și îl
// returnează prin deep copy.

// 8. Să se dezaloce complet stiva.
struct Task {
    int id;
    char* titlu;
    char* programator;
    float estimareOre;
    unsigned int prioritate;
};
typedef struct Task Task;

struct Nod {
    Task task;
    struct Nod* next;
};

Task citireTaskDinFisier(FILE* file) {
    char linie[256];
    Task t;
    t.id = -1;
    fgets(linie, 256, file);
    char sep[3] = ";\n";


    t.id = atoi(strtok(linie, sep));

    char* buffer = strtok(NULL, sep);
    t.titlu = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(t.titlu, buffer);

    buffer = strtok(NULL, sep);
    t.programator = malloc(sizeof(char) * (strlen(buffer) + 1));
    strcpy(t.programator, buffer);

    t.estimareOre = atof(strtok(NULL, sep));

    t.prioritate = atoi(strtok(NULL, sep));

    return t;
}

void pushStack(struct Nod** stack, Task taskNou) {
    // last in first out LIFO -> adaugam in top = primul nod din lista
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->task = taskNou;
    nou->next = *stack;
    *stack = nou;
}

struct Nod* citireListaDinFisier(const char* numeFisier) {
    FILE* file = fopen(numeFisier, "r");
    if (file == NULL ) {
        return NULL;
    }
    struct Nod* stack = NULL;

    while (!feof(file)) {
        Task t = citireTaskDinFisier(file);
        if (t.id != -1) {
            pushStack(&stack, t);
        }
    }
    fclose(file);

    return stack;
}

void afisareTask(Task t) {
    printf("Id: %d\n ", t.id);
    printf("titlu: %s\n", t.titlu);
    printf("Programator: %s\n", t.programator);
    printf("Estimare ore: %.2f\n", t.estimareOre);
    printf("Prioritate: %d\n", t.prioritate);
}

void afisareStack(struct Nod* stack) {
    struct Nod* current = stack;

    while (current) {
        afisareTask(current->task);
        printf("\n");
        current = current->next;
    }
}

Task popTask(struct Nod** stack) {
    // last in first out -> il scoatem pe cel de la final deci -> adica topul adica primul din lista
    Task t;
    t.id = -1;

    if (*stack == NULL) {
        return t;
    }

    struct Nod* current = *stack;

    if ((*stack)->next != NULL) {
        *stack = (*stack)->next;
    }
    else {
        *stack = NULL;
    }

    t = current->task;

    free(current);

    return t;
}

int nrTaskuriProgramator(struct Nod* stack, const char* programator) {
    struct Nod* current = stack;
    int nr = 0;

    while (current) {
        if (strcmp(programator, current->task.programator)==0) {
            nr ++;
        }
        current   = current->next;
    }
    return nr;
}

Task* taskuriOrePestePrag(struct Nod* stack, int prag, int *nr) {
    struct Nod* current = stack;
    (*nr) = 0;

    while (current) {
        if (prag < current->task.estimareOre) {
            (*nr)++;
        }
        current = current->next;
    }
    Task* taskuri = malloc(sizeof(Task) * (*nr));
    int k = 0;
    current = stack;
    while (current) {
        if (prag < current->task.estimareOre) {
            taskuri[k++] = current->task;
        }
        current = current->next;
    }
    return taskuri;
}

Task prioritateMaxima(struct Nod* stack) {
    Task taskReturnat;
    taskReturnat.id = -1;
    taskReturnat.titlu = "";
    taskReturnat.programator = "";
    taskReturnat.estimareOre = -1;
    taskReturnat.prioritate = 0;

    unsigned int prioritateMax = 0;
    struct Nod* current = stack;

    while (current) {
        if (prioritateMax < current->task.prioritate) {
            prioritateMax = current->task.prioritate;
        }
        current = current->next;
    }
    current = stack;
    while (current) {
        if (prioritateMax == current->task.prioritate) {
            taskReturnat.id = current->task.id;
            taskReturnat.titlu = malloc(sizeof(char) * (strlen(current->task.titlu) + 1));
            strcpy(taskReturnat.titlu, current->task.titlu);
            taskReturnat.programator = malloc(sizeof(char) * (strlen(current->task.programator) + 1));
            strcpy(taskReturnat.programator, current->task.programator);
            taskReturnat.estimareOre = current->task.estimareOre;
            taskReturnat.prioritate = current->task.prioritate;
        }
        current = current->next;
    }
    return taskReturnat;
}

void dezalocareStack(struct Nod** stack) {
    struct Nod* current = *stack;

    while (current) {
        struct Nod* temp = current;
        current= current->next;
        free(temp->task.titlu);
        free(temp->task.programator);
        free(temp);
    }
    *stack = NULL;
}

int main(void) {
    printf("Hello, World!\n");
    return 0;
}
