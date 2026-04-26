#ifndef TDA_H_INCLUDED
#define TDA_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define minimo(a,b) (((a) <= (b)) ? (a) : (b))

#define reservarMemoriaNodo(X, Y, Z, W) ( \
                                         ( ( X ) = (typeof( X ))malloc( Y ) ) == NULL || \
                                         ( ( Z ) = malloc( W ) ) == NULL ? \
                                         free( X ), 0 : 1 )




#endif // TDA_H_INCLUDED
