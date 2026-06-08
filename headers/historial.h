

#ifndef HISTORIAL_H_INCLUDED
#define HISTORIAL_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define ARCHIVO_PARTIDAS "partidas.bin"
#define MAX_NOMBRE 64

// Estructura para registrar los datos solicitados
typedef struct {
    char nombreJugador[MAX_NOMBRE];
    int puntos;
    int movimientos;
    int resultado; // 1 = Victoria, 0 = Derrota
    char fecha[20]; // Formato "DD/MM/YYYY HH:MM"
} tPartida;

int guardarPartida(const char* nombre, int puntos, int movimientos, int resultado);
void mostrarHistorialJugador(const char* nombre);

#endif // HISTORIAL_H_INCLUDED
