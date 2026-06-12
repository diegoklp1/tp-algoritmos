#include "../headers/juego.h"
#include "../headers/tda/cola.h"
#include "../headers/mapa.h"
tNodoLCDE* destinoJugador(tNodoLCDE* posActual, int pasos, int direccion)
{
    tNodoLCDE*aux=posActual;
    NodoRuta* info;
    while(pasos>0)
    {
        info=(NodoRuta*)obtenerInfoNodo(aux);
        if(direccion==ADELANTE)
        {
            aux=siguienteNodo(aux);
            info=(NodoRuta*)obtenerInfoNodo(aux);
            if(info->terreno==SALIDA&&pasos>1)//si llegue al final y aun me quedan pasos...
            {
                direccion=ATRAS;
            }
        }
        else //si la direccion es atras.
        {
            if(info->pos==1)
                pasos=0;
            else
                aux=anteriorNodo(aux);
        }
        pasos--;
    }
    return aux;
}
tNodoLCDE* destinoBandido(tNodoLCDE* posActual,tNodoLCDE*destJugador, int pasosBandido,int casillasTotales)
{
    int direccion=calcularDireccionBandido(posActual,destJugador,casillasTotales);
    int i;
    tNodoLCDE* aux = posActual;
    for(i=0;i<pasosBandido;i++)
    {
        if(direccion==ADELANTE)
            aux=siguienteNodo(aux); //el bandido avanza para alcanzar al jugador por delante
        else
            aux=anteriorNodo(aux); //el bandido retrocede para alcanzar al jugador por atras
    }
    return aux;
}
int calcularDireccionBandido(tNodoLCDE*posActualB,tNodoLCDE*destJugador,int totalCasillas)
{
    int casillaBandido =((NodoRuta*)obtenerInfoNodo(posActualB))->pos;
    int casillaJugador =((NodoRuta*)obtenerInfoNodo(destJugador))->pos;

   int distanciaAdelante=(casillaJugador-casillaBandido+totalCasillas)%totalCasillas;//calculo de la distancia del bandido al jugador yendo por adelante
   int distanciaAtras=(casillaBandido-casillaJugador+totalCasillas)%totalCasillas; //calculo de la distancia del bandido al jugador yendo por atras
   if(distanciaAdelante<=distanciaAtras)
        return ADELANTE;
   else
        return ATRAS;

}
void planificarTurnoJugador(JugadorPartida* jugador,tCola* colaTurnos,tNodoLCDE** destinoFuturoJ,tCola *colaHistorial){
    MovimientoCola movJugador;
    MovimientoHistorial colaLogs;
    int direccion;
    int pasos;
    printf("\n--- TU TURNO  ---\n");
    printf("Presiona ENTER para tirar el dado...");
    fflush(stdin);
    getchar();
    //Tirar el dado

    pasos=generarNumeroEntre(1,6);
    printf("Sacaste un %d.\n",pasos);
    jugador->movimientos++;
    int posActual = ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->pos; // posicion actual
    int puedeRetroceder = (posActual - pasos) >= 1; // veo si puede ir para atrás
    if (puedeRetroceder) {
        do {
            printf("Hacia donde vas? (1: Adelante, 2: Atras): ");
            scanf("%d", &direccion);
        } while (direccion != ADELANTE && direccion != ATRAS);
    } else {
        printf("No puedes retroceder, avanzas hacia adelante. (ENTER)\n");
        fflush(stdin);
        getchar();
        direccion = ADELANTE;
    }

    //Calcular destino
    *destinoFuturoJ=destinoJugador(jugador->posicion_actual,pasos,direccion);

    // encolar el movimiento
    movJugador.tipo_entidad=JUGADOR;
    movJugador.entidad=jugador;
    movJugador.nodoDestino=*destinoFuturoJ;

    colaLogs.casillasMovidas=pasos;
    if (direccion == ADELANTE)
        colaLogs.tipo_movimiento = 'F';
    else
        colaLogs.tipo_movimiento = 'B';

    ponerEnCola(colaTurnos,&movJugador,sizeof(MovimientoCola));
    ponerEnCola(colaHistorial,&colaLogs,sizeof(MovimientoHistorial));
    printf("Movimiento planificado.\n");
}

void planificarTurnoBandidos(Bandido* bandidos, int cantBandidos, tNodoLCDE* destinoJugador, int totalCasillas, tCola* colaTurnos) {

    // Recorremos a todos los bandidos uno por uno
    for (int i = 0; i < cantBandidos; i++) {
        if((bandidos+i)->id!=-1)
        // El bandido tira el dado
        {
            int pasos = generarNumeroEntre(1,4);
            tNodoLCDE* destinoCalculado = destinoBandido((bandidos+i)->posicion_actual, destinoJugador, pasos, totalCasillas);

            // Creamos movimiento de bandido para la cola
            MovimientoCola movBandido;
            movBandido.tipo_entidad = BANDIDO;
            movBandido.entidad = (bandidos+i);          // Pasamos el bandido
            movBandido.nodoDestino = destinoCalculado;  // Pasamos el nodo que calculamos

            // Metemos el ticket en la cola
            ponerEnCola(colaTurnos, &movBandido, sizeof(MovimientoCola));
        }
    }
}
void actualizarPosiciones(tCola* colaTurnos) {
    MovimientoCola turnoActual;

    // Desencolamos hasta que no queden mas movimientos
    while (sacarDeCola(colaTurnos, &turnoActual, sizeof(MovimientoCola)))
    {
        if (turnoActual.tipo_entidad == JUGADOR)
        {
            JugadorPartida* j = (JugadorPartida*)turnoActual.entidad;

            // Quito al jugador de la casilla vieja
            if (j->posicion_actual != NULL)
            {
               ((NodoRuta*)obtenerInfoNodo(j->posicion_actual))->hay_jugador = false;
            }

            // Le doy su nueva posicion
            j->posicion_actual = turnoActual.nodoDestino;

            // Activo al jugador en la casilla nueva
            if (j->posicion_actual != NULL)
            {
                ((NodoRuta*)obtenerInfoNodo(j->posicion_actual))->hay_jugador = true;
            }
        }
        else if (turnoActual.tipo_entidad == BANDIDO)
        {
            Bandido* b = (Bandido*)turnoActual.entidad;

            // Resto al bandido de la casilla vieja
            if (b->posicion_actual != NULL && ((NodoRuta*)obtenerInfoNodo(b->posicion_actual))->cant_bandidos > 0)
            {
                ((NodoRuta*)obtenerInfoNodo(b->posicion_actual))->cant_bandidos--;
            }

            // Le doy nueva posicion
            b->posicion_actual = turnoActual.nodoDestino;

            // Sumo al bandido en la casilla nueva
            if (b->posicion_actual != NULL)
            {
                ((NodoRuta*)obtenerInfoNodo(b->posicion_actual))->cant_bandidos++;
            }
        }
    }
}

void verificarChoques(JugadorPartida* jugador, Bandido* bandidos, int cantBandidos, tListaCD* mapa)
{

    // Leemos en que casilla esta parado el jugador AHORA
    NodoRuta* CasillaActual = (NodoRuta*)obtenerInfoNodo(jugador->posicion_actual);
    //bool caiEnOasis=false; // lo saco ya que le saco la vieja y si cayo en un oasis se reasigna.



    // verificar choques con bandidos
    for (int i = 0; i < cantBandidos; i++)
    {

        if ((bandidos + i)->posicion_actual != NULL)   // Si el bandido esta vivo
        {

            if (jugador->posicion_actual == (bandidos + i)->posicion_actual && CasillaActual->terreno != SALIDA)
            {

                // Hubo choque, pero verificamos si esta en oasis
                if (jugador->protegido_por_oasis==true)
                {
                    printf("\nEl bandido %d te alcanzo, pero el oasis te protegio. Estas a salvo.\n",
                           (bandidos + i)->id);
                    jugador->protegido_por_oasis=false;
                }
                else
                {
                    // Logica normal de dado (fuera del oasis)
                    printf("\nCRITICO: El bandido %d te atrapo en la casilla %d.\n",(bandidos + i)->id,CasillaActual->pos);
                    jugador->vidas--;
                    ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->hay_jugador = false;
                    (bandidos + i)->posicion_actual = NULL;


                    printf("El bandido %d fue eliminado de la ruta.\n",(bandidos + i)->id);
                    ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->cant_bandidos--;
                    (bandidos+i)->id=-1;

                    if (jugador->vidas > 0)
                    {
                        jugador->posicion_actual = obtenerNodoPorPosicion(mapa, 1);
                        ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->hay_jugador = true;
                        printf("Has sido devuelto al campamento inicial (Casilla 1).\n");


                    // muevo los bandidos que esten en el inicio del tablero
                    for (int j = 0; j < cantBandidos; j++) {
                        if ((bandidos + j)->id != -1 && (bandidos + j)->posicion_actual != NULL) {
                            NodoRuta* casillaB = (NodoRuta*)obtenerInfoNodo((bandidos + j)->posicion_actual);
                            if (casillaB->pos == 1)
                            {
                                casillaB->cant_bandidos--;
                                (bandidos + j)->posicion_actual = anteriorNodo((bandidos + j)->posicion_actual);
                                ((NodoRuta*)obtenerInfoNodo((bandidos + j)->posicion_actual))->cant_bandidos++;
                                printf("El bandido %d retrocede del inicio para dar lugar al jugador.\n", (bandidos + j)->id);
                            }
                        }
                    }
                    }
                    return;
                }
            }
        }
    }

    //aca decidimos dejar que la tormenta permamenzca en la casilla por mas que haya caido el jugador
    if (CasillaActual->terreno == TORMENTA)
    {
        if (jugador->protegido_por_oasis == true)
            printf("\nCaiste en una tormenta, pero el oasis te protegio!\n"); 
        else
        {
            printf("\nCaiste en una tormenta. Pierdes tu proximo turno.\n");
            jugador->pierde_proximo_turno = true;
            CasillaActual->terreno = VACIO;
        }
    }

    if(jugador->protegido_por_oasis==true)
        jugador->protegido_por_oasis=false; //si el jugador tiene un escudo activo, y no cayo en el oasis, significa q la proteccion viene de un turno anterior, hay q desactivarla

    if(CasillaActual->terreno == OASIS)
    {
        printf("Caiste en el oasis! estaras protegido el siguiente turno!\n");
        jugador->protegido_por_oasis=true;
        CasillaActual->terreno=VACIO;
    }
    // Evaluamos los terrenos beneficiosos / perjudiciales

    if (CasillaActual->terreno == VIDA_EXTRA)
    {
        printf("\nEncontraste una vida extra.\n");
        jugador->vidas++;
        ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->terreno = VACIO;

    }
    else if (CasillaActual->terreno == PREMIO)
    {
        printf("\nRecogiste un premio.\n");
        jugador->puntos += 1;
        ((NodoRuta*)obtenerInfoNodo(jugador->posicion_actual))->terreno = VACIO;
    }
}
void mostrarHistorialMovimientos(tCola* historial) {
    MovimientoHistorial mov;
    printf("\n------------------------------------------\n");
    printf("        HISTORIAL DE MOVIMIENTOS        \n");
    printf("-------------------------------------\n");

    // sacarDeCola devuelve 1 si saco algo, 0 si la cola se vacio
    while (sacarDeCola(historial, &mov, sizeof(MovimientoHistorial))) {
        printf("%c%d\n",mov.tipo_movimiento,mov.casillasMovidas);
    }
    printf("-------------------------------------\n");
}
