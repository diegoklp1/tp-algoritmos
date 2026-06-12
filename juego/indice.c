#include "../headers/indice.h"
#include "../headers/menu.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int cmpNombreJugador(const void *a, const void *b)
{
    tRegistroIndice *a1 = (tRegistroIndice *)a;
    tRegistroIndice *b1 = (tRegistroIndice *)b;
    return strncmp(a1->nombre, b1->nombre, MAX_NOMBRE_JUGADOR);
}

static void _construirIndiceDesdeJugadores(tArbolBinBusq *indice)
{
    FILE *pf = fopen(ARCHIVO_JUGADORES, "rb");
    if (!pf) return;

    tJugador jug;
    tRegistroIndice reg;
    long pos = 0;

    while (fread(&jug, sizeof(tJugador), 1, pf) == 1) {
        if (jug.nombre[0] != '\0') {
            strncpy(reg.nombre, jug.nombre, MAX_NOMBRE_JUGADOR - 1);
            reg.nombre[MAX_NOMBRE_JUGADOR - 1] = '\0';
            reg.posJugadores = pos;
            insertarArbolBinBusq(indice, &reg, sizeof(tRegistroIndice), cmpNombreJugador);
        }
        pos++;
    }
    fclose(pf);
}

void iniciarIndiceJugadores(tArbolBinBusq *indice)
{
    crearArbolBinBusq(indice);

    FILE *pf = fopen(ARCHIVO_INDICE, "rb");
    if (!pf) {
        _construirIndiceDesdeJugadores(indice);
        return;
    }

    cargarDesdeArchBinOrdArbol(pf, indice, sizeof(tRegistroIndice));
    fclose(pf);
}

int guardarIndiceJugadores(const tArbolBinBusq *indice)
{
    FILE *pf = fopen(ARCHIVO_INDICE, "wb");
    if (!pf) return ERROR_ARCH;
    int r = crearDesdeArchBinArbol(pf, indice);
    fclose(pf);
    return r;
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

    //strMayus(nombre, nombre);

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

static int cmpPuntosDesc(const void *a1, const void *b1)
{
    const tJugador *a = (const tJugador *)a1;
    const tJugador *b = (const tJugador *)b1;
    return b->totalPuntos - a->totalPuntos;
}

void mostrarRankingPorPuntos()
{
    FILE *fp = fopen(ARCHIVO_JUGADORES, "rb");
    if (!fp) {
        printf("No hay jugadores registrados.\n");
        return;
    }
    fseek(fp, 0L, SEEK_END);
    int cant = ftell(fp) /sizeof(tJugador);
    rewind(fp);

    if (cant == 0) {
        fclose(fp);
        printf("No hay jugadores registrados.\n");
        return;
    }
    tJugador* vec = malloc(cant * sizeof(tJugador));
    if (!vec)
    {
        fclose(fp);
        return;
    }
    fread(vec, sizeof(tJugador), cant, fp);
    qsort(vec, cant, sizeof(tJugador), cmpPuntosDesc);


    printf("\n");
    printf("==========================================================\n");
    printf("                    RANKING DE JUGADORES\n");
    printf("==========================================================\n");
    printf("| %-4s | %-25s | %-8s | %-8s |\n",
        "POS", "JUGADOR", "PUNTOS", "PARTIDAS");
    printf("----------------------------------------------------------\n");

    int posicion = 1;
    for (unsigned i = 0; i < cant; i++)
    {
        if (vec[i].nombre[0] != '\0')
        {
            printf("| %-4d | %-25s | %8d | %8d |\n",posicion, vec[i].nombre, vec[i].totalPuntos, vec[i].totalPartidas);
            posicion++;
        }
    }
    printf("==========================================================\n\n");
    fclose(fp);
    free(vec);
}
void loginJugador(tArbolBinBusq *indice, char* nombreFinal) {
    tJugador jug;
    int nombreConfirmado = 0;
    char respuesta;
    int opcion;


    //Pedimos el nombre
    ingresarYValidarNombre(nombreFinal);

    while (!nombreConfirmado) {
        // Buscamos si el nombre ingresado ya existe en el arbol
        if (buscarJugador(indice, nombreFinal, &jug)) {
            printf("\nSe encontro un usuario con ese nombre. ¿Eres tu? (y/n): ");
            scanf(" %c", &respuesta);
            while(getchar() != '\n'); // Limpiamos el buffer

            if (respuesta == 'y' || respuesta == 'Y') {
                printf("\nBienvenido nuevamente, %s!\n", nombreFinal);
                nombreConfirmado = 1; //salimos del bucle
            } else {
                printf("\nEl nombre '%s' ya pertenece a otra persona.\n", nombreFinal);
                printf("1. Generar un nombre aleatorio a partir del escrito\n");
                printf("2. Deseo usar otro nombre de usuario\n");
                printf("Seleccione una opcion: ");


                if (scanf("%d", &opcion) != 1)
                     opcion = 2;
                while(getchar() != '\n'); // Limpiamos el buffer
                if (opcion == 1) {
                    //Generamos un numero aleatorio de 3 digitos (entre 100 y 999)
                    int numAleatorio = (rand() % 900) + 100;
                    char nombreBase[21];
                    strcpy(nombreBase, nombreFinal);
                    snprintf(nombreFinal, 21, "%.16s%d", nombreBase, numAleatorio);

                    // vuelve al while y se chequea este nombre
                } else {
                    printf("\n");
                    // Pide un nombre totalmente nuevo y el while vuelve a empezar
                    ingresarYValidarNombre(nombreFinal);
                }
            }
        } else {
            // Si la busqueda da falso, el nombre esta libre para usarse
            printf("\nJugador nuevo detectado. Procediendo al alta...\n");
            altaJugador(indice, nombreFinal);
            printf("Jugador '%s' registrado con exito.\n", nombreFinal);
            nombreConfirmado = 1; // Salimos del bucle
        }
    }
}
