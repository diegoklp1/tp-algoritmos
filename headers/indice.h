#ifndef INDICE_H_INCLUDED
#define INDICE_H_INCLUDED

#include "tda/arbol.h"

#define MAX_NOMBRE_JUGADOR  64
#define ARCHIVO_INDICE      "jugadores.idx"
#define ARCHIVO_JUGADORES   "jugadores.bin"

typedef struct {
    char nombre[MAX_NOMBRE_JUGADOR];
    long posJugadores; /* pos jugador en jugadores.bin */
} tRegistroIndice;

typedef struct {
    char nombre[MAX_NOMBRE_JUGADOR];
    int  totalPuntos;
    int  totalPartidas;
} tJugador;

void iniciarIndiceJugadores(tArbolBinBusq *indice);
int guardarIndiceJugadores(const tArbolBinBusq *indice);
int buscarJugador(const tArbolBinBusq *indice, const char *nombre, tJugador *jug);
int altaJugador(tArbolBinBusq *indice, char *nombre);
int bajaJugador(tArbolBinBusq *indice, const char *nombre);
int actualizarJugador(const tArbolBinBusq *indice, const char *nombre, int puntosNuevos);
void liberarIndiceJugadores(tArbolBinBusq *indice);
int cmpNombreJugador(const void *a, const void *b);
void mostrarRankingPorPuntos();
void loginJugador(tArbolBinBusq *,char *);

#endif // INDICE_H_INCLUDED
