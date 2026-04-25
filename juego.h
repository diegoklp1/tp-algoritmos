#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define MAX_LINEA 0
#define TAM_INI 0
#define TODO_OK 1
#define SIN_MEM 0
#define DUPLICADO 0

#define DADO_MIN 1
#define DADO_MAX 6


typedef struct {
    char nombre[50];
    int vidas;
    int puntos;
} Jugador;

typedef struct {
    int id;
    int posicion_actual;
    int direccion;
} Bandido;

#endif // JUEGO_H_INCLUDED
