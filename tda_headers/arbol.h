#ifndef ARBOL_H_INCLUDED
#define ARBOL_H_INCLUDED

#include "../tda.h"

#define reservarMemoriaNodo(X, Y, Z, W) ( \
                                         ( ( X ) = (typeof( X ))malloc( Y ) ) == NULL || \
                                         ( ( Z ) = malloc( W ) ) == NULL ? \
                                         free( X ), 0 : 1 )

typedef struct sNodoArbol
{
    void* info;
    unsigned tamInfo;
    struct sNodoArbol *izq, *der;
} tNodoArbol;

typedef tNodoArbol* tArbolBinBusq;

int insertarArbolBinBusq(tArbolBinBusq* p, const void* d, unsigned tam, int (*cmp)(const void*, const void*));


#endif // ARBOL_H_INCLUDED