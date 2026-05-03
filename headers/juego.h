#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>


// REPRESENTACIÓN DE ESTADO DE CASILLERO
#define JUGADOR        'J'
#define INICIO         'I'
#define SALIDA         'S'
#define PREMIO         'P'
#define VIDA_EXTRA     'V'
#define OASIS          'O'
#define TORMENTA       'T'
#define BANDIDO        'B'
#define VACIO          '.'

#define DADO_MIN 1
#define DADO_MAX 6





// =================================================================
// CONFIGURACIÓN DEL JUEGO (Innegociable)
// =================================================================
typedef struct {
    int cantidad_posiciones;
    int vidas_inicio;
    int maximo_bandidos;
    int maximo_premios;
    int maximo_vidas_extra;
    int maximo_oasis;
    int maximo_tormentas;
} Config;

// =================================================================
// EL MAPA Y LA LISTA (Innegociable la estructura, flexible el contenido)
// =================================================================
typedef struct NodoRuta {
    int numero_posicion;  // Ej: 1, 2, 3... útil para imprimir "01:[I J]"
    char tipo_casilla[6];    // DECISIÓN: Usamos 'I', 'S', 'P', 'V', 'O', 'T', '.' 
    
} NodoRuta; // ESTA VA A SER LA INFO QUE VA A CONTENER CADA CASILLERO QUE CORRESPONDE AL CAMPO *INFO DE LA LISTA DOBLEMENTE ENLAZADA

// =================================================================
// ENTIDADES EN PARTIDA
// =================================================================
typedef struct {
    char nombre[50];
    int vidas;
    int puntos;
    
    // Estados alterados (Efectos de casillas)
    bool protegido_por_oasis;
    bool pierde_proximo_turno; // SI LE CAE UNA TORMENTA
    
    //El jugador sabe dónde está mediante un puntero al mapa
    NodoRuta* posicion_actual; 
} JugadorPartida;

typedef struct {
    //bool activo; // Si el jugador lo pisa, muere y se pone en false // ESTO LO SACAMOS YA QUE SE EVALUA AL FINAL DE CADA TURNO, NO VALE SI EL JUGADOR SIMPLEMENTE PASÓ POR SU CASILLA
    int id;
    NodoRuta* posicion_actual; // Puntero a dónde está en la lista
} Bandido;

// =================================================================
// REGISTRO DE MOVIMIENTOS (Historial de la partida)
// =================================================================
typedef struct {
    char tipo_movimiento; // 'F' (Forward) o 'B' (Backward)
    int casillas;         // Cantidad de casillas (el valor del dado) // REVISAR SI ESTE VALOR SERÁ UNSIGNED (optimizar)
} MovimientoHistorial;
// vamos a guardar cada movimiento en una lista (por ahora), puede haber una forma de guardar estos datos de una manera más óptima.

// =================================================================
// PERSISTENCIA Y RANKING (Para el Árbol Binario)
// =================================================================
// DECISIÓN: Es buena idea separar el jugador "en vivo" del que se guarda en el archivo.
typedef struct {
    char nombre[50];
    int puntos_totales_historicos;
    int cantidad_movimientos_historicos;
    int partidas_jugadas;
} JugadorRegistro;

int leerConfig(Config* configuracion);
void imprimirConfig(const Config* c);

#endif // JUEGO_H_INCLUDED
