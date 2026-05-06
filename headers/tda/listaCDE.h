#ifndef LISTACDE_H_INCLUDED
#define LISTACDE_H_INCLUDED

#include "tda.h"
typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo *sig;
    struct sNodo *ant;
} tNodoLCDE;

typedef tNodoLCDE* tListaCD;

void crearListaCD(tListaCD *p);
int listaVaciaCD(const tListaCD *p);
void vaciarListaCD(tListaCD *p);
int listaLlenaCD(const tListaCD *p, unsigned cantBytes); // <-- es necesaria esta funcion?
int ponerAlComienzoCD(tListaCD *p, const void *d, unsigned cantBytes);
int ponerAlFinalCD(tListaCD *p, const void *d, unsigned cantBytes);
int sacarPrimeroListaCD(tListaCD *p, void *d, unsigned cantBytes);
int sacarUltimoListaCD(tListaCD *p, void *d, unsigned cantBytes);
void mostrarListaCD(const tListaCD *p, void(*mostrar)(const void*));
int buscarElementoLista(tListaCD* p, void* d, unsigned cantBytes, int(*comparar)(const void*, const void*), void* elem);
int modificarValor(tListaCD* p, void *d, unsigned cantBytes, int(*comparar)(const void*, const void*), int(*accion)(void**, unsigned*, const void*, unsigned), void* datosAModif);
void mostrarListaArchCD(const tListaCD *p, FILE* donde, void(*mostrar)(const void*, FILE*));

#endif // LISTACDE_H_INCLUDED
