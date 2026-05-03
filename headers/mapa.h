#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED

#include "juego.h"

int generarNumeroEntre(int ini, int fin);
int generarMovimiento();
void mostrarTablero(char tablero[25]);
int generarTablero(Config* c);
void mostrarPos(const void* dato);
int testTablero(Config* c);

#endif // MAPA_H_INCLUDED
