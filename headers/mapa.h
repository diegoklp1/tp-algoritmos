#ifndef MAPA_H_INCLUDED
#define MAPA_H_INCLUDED
#include "tda/listaCDE.h"
#include "juego.h"

int generarNumeroEntre(int ini, int fin);
int generarTablero(Config* c,Bandido* bands,tListaCD *lista);
void mostrarPos(const void* dato,FILE* donde);
#endif // MAPA_H_INCLUDED
