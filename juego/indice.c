#include "../headers/indice.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


void strMayus(char *dest, const char *orig)
{
    while(*orig)
    {
        *dest = toupper((unsigned char)*orig);
        dest++;
        orig++;
    }

    *dest = '\0';
}

int cmpNombreJugador(const void *a, const void *b)
{
    tRegistroIndice *a1 = (tRegistroIndice *)a;
    tRegistroIndice *b1 = (tRegistroIndice *)b;
    return _strnicmp(a1->nombre, b1->nombre, MAX_NOMBRE_JUGADOR);
}

void iniciarIndiceJugadores(tArbolBinBusq *indice)
{
    crearArbolBinBusq(indice);

    FILE *pf = fopen(ARCHIVO_INDICE, "rb");
    if (!pf)
        return;
    fclose(pf);

    cargarIndiceArbolBinBusq(indice, ARCHIVO_INDICE);
}

int guardarIndiceJugadores(const tArbolBinBusq *indice)
{
    return guardarIndiceArbolBinBusq(indice, ARCHIVO_INDICE);
}

int buscarJugador(const tArbolBinBusq *indice, const char *nombre, tJugador *jug)
{
    tRegistroIndice clave;
    FILE *pf;

    strncpy(clave.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    clave.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    clave.posJugadores = -1;

    if (!buscarElemArbolBinBusq(indice, &clave, sizeof(tRegistroIndice), cmpNombreJugador))
        return 0;

    pf = fopen(ARCHIVO_JUGADORES, "rb");
    if (!pf) return 0;

    fseek(pf, clave.posJugadores * (long)sizeof(tJugador), SEEK_SET);
    int leido = (int)fread(jug, sizeof(tJugador), 1, pf);
    fclose(pf);

    return leido == 1;
}

int altaJugador(tArbolBinBusq *indice, char *nombre)
{
    tRegistroIndice reg;
    tJugador nuevoJug;
    FILE *pf;
    long cantReg; /* Donde voy a ponerlo en el archivo*/

    strMayus(nombre, nombre);

    strncpy(reg.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    reg.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    reg.posJugadores = -1;

    tRegistroIndice clavePrueba;
    strncpy(clavePrueba.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    clavePrueba.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    clavePrueba.posJugadores = -1;
    if (buscarElemArbolBinBusq(indice, &clavePrueba, sizeof(tRegistroIndice), cmpNombreJugador))
        return CLA_DUP;

    pf = fopen(ARCHIVO_JUGADORES, "ab+");
    if (!pf) return ERROR_ARCH;

    fseek(pf, 0L, SEEK_END);
    cantReg = ftell(pf) / (long)sizeof(tJugador);

    strncpy(nuevoJug.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    nuevoJug.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    nuevoJug.totalPuntos   = 0;
    nuevoJug.totalPartidas = 0;
    fwrite(&nuevoJug, sizeof(tJugador), 1, pf);
    fclose(pf);

    reg.posJugadores = cantReg;

    return insertarArbolBinBusq(indice, &reg, sizeof(tRegistroIndice), cmpNombreJugador);
}

int bajaJugador(tArbolBinBusq *indice, const char *nombre)
{
    tRegistroIndice clave;
    tJugador        jugBorrado; /* jugador marcado como eliminado */
    FILE           *pf;

    strncpy(clave.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    clave.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    clave.posJugadores = -1;

    if (!buscarElemArbolBinBusq(indice, &clave, sizeof(tRegistroIndice), cmpNombreJugador))
        return 0;

    /* Marca el registro en el .bin con nombre vacío para señalarlo como eliminado */
    pf = fopen(ARCHIVO_JUGADORES, "r+b");
    if (!pf) return ERROR_ARCH;

    fseek(pf, clave.posJugadores * (long)sizeof(tJugador), SEEK_SET);
    if (fread(&jugBorrado, sizeof(tJugador), 1, pf) == 1) {
        memset(jugBorrado.nombre, 0, MAX_NOMBRE_JUGADOR);
        fseek(pf, clave.posJugadores * (long)sizeof(tJugador), SEEK_SET);
        fwrite(&jugBorrado, sizeof(tJugador), 1, pf);
    }
    fclose(pf);

    return eliminarElemArbolBinBusq(indice, &clave, sizeof(tRegistroIndice), cmpNombreJugador);
}

int actualizarJugador(const tArbolBinBusq *indice, const char *nombre, int puntosNuevos)
{
    tRegistroIndice clave;
    tJugador        jug;
    FILE           *pf;

    strncpy(clave.nombre, nombre, MAX_NOMBRE_JUGADOR - 1);
    clave.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
    clave.posJugadores = -1;

    if (!buscarElemArbolBinBusq(indice, &clave, sizeof(tRegistroIndice), cmpNombreJugador))
        return 0;

    pf = fopen(ARCHIVO_JUGADORES, "r+b");
    if (!pf) return ERROR_ARCH;

    long offset = clave.posJugadores * (long)sizeof(tJugador);

    fseek(pf, offset, SEEK_SET);
    if (fread(&jug, sizeof(tJugador), 1, pf) != 1) { fclose(pf); return ERROR_ARCH; }

    jug.totalPuntos   += puntosNuevos;
    jug.totalPartidas += 1;

    fseek(pf, offset, SEEK_SET);
    fwrite(&jug, sizeof(tJugador), 1, pf);
    fclose(pf);

    return TODO_BIEN;
}

void liberarIndiceJugadores(tArbolBinBusq *indice)
{
    while (*indice)
        eliminarRaizArbolBinBusq(indice);
}

static void _imprimirRegistro(void *info, unsigned tamInfo, unsigned nivel, void *params)
{
    (void)tamInfo; (void)nivel; (void)params;
    tRegistroIndice *reg = (tRegistroIndice *)info;
    tJugador         jug;
    FILE            *pf;

    pf = fopen(ARCHIVO_JUGADORES, "rb");
    if (!pf) { printf("  %-30s  [sin datos]\n", reg->nombre); return; }

    fseek(pf, reg->posJugadores * (long)sizeof(tJugador), SEEK_SET);
    if (fread(&jug, sizeof(tJugador), 1, pf) == 1)
        printf("  %-30s  Puntos: %4d  Partidas: %3d\n",
               jug.nombre, jug.totalPuntos, jug.totalPartidas);
    fclose(pf);
}

void mostrarRankingAlfabetico(const tArbolBinBusq *indice)
{
    printf("\n=== RANKING (orden alfabetico) ===\n");
    recorrerEnOrdenArbolBinBusq(indice, NULL, _imprimirRegistro);
    printf("==================================\n\n");
}
