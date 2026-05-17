#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED
#include "tda/listaCDE.h"
#include "juego.h"

int generarNumeroEntre(int ini, int fin);
int generarMovimiento();
void mostrarTablero(char tablero[25]);
int generarTablero(Config* c,Bandido* bands,tListaCD *lista);
void mostrarPos(const void* dato);
int testTablero(Config* c);
void visualizarYGenerarTablero();
#endif // MAPA_H_INCLUDED
