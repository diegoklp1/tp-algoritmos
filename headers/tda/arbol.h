#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "tda.h"

#define CLA_DUP         0
#define SIN_MEM         0
#define SIN_INICIALIZAR 0
#define ERROR_ARCH      0
#define TODO_BIEN       1

typedef int (*Cmp)(const void*, const void*);
typedef void (*Accion)(void*, unsigned, unsigned, void*);

typedef struct sNodoArbol {
    void *info;
    unsigned tamInfo;
    struct sNodoArbol *izq;
    struct sNodoArbol *der;
} tNodoArbol;

typedef tNodoArbol *tArbolBinBusq;

void crearArbolBinBusq(tArbolBinBusq *p);
int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam, Cmp cmp);
int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam, Cmp cmp);
int eliminarRaizArbolBinBusq(tArbolBinBusq *p);
int eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned tam, Cmp cmp);
void recorrerEnOrdenArbolBinBusq(const tArbolBinBusq *p, void *params, Accion accion);
int crearDesdeArchBinArbol(FILE *arch, const tArbolBinBusq *arbol);
int cargarDesdeArchBinOrdArbol(FILE *arch, tArbolBinBusq *p, unsigned tamInfo);

#endif // ARBOL_H_INCLUDED
