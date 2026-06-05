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


void mostrarPos(const void* dato, FILE* donde) {
    NodoRuta* pos = (NodoRuta*)dato;
    if(pos == NULL) return;

    //ImprimO el numero (2 digitos), el terreno y un espacio
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
        for (int i = 0; i < pos->cant_bandidos; i++) {
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

/*int compararTipo(const void* a, const void* b) {
    NodoRuta posA = *(NodoRuta*)a;
    NodoRuta posB = *(NodoRuta*)b;
    return (posA.caracter == posB.caracter) ? 1 : 0;
}*/

/*int accionTipo(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    NodoRuta* posA = *(NodoRuta**)elemAModif;
    NodoRuta* posB = (NodoRuta*)elem;
    posA->caracter = posB->caracter;
    return*/
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

int accionMovimiento(void** elemAModif, unsigned* tamInfoAModif, const void* elem, unsigned tamInfo) {
    (void)tamInfoAModif;
    (void)tamInfo;

    NodoRuta* posA = *(NodoRuta**)elemAModif;
    NodoRuta* posB = (NodoRuta*)elem;
    posA->pos = posB->pos;
    return 1;
}

void agregarTerreno(tListaCD* lista, char tipoTerreno, int cantidadMaxima, int numeroEspacios) {
    for(int i = 0; i < cantidadMaxima; i++) {
        int numero;
        NodoRuta pos;
        //Hay que verificar que el terreno este vacio
        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            NodoRuta buscado = {numero, VACIO, false,0};

            if(!buscarElementoLista(lista, &buscado, sizeof(NodoRuta), compararPosicion, &pos))
                return;

        } while(pos.terreno != VACIO || (pos.hay_jugador == true || pos.cant_bandidos > 0));
        //Verificar si puede haber terreno y bandido en el mismo lugar
        NodoRuta nuevo = {numero, tipoTerreno, false,0};
        modificarValor(lista, &pos, sizeof(NodoRuta), compararPosicion, accionTerreno, &nuevo);
    }
}

void agregarPersonaje(tListaCD* lista, char elemento, int cantidadMaxima, int numeroEspacios,Bandido* arrayBandidos) {
    for(int i = 0; i < cantidadMaxima; i++) {
        int numero;
        NodoRuta pos;

        do {
            numero = generarNumeroEntre(1, numeroEspacios-1);
            NodoRuta buscado = {numero, VACIO, false,0};

            if(!buscarElementoLista(lista, &buscado, sizeof(NodoRuta), compararPosicion, &pos))
            return;

        } while(pos.terreno != VACIO || pos.hay_jugador == true || pos.cant_bandidos > 0);
         //Verificar si puede haber terreno y bandido en el mismo lugar

        NodoRuta nuevo = {numero, VACIO, false, 1};
        modificarValor(lista, &pos, sizeof(NodoRuta), compararPosicion, accionAgregarBandido, &nuevo);
        //Inicializamos al bandido, le damos su id y su posicion dentro del tablero
        (arrayBandidos + i)->id = i + 1;
        (arrayBandidos + i)->posicion_actual = obtenerNodoPorPosicion(lista, numero);
    }
}

void mostrarJugador(NodoRuta jugador) {
    printf("El jugador es %c %d\n", jugador.terreno, jugador.pos);
}

//

/*void movimientoJugador(tListaCD* lista, short cantidad) {
    //Chequear movimiento circular
    // int mov = generarMovimiento();
    NodoRuta jugadorNodo = {0, VACIO, JUGADOR}; //Aca guardo la posicion original del jugador
    //Busco la posicion original del jugador
    buscarElementoLista(lista, &jugadorNodo, sizeof(NodoRuta), compararTipo, &jugadorNodo);
    NodoRuta nuevaPos = {jugadorNodo.pos + cantidad, VACIO, JUGADOR}; //Nuevo elemento con la nuevo posicion
    //Verifico que las posiciones sean las mismas y cambio el terreno de la posicion 6
    modificarValor(lista, &nuevaPos, sizeof(NodoRuta), compararPosicion, accionTipo, &jugadorNodo);
    NodoRuta nuevoVacio = {jugadorNodo.pos, VACIO, VACIO};
    //Pongo vacio el valor de la posicion del jugador
    modificarValor(lista, &jugadorNodo, sizeof(NodoRuta), compararPosicion, accionTipo, &nuevoVacio);
}*/
tNodoLCDE* obtenerNodoPorPosicion(tListaCD* lista, int posicionBuscada) {
    if (*lista == NULL)
        {
        return NULL;
        }

    tNodoLCDE* aux = *lista;
    if (((NodoRuta*)aux->info)->pos == posicionBuscada)
        {
        return aux;
        }
        aux=aux->sig; //si no es la primera posicion, buscamos el resto
    while (aux != *lista)
    {
        NodoRuta* info = (NodoRuta*)aux->info;
        if (info->pos == posicionBuscada)
        {
            return aux; //Encontramos el nodo
        }
        aux = aux->sig;
    }

    return NULL;
}
int generarTablero(Config* c,Bandido* arrayBandidos,tListaCD *lista)
{
   FILE* tableroArch = fopen(NOMBRE_ARCHIVO_TABLERO, "w+t");
   if(!tableroArch)
   {
       return ERROR;
   }

   if (c->maximo_bandidos >= c->cantidad_posiciones - 2) 
   {
        printf("Error: demasiados bandidos para garantizar solucion posible.\n");
        return ERROR;
   }
   
    NodoRuta t = {1, VACIO, true,0};
    ponerAlComienzoCD(lista, &t, sizeof(NodoRuta));
    for(int i=2; i<c->cantidad_posiciones; i++)
    {
        NodoRuta t = {i, VACIO, false,0};
        ponerAlFinalCD(lista, &t, sizeof(NodoRuta));
    }
    NodoRuta f = {c->cantidad_posiciones, SALIDA, false,0};
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

// VISUALIZACIÓN Y GENERACIÓN DEL TABLERO

void visualizarYGenerarTablero() {
    const char* tableroPrueba[] = { //harcodeado 25
        "[I J]",
        ".",
        "P",
        "T",
        ".",
        "B",
        "O",
        ".",
        "P",
        ".",
        "V",
        ".",
        "S",
        "T",
        ".",
        "O",
        "B",
        ".",
        "P",
        ".",
        "T",
        ".",
        ".",
        ".",
        "."
    };
    int cantidad_posiciones = 25,i;
    FILE *archTablero;
    printf("\n========================================\n"); // esto va cuando se pone "nueva partda"
    printf("       ESTADO ACTUAL DEL TABLERO        \n");
    printf("========================================\n");

    for (i=0; i < cantidad_posiciones; i++) {
        printf("%s\n", tableroPrueba[i]);
    }
    archTablero = fopen("caravana.txt", "wt");
    if(!archTablero)
    {
        printf("no se pudo abrir el archivo");
        exit(1);
    }

        for (int i = 0; i < cantidad_posiciones; i++) {
            fprintf(archTablero, "%02d:%s\n", i + 1, tableroPrueba[i]);
        }
        fclose(archTablero);
}

