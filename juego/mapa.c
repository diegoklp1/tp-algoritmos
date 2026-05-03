#include "../headers/juego.h"
#include "../headers/main.h" //ver si así está bien(que solicite ambos .h o separar en otro .h)
#include "../headers/tda/listaCDE.h"

int generarNumeroEntre(int ini, int fin)
{
    int r = rand();
    return (r % fin)+ini;
}

int generarMovimiento()
{
    return generarNumeroEntre(1, 6);
}

typedef struct
{
    int pos;
    char terreno;
    char caracter;
} nodoPosicion;

void mostrarPos(const void* dato, FILE* donde) {
    nodoPosicion* pos = (nodoPosicion*)dato;
    if(pos == NULL) {
        printf("");
    } else {
    fprintf(donde, "%02d %c %c\n", pos->pos, pos->terreno, pos->caracter);
    }
}

int compararPosicion(const void* a, const void* b) {
    nodoPosicion posA = *(nodoPosicion*)a;
    nodoPosicion posB = *(nodoPosicion*)b;
    return posA.pos - posB.pos;
}

int compararTipo(const void* a, const void* b) {
    nodoPosicion posA = *(nodoPosicion*)a;
    nodoPosicion posB = *(nodoPosicion*)b;
    return posA.caracter - posB.caracter;
}

int accionTipo(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    nodoPosicion* posA = *(nodoPosicion**)elemAModif;
    nodoPosicion* posB = (nodoPosicion*)elem;
    posA->caracter = posB->caracter;
    return 1;
}

int accionTerreno(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    nodoPosicion* posA = *(nodoPosicion**)elemAModif;
    nodoPosicion* posB = (nodoPosicion*)elem;
    posA->terreno = posB->terreno;
    return 1;
}

int accionMovimiento(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    nodoPosicion* posA = *(nodoPosicion**)elemAModif;
    nodoPosicion* posB = (nodoPosicion*)elem;
    posA->pos = posB->pos;
    return 1;
}

void agregarTerreno(tListaCD* lista, char tipoTerreno, int cantidadMaxima, int numeroEspacios) {
    for(int i = 0; i < cantidadMaxima; i++) {
        int numero;
        nodoPosicion pos;
        //Hay que verificar que el terreno este vacio
        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            nodoPosicion buscado = {numero, VACIO, VACIO};

            if(!buscarElementoLista(lista, &buscado, sizeof(nodoPosicion), compararPosicion, &pos))
                return;

        } while(pos.terreno != VACIO || (pos.caracter == JUGADOR || pos.caracter == BANDIDO));
        //Verificar si puede haber terreno y bandido en el mismo lugar
        nodoPosicion nuevo = {numero, tipoTerreno, VACIO};
        modificarValor(lista, &pos, sizeof(nodoPosicion), compararPosicion, accionTerreno, &nuevo);
    }
}

void agregarPersonaje(tListaCD* lista, char elemento, int cantidadMaxima, int numeroEspacios) {
    for(int i = 0; i < cantidadMaxima; i++) {
        int numero;
        nodoPosicion pos;

        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            nodoPosicion buscado = {numero, VACIO, VACIO};

            if(!buscarElementoLista(lista, &buscado, sizeof(nodoPosicion), compararPosicion, &pos))
            return;

        } while(pos.caracter != VACIO && pos.terreno != VACIO);
        //Verificar si puede haber terreno y bandido en el mismo lugar

        nodoPosicion nuevo = {numero, VACIO, elemento};
        modificarValor(lista, &pos, sizeof(nodoPosicion), compararPosicion, accionTipo, &nuevo);
    }
}

void mostrarJugador(nodoPosicion jugador) {
    printf("El jugador es %c %d\n", jugador.terreno, jugador.pos);
}

//
void movimientoJugador(tListaCD* lista, short cantidad) {
    //Chequear movimiento circular
    // int mov = generarMovimiento();
    nodoPosicion jugadorNodo = {0, VACIO, JUGADOR}; //Aca guardo la posicion original del jugador
    //Busco la posicion original del jugador
    buscarElementoLista(lista, &jugadorNodo, sizeof(nodoPosicion), compararTipo, &jugadorNodo);
    nodoPosicion nuevaPos = {jugadorNodo.pos + cantidad, VACIO, JUGADOR}; //Nuevo elemento con la nuevo posicion
    //Verifico que las posiciones sean las mismas y cambio el terreno de la posicion 6
    modificarValor(lista, &nuevaPos, sizeof(nodoPosicion), compararPosicion, accionTipo, &jugadorNodo);
    nodoPosicion nuevoVacio = {jugadorNodo.pos, VACIO, VACIO};
    //Pongo vacio el valor de la posicion del jugador
    modificarValor(lista, &jugadorNodo, sizeof(nodoPosicion), compararPosicion, accionTipo, &nuevoVacio);
}

int generarTablero(Config* c)
{
   FILE* tableroArch = fopen(NOMBRE_ARCHIVO, "w+t");
   if(!tableroArch)
   {
       return ERROR;
   }
    tListaCD lista;
    crearListaCD(&lista);
    nodoPosicion t = {1, VACIO, JUGADOR};
    ponerAlComienzoCD(&lista, &t, sizeof(nodoPosicion));
    for(int i=2; i<c->cantidad_posiciones; i++)
    {
        nodoPosicion t = {i, VACIO, VACIO};
        ponerAlFinalCD(&lista, &t, sizeof(nodoPosicion));
    }
    nodoPosicion f = {c->cantidad_posiciones, SALIDA, VACIO};
    ponerAlFinalCD(&lista, &f, sizeof(nodoPosicion));

    agregarPersonaje(&lista, BANDIDO, c->maximo_bandidos, c->cantidad_posiciones);
    agregarTerreno(&lista, VIDA_EXTRA, c->maximo_vidas_extra, c->cantidad_posiciones);
    agregarTerreno(&lista, PREMIO, c->maximo_premios, c->cantidad_posiciones);
    agregarTerreno(&lista, TORMENTA, c->maximo_tormentas, c->cantidad_posiciones);
    agregarTerreno(&lista, OASIS, c->maximo_oasis, c->cantidad_posiciones);
    movimientoJugador(&lista, 5);
    mostrarListaArchCD(&lista, stdout, mostrarPos);
    vaciarListaCD(&lista);
    fclose(tableroArch);
}
