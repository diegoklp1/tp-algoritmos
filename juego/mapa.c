#include "../headers/juego.h"
#include "../headers/main.h"
#include "../headers/tda/listaCDE.h"

int generarNumeroEntre(int ini, int fin)
{
    int r = rand();
    return (r % fin)+ini;
}

void mostrarPos(const void* dato, FILE* donde) {
    NodoRuta* pos = (NodoRuta*)dato;
    int i;
    if(pos == NULL) return;

    //Imprimo el numero (2 digitos), el terreno y un espacio
    fprintf(donde, "%02d %c ", pos->pos, pos->terreno);

    // Si la casilla esta sin bandidos ni jugador, imprime un punto
    if (!pos->hay_jugador && pos->cant_bandidos == 0) {
        fprintf(donde, ".");
    } else {
        // Si hay jugador, imprime la J
        if (pos->hay_jugador) {
            fprintf(donde, "J ");
        }
        // Si hay bandidos, imprime tantas B como haya
        for (i = 0; i < pos->cant_bandidos; i++) {
            fprintf(donde, "B ");
        }
    }

    // Salto de linea al final de la casilla
    fprintf(donde, "\n");
}

int compararPosicion(const void* a, const void* b) {
    NodoRuta posA = *(NodoRuta*)a;
    NodoRuta posB = *(NodoRuta*)b;
    return (posA.pos == posB.pos) ? 1 : 0;
}

int accionAgregarBandido(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {


    NodoRuta* posA = *(NodoRuta**)elemAModif;
    posA->cant_bandidos++;
    return 1;
}

int accionTerreno(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    NodoRuta* posA = *(NodoRuta**)elemAModif;
    NodoRuta* posB = (NodoRuta*)elem;
    posA->terreno = posB->terreno;
    return 1;
}

void agregarTerreno(tListaCD* lista, char tipoTerreno, int cantidadMaxima, int numeroEspacios) {
    int i;
    int numero;
    NodoRuta pos;
    NodoRuta buscado;
    NodoRuta nuevo;

    for(i = 0; i < cantidadMaxima; i++) {
        //Hay que verificar que el terreno este vacio
        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            buscado.pos = numero;
            buscado.terreno = VACIO;
            buscado.hay_jugador = false;
            buscado.cant_bandidos = 0;

            if(!buscarElementoLista(lista, &buscado, sizeof(NodoRuta), compararPosicion, &pos))
                return;

        } while(pos.terreno != VACIO || (pos.hay_jugador == true || pos.cant_bandidos > 0));
        //Verificar si puede haber terreno y bandido en el mismo lugar
        nuevo.pos = numero;
        nuevo.terreno = tipoTerreno;
        nuevo.hay_jugador = false;
        nuevo.cant_bandidos = 0;
        modificarValor(lista, &pos, sizeof(NodoRuta), compararPosicion, accionTerreno, &nuevo);
    }
}

void agregarPersonaje(tListaCD* lista, char elemento, int cantidadMaxima, int numeroEspacios,Bandido* arrayBandidos) {
    int i;
    int numero;
    NodoRuta pos;
    NodoRuta buscado;


    for(i = 0; i < cantidadMaxima; i++) {
        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            buscado.pos = numero;
            buscado.terreno = VACIO;
            buscado.hay_jugador = false;
            buscado.cant_bandidos = 0;

            if(!buscarElementoLista(lista, &buscado, sizeof(NodoRuta), compararPosicion, &pos))
            return;

        } while(pos.terreno != VACIO || pos.hay_jugador == true || pos.cant_bandidos > 0);
         //Verificar si puede haber terreno y bandido en el mismo lugar


        modificarValor(lista, &pos, sizeof(NodoRuta), compararPosicion, accionAgregarBandido, NULL);
        //Inicializamos al bandido, le damos su id y su posicion dentro del tablero
        (arrayBandidos + i)->id = i + 1;
        (arrayBandidos + i)->posicion_actual = obtenerNodoPorPosicion(lista, numero);
    }
}

tNodoLCDE* obtenerNodoPorPosicion(tListaCD* lista, int posicionBuscada) {
   NodoRuta buscado;
    buscado.pos = posicionBuscada;

    //le pasamos la posicion q buscamos a nuestra funcion generica del tda lista
    return buscarNodo(lista, &buscado, compararPosicion);
}

int generarTablero(Config* c,Bandido* arrayBandidos,tListaCD *lista)
{
   FILE* tableroArch;
   NodoRuta t;
   NodoRuta f;
   int i;

   tableroArch = fopen(NOMBRE_ARCHIVO_TABLERO, "w+t");
   if(!tableroArch)
   {
       return ERROR;
   }

   if (c->maximo_bandidos >= c->cantidad_posiciones - 2)
   {
        printf("Error: demasiados bandidos para garantizar solucion posible.\n");
        return ERROR;
   }

    t.pos = 1;
    t.terreno = INICIO;
    t.hay_jugador = true;
    t.cant_bandidos = 0;
    ponerAlComienzoCD(lista, &t, sizeof(NodoRuta));
    for(i=2; i<c->cantidad_posiciones; i++)
    {
        t.pos = i;
        t.terreno = VACIO;
        t.hay_jugador = false;
        t.cant_bandidos = 0;
        ponerAlFinalCD(lista, &t, sizeof(NodoRuta));
    }
    f.pos = c->cantidad_posiciones;
    f.terreno = SALIDA;
    f.hay_jugador = false;
    f.cant_bandidos = 0;
    ponerAlFinalCD(lista, &f, sizeof(NodoRuta));

    agregarPersonaje(lista, BANDIDO, c->maximo_bandidos, c->cantidad_posiciones,arrayBandidos);
    agregarTerreno(lista, VIDA_EXTRA, c->maximo_vidas_extra, c->cantidad_posiciones);
    agregarTerreno(lista, PREMIO, c->maximo_premios, c->cantidad_posiciones);
    agregarTerreno(lista, TORMENTA, c->maximo_tormentas, c->cantidad_posiciones);
    agregarTerreno(lista, OASIS, c->maximo_oasis, c->cantidad_posiciones);
    //movimientoJugador(lista, 5);
    mostrarListaArchCD(lista, tableroArch, mostrarPos);
    mostrarListaArchCD(lista, stdout, mostrarPos);
    //vaciarListaCD(&lista);
    fclose(tableroArch);
    return 1;
}
void inicializarJugador(JugadorPartida *j, const char *nombreJugador, int vidasInicio, tListaCD *mapa)
{
    strncpy(j->nombre, nombreJugador, sizeof(j->nombre) - 1);
    j->nombre[sizeof(j->nombre) - 1] = '\0';

    j->vidas = vidasInicio;
    j->posicion_actual = obtenerPrimeroLista(mapa);
    j->puntos = 0;
    j->movimientos = 0;
    j->protegido_por_oasis = false;
    j->pierde_proximo_turno = false;
}
