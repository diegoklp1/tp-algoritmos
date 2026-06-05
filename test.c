// generar_datos.c

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOMBRE_JUGADOR 64
#define MAX_NOMBRE 64

#define ARCHIVO_JUGADORES "jugadores.bin"
#define ARCHIVO_INDICE    "jugadores.idx"
#define ARCHIVO_PARTIDAS "partidas.bin"

#define BANDERA_NULO 0
#define BANDERA_NODO 1

typedef struct {
    char nombre[MAX_NOMBRE_JUGADOR];
    long posJugadores;
} tRegistroIndice;

typedef struct {
    char nombreJugador[MAX_NOMBRE];
    int puntos;
    int movimientos;
    int resultado;
    char fecha[20];
} tPartida;

typedef struct {
    char nombre[MAX_NOMBRE_JUGADOR];
    int totalPuntos;
    int totalPartidas;
} tJugador;

void generarJugadores(void);
void generarIndice(void);
void generarPartidas(void);

int main(void)
{
    generarJugadores();
    generarIndice();
    generarPartidas();
    printf("Archivos generados correctamente.\n");
    return 0;
}
void generarJugadores(void)
{
    FILE* fp = fopen(ARCHIVO_JUGADORES, "wb");

    if(!fp)
    {
        perror("Error creando jugadores.bin");
        exit(EXIT_FAILURE);
    }

    tJugador jug;

    memset(&jug, 0, sizeof(jug));
    strcpy(jug.nombre, "DIEGO");
    jug.totalPuntos = 450;
    jug.totalPartidas = 3;
    fwrite(&jug, sizeof(jug), 1, fp);

    memset(&jug, 0, sizeof(jug));
    strcpy(jug.nombre, "KEVIN");
    jug.totalPuntos = 300;
    jug.totalPartidas = 2;
    fwrite(&jug, sizeof(jug), 1, fp);

    memset(&jug, 0, sizeof(jug));
    strcpy(jug.nombre, "TEST");
    jug.totalPuntos = 700;
    jug.totalPartidas = 1;
    fwrite(&jug, sizeof(jug), 1, fp);


    fclose(fp);
}
void generarPartidas(void)
{
    FILE* fp = fopen("partidas.bin", "wb");
    if (!fp) { perror("Error creando partidas.bin"); exit(EXIT_FAILURE); }

    tPartida p;

// DIEGO = 100 + 150 + 200 = 450

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"DIEGO");
p.puntos=100;
p.movimientos=12;
p.resultado=1;
strcpy(p.fecha,"01/06/2026 14:30");
fwrite(&p,sizeof(p),1,fp);

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"DIEGO");
p.puntos=150;
p.movimientos=9;
p.resultado=1;
strcpy(p.fecha,"02/06/2026 10:15");
fwrite(&p,sizeof(p),1,fp);

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"DIEGO");
p.puntos=200;
p.movimientos=15;
p.resultado=0;
strcpy(p.fecha,"03/06/2026 19:00");
fwrite(&p,sizeof(p),1,fp);


// ANA = 100 + 200 = 300

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"KEVIN");
p.puntos=100;
p.movimientos=8;
p.resultado=1;
strcpy(p.fecha,"01/06/2026 11:00");
fwrite(&p,sizeof(p),1,fp);

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"KEVIN ");
p.puntos=200;
p.movimientos=14;
p.resultado=0;
strcpy(p.fecha,"04/06/2026 16:45");
fwrite(&p,sizeof(p),1,fp);


// TEST = 700

memset(&p,0,sizeof(p));
strcpy(p.nombreJugador,"TEST");
p.puntos=700;
p.movimientos=5;
p.resultado=1;
strcpy(p.fecha,"05/06/2026 09:00");
fwrite(&p,sizeof(p),1,fp);

    fclose(fp);
}


void generarIndice(void)
{
    FILE* fp = fopen(ARCHIVO_INDICE, "wb");

    if(!fp)
    {
        perror("Error creando jugadores.idx");
        exit(EXIT_FAILURE);
    }

    unsigned char bandera;
    unsigned tamInfo = sizeof(tRegistroIndice);

    tRegistroIndice reg;

    /* RAIZ: DIEGO */

    bandera = BANDERA_NODO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    fwrite(&tamInfo, sizeof(tamInfo), 1, fp);

    memset(&reg, 0, sizeof(reg));
    strcpy(reg.nombre, "DIEGO");
    reg.posJugadores = 0;

    fwrite(&reg, sizeof(reg), 1, fp);

    /* HIJO IZQUIERDO: KEVIN */

    bandera = BANDERA_NODO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    fwrite(&tamInfo, sizeof(tamInfo), 1, fp);

    memset(&reg, 0, sizeof(reg));
    strcpy(reg.nombre, "KEVIN");
    reg.posJugadores = 1;

    fwrite(&reg, sizeof(reg), 1, fp);

    /* hijos de KEVIN */

    bandera = BANDERA_NULO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    bandera = BANDERA_NULO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    /* HIJO DERECHO: TEST */

    bandera = BANDERA_NODO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    fwrite(&tamInfo, sizeof(tamInfo), 1, fp);

    memset(&reg, 0, sizeof(reg));
    strcpy(reg.nombre, "TEST");
    reg.posJugadores = 2;

    fwrite(&reg, sizeof(reg), 1, fp);

    /* hijos de TEST */

    bandera = BANDERA_NULO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    bandera = BANDERA_NULO;
    fwrite(&bandera, sizeof(bandera), 1, fp);

    fclose(fp);
}