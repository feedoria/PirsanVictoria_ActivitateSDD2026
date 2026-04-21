
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Moneda {
    char* taraEmitenta;
    int valoareNominala;
    float greutate;
    int anEmitere;
};
typedef struct Moneda Moneda;

// inserare in LSI
struct Nod {
    Moneda moneda;
    struct Nod* next;
};

void inserareMonedaInLista(struct Nod** lista, Moneda monedaNoua) {
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->moneda = monedaNoua;
    nou->next = *lista;
    *lista = nou;
}

void inserareMonedaLaFinalInLista(struct Nod** lista, Moneda monedaNoua) {
    struct Nod* nou = malloc(sizeof(struct Nod));
    nou->moneda = monedaNoua;
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

Moneda citireMonedaDinFisier(FILE* file) {
    char linie[256];
    fgets(linie, 256, file);
    char sep[3] = ";\n";

    Moneda m;
    char* buffer = strtok(linie, sep);
    m.taraEmitenta = malloc(sizeof(char) * (strlen(buffer)+1));
    strcpy(m.taraEmitenta, buffer);

    m.valoareNominala = atoi(strtok(NULL, sep));
    m.greutate = atof(strtok(NULL, sep));
    m.anEmitere = atoi(strtok(NULL, sep));

    return m;
}

void afisareMoneda(Moneda m) {
    printf("Tara emitenta: %s\n", m.taraEmitenta);
    printf("Valoare nominala: %d\n", m.valoareNominala);
    printf("Greutate: %.2f\n", m.greutate);
    printf("An emitere: %d\n", m.anEmitere);
}

void afisareLista(struct Nod* lista) {
    struct Nod* current = lista;
    while (current) {
        afisareMoneda(current->moneda);
        printf("\n");
        current = current->next;
    }
}

// vrea deep copy
// deep copy adica -> obiectul returnat să aibă propria memorie pentru pointerii interni
Moneda* monedaCuGreutateMaxima(struct Nod* lista) {

    float maxim = -1;

    struct Nod* current = lista;

    while (current) {

        if (maxim < current->moneda.greutate)

            maxim = current->moneda.greutate;

        current = current->next;

    }

    current = lista;

    Moneda* m = malloc(sizeof(Moneda));

    while (current) {

        if (maxim == current->moneda.greutate) {

            m->taraEmitenta = malloc(strlen(current->moneda.taraEmitenta) + 1);

            strcpy(m->taraEmitenta, current->moneda.taraEmitenta);

            m->valoareNominala = current->moneda.valoareNominala;

            m->greutate = current->moneda.greutate;

            m->anEmitere = current->moneda.anEmitere;

            return m;

        }

        current = current->next;

    }

    free(m);

    return NULL;

}

/*Moneda* monedaCuGreutateMaxima(struct Nod* lista) {
    float maxim = -1;
    struct Nod* current = lista;
    while (current) {
        if (maxim < current->moneda.greutate)
            maxim = current->moneda.greutate;
        current = current->next;
    }

    current = lista;
    Moneda* m = malloc(sizeof(Moneda));
    while (current) {
        if (maxim == current->moneda.greutate)
            *m = current->moneda;
        current = current->next;
    }

    return m;
}*/

void stergeMonedaCuGretateData(struct Nod** lista, float greutateCautata) {
    struct Nod* current = *lista;
    struct Nod* stanga = NULL;

    while (current) {
        struct Nod* dreapta = current->next;

        if (greutateCautata == current->moneda.greutate) {
            if (stanga != NULL) {
                stanga->next = dreapta;
            }
            else {
                *lista = dreapta;
            }

            free(current->moneda.taraEmitenta);
            free(current);
        }
        else {
            stanga = current;
        }

        current = dreapta;
    }
}

void dezalocareLista(struct Nod** lista) {
    struct Nod* current = *lista;

    while (current) {
        struct Nod* temp = current;
        current = current->next;
        free(temp->moneda.taraEmitenta);
        free(temp);
    }
    *lista = NULL;
}

Moneda creareMoneda(const char* tara, int valoare, float greutate, int an)
{
    Moneda m;

    m.taraEmitenta = malloc(strlen(tara) + 1);
    strcpy(m.taraEmitenta, tara);

    m.valoareNominala = valoare;
    m.greutate = greutate;
    m.anEmitere = an;

    return m;
}

int main(void) {
    // creare monede
        Moneda m1 = creareMoneda("Romania", 50, 7.5, 2005);
        Moneda m2 = creareMoneda("Franta", 20, 6.2, 1998);
        Moneda m3 = creareMoneda("Italia", 10, 5.8, 2010);
        Moneda m4 = creareMoneda("Germania", 100, 8.1, 2001);
        Moneda m5 = creareMoneda("Spania", 5, 4.9, 1995);
    // lista
    struct Nod* lista = NULL;
    inserareMonedaInLista(&lista, m1);
    inserareMonedaInLista(&lista, m2);
    inserareMonedaInLista(&lista, m3);
    inserareMonedaInLista(&lista, m4);
    inserareMonedaInLista(&lista, m5);

    afisareLista(lista);

    printf("Moneda cu greutate maxima\n");
    Moneda* mMax = monedaCuGreutateMaxima(lista);
    afisareMoneda(*mMax);
    free(mMax->taraEmitenta);
    free(mMax);

    dezalocareLista(&lista);
    return 0;
}
