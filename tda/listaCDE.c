#include "../headers/tda/listaCDE.h"
#include "../headers/main.h"

void crearListaCD(tListaCD *p)
{
    *p = NULL;
}

int listaVaciaCD(const tListaCD *p)
{
    return *p == NULL;
}

int listaLlenaCD(const tListaCD *p, unsigned cantBytes)
{

    tNodo *aux = (tNodo*)malloc(sizeof(tNodo));
    void *info = malloc(cantBytes);
    free(aux);
    free(info);
    return aux == NULL || info == NULL;
}

void vaciarListaCD(tListaCD *p)
{
    if (*p == NULL) return;

    tNodo *act = *p;
    tNodo *ultimo = (*p)->ant;

    // pongo el sig del último en null para saber hasta donde parar.
    ultimo->sig = NULL;

    // recorro hasta que sig* del nodo en que esté, sea NULL.
    while (act) {
        tNodo *aux = act;
        act = act->sig;
        free(aux->info);
        free(aux);
    }

    *p = NULL;
}

int ponerAlComienzoCD(tListaCD *p, const void *d, unsigned cantBytes)
{

    tNodo* nue = (tNodo *)malloc(sizeof(tNodo));
    if (!nue) return 0;

    if (!(nue->info = malloc(cantBytes))) {
        free(nue);
        return 0;
    }

    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;

    if (*p == NULL) {
        nue->sig = nue;
        nue->ant = nue;
        *p = nue;
    } else {
        tNodo *ultimo = (*p)->ant;
        nue->sig = *p;         //el siguiente del nuevo es el actual primero
        nue->ant = ultimo;     //el anterior del nuevo es el último

        (*p)->ant = nue;       //el nodo que estaba le actualizo su anterior con el nuevo que puse
        ultimo->sig = nue;     //actualizo el sig del ultimo con el nuevo que puse

        *p = nue;              // actualizo el puntero al nuevo primero.
    }
    return 1;
}

int ponerAlFinalCD(tListaCD *p, const void *d, unsigned cantBytes)
{
    tNodo *nue = (tNodo *)malloc(sizeof(tNodo));
    if (!nue) return 0;

    if (!(nue->info = malloc(cantBytes))) {
        free(nue);
        return 0;
    }

    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;

    if (*p == NULL) {
        nue->sig = nue;
        nue->ant = nue;
        *p = nue;
    } else {
        tNodo *ultimo = (*p)->ant;

        //Lo mismo que para poner al inicio esta parte
        nue->sig = *p;
        nue->ant = ultimo;
        (*p)->ant = nue;
        ultimo->sig = nue;

        //y no actualizo el *p, recordá que lo tengo que apuntar al inicio siempre.
    }
    return 1;
}

int sacarPrimeroListaCD(tListaCD *p, void *d, unsigned cantBytes)
{
    if (*p == NULL) return 0;

    tNodo *aux = *p;//*p apunta al primero siempre en esta estructura
    memcpy(d, aux->info, minimo(cantBytes, aux->tamInfo));

    if (aux->sig == aux) {
        //si era el unico en la lista, queda vacia
        *p = NULL;
    } else {
        tNodo *ultimo = aux->ant;
        *p = aux->sig; // vamos a sacar el primero, entonces actualizo *p con el siguiente. pasa a ser el primero.

        // en esta parte reconecto (-1) con (1), suponiendo que tenia una lista de (-1) <-> (0) <-> (1). y (0) era el primero.
        (*p)->ant = ultimo;
        ultimo->sig = *p;
    }
    free(aux->info);
    free(aux);
    return 1;
}

int sacarUltimoListaCD(tListaCD *p, void *d, unsigned cantBytes)
{
    if (*p == NULL) return 0;
    tNodo *elim = (*p)->ant;
    memcpy(d, elim->info, minimo(cantBytes, elim->tamInfo));

    if (elim->sig == elim) {
        //si era el unico elemento, queda vacia la lista.
        *p = NULL;
    } else {
        tNodo *nuevoUltimo = elim->ant; // El nodo anterior al que vamos a borrar

        // en esta parte reconecto (-2) con (1), suponiendo que tenia una lista de (-2) <-> (-1) <-> (1). y (-1) era el ultimo.
        nuevoUltimo->sig = *p;
        (*p)->ant = nuevoUltimo;
    }
    free(elim->info);
    free(elim);
    return 1;
}

void mostrarListaCD(const tListaCD *p, void(*mostrar)(const void*))
{
    if (*p == NULL) {
        printf("Lista vacia.\n");
        return;
    }
    tNodo* act = *p;

    do {
        mostrar(act->info);
        act = act->sig;
    } while (act != *p);
}

void mostrarListaArchCD(const tListaCD *p, FILE* donde, void(*mostrar)(const void*, FILE*))
{
    if (*p == NULL) {
        printf("Lista vacia.\n");
        return;
    }
    tNodo* act = *p;

    do {
        mostrar(act->info, donde);
        act = act->sig;
    } while (act != *p);
}

int buscarElementoLista(tListaCD* p, void* d, unsigned cantBytes, int(*comparar)(const void*, const void*), void* elem) {
    if (*p == NULL)
        return 0;

    tNodo* act = *p;

    do {
        if (comparar(act->info, d) == 1) {
            memcpy(elem, act->info, minimo(cantBytes, act->tamInfo));
            return 1;
        }

        act = act->sig;
    } while (act != *p);

    return 0;
}

int modificarValor(
    tListaCD* p,
    void *d,
    unsigned cantBytes,
    int(*comparar)(const void*, const void*),
    int(*accion)(void**, unsigned*, const void*, unsigned),
    void* datosAModif
)
{
    if (*p == NULL)
        return 0;

    tNodo* act = *p;

    do {
        if (comparar(act->info, d) == 1)
            return accion(&act->info, &act->tamInfo, datosAModif, cantBytes);

        act = act->sig;
    } while (act != *p);

    return 0;
}
