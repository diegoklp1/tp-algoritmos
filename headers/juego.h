#ifndef JUEGO_H_INCLUDED
#define JUEGO_H_INCLUDED


#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "tda/listaCDE.h"
#include "tda/cola.h"
#include "indice.h"

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
#define ADELANTE 1
#define ATRAS 2
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
// EL MAPA Y LA LISTA
// =================================================================
typedef struct
{
    int pos;
    char terreno;
    bool hay_jugador;
    int cant_bandidos;
} NodoRuta;  // ESTA VA A SER LA INFO QUE VA A CONTENER CADA CASILLERO QUE CORRESPONDE AL CAMPO *INFO DE LA LISTA DOBLEMENTE ENLAZADA

// =================================================================
// ENTIDADES EN PARTIDA
// =================================================================
typedef struct {
    char nombre[50];
    int vidas;
    int puntos;
    int movimientos;
    // Estados alterados (Efectos de casillas)
    bool protegido_por_oasis;
    bool pierde_proximo_turno; // SI LE CAE UNA TORMENTA

    //El jugador sabe dónde está mediante un puntero al mapa
    tNodoLCDE* posicion_actual;  //Cambio de NodoRuta a un tNodo * ya que sino no es posible avanzar por el tablero
} JugadorPartida;

typedef struct {
    //bool activo; // Si el jugador lo pisa, muere y se pone en false // ESTO LO SACAMOS YA QUE SE EVALUA AL FINAL DE CADA TURNO, NO VALE SI EL JUGADOR SIMPLEMENTE PASÓ POR SU CASILLA
    int id;
     tNodoLCDE* posicion_actual;  //Cambio de NodoRuta a un tNodo * ya que sino no es posible avanzar por el tablero
} Bandido;

// =================================================================
// REGISTRO DE MOVIMIENTOS (Historial de la partida)
// =================================================================
typedef struct {
    char tipo_movimiento; // 'F' (Forward) o 'B' (Backward)
    unsigned casillasMovidas;//el lanzamiento ddel dado
} MovimientoHistorial;
// vamos a guardar cada movimiento en una lista (por ahora), puede haber una forma de guardar estos datos de una manera más óptima.

// =================================================================
// PERSISTENCIA Y RANKING (Para el Árbol Binario)
// =================================================================
// Separamos al jugador que está jugando del que se guarda
typedef struct {
    char nombre[50];
    int puntos_totales_historicos;
    int cantidad_movimientos_historicos;
    int partidas_jugadas;
} JugadorRegistro;

typedef struct {
    char tipo_entidad;   //JUGADOR o BANDIDO
    void* entidad;      //Puntero al JugadorPartida o al Bandido
    tNodoLCDE* nodoDestino; //El nodo exacto donde van a terminar
} MovimientoCola;
int leerConfig(Config* configuracion);
void imprimirConfig(const Config* c);
void iniciarTablero(const char* nombre);

tNodoLCDE* destinoJugador(tNodoLCDE* posActual, int pasos, int direccion);
tNodoLCDE* destinoBandido(tNodoLCDE* posActual,tNodoLCDE* destJugador ,int pasosBandido,int casillas);
tNodoLCDE* obtenerNodoPorPosicion(tListaCD* lista, int posicionBuscada);
int calcularDireccionBandido(tNodoLCDE*posActualB,tNodoLCDE*destJugador,int totalCasillas);
tNodoLCDE* obtenerNodoPorPosicion(tListaCD* lista, int posicionBuscada);

void planificarTurnoJugador(JugadorPartida* jugador,tCola* colaTurnos,tNodoLCDE** destinoFuturoJ,tCola *);
void planificarTurnoBandidos(Bandido* bandidos, int cantBandidos, tNodoLCDE* destinoJugador, int totalCasillas, tCola* colaTurnos);
void actualizarPosiciones(tCola *colaTurnos);
void verificarChoques(JugadorPartida* jugador, Bandido* bandidos, int cantBandidos, tListaCD* mapa);
void mostrarHistorialMovimientos(tCola* historial);

#endif // JUEGO_H_INCLUDED
