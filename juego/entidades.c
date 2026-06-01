#include "../headers/juego.h"
#include "../headers/tda/cola.h"
#include "../headers/mapa.h"
tNodoLCDE* destinoJugador(tNodoLCDE* posActual, int pasos, int direccion)
{
    tNodoLCDE*aux=posActual;
    NodoRuta* info;
    while(pasos>0)
    {
        info=(NodoRuta*)aux->info;
        if(direccion==ADELANTE)
        {
            aux=aux->sig;
            info=(NodoRuta*)aux->info;
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
                aux=aux->ant;
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
            aux=aux->sig;//el bandido avanza para alcanzar al jugador por delante
        else
            aux=aux->ant;//el bandido retrocede para alcanzar al jugador por atras
    }
    return aux;
}
int calcularDireccionBandido(tNodoLCDE*posActualB,tNodoLCDE*destJugador,int totalCasillas)
{
    int casillaBandido =((NodoRuta*)posActualB->info)->pos;
    int casillaJugador =((NodoRuta*)destJugador->info)->pos;

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
    //Pedir direccion
    do{
        printf("Hacia donde vas? (1: Adelante, 2: Atras): ");
        scanf("%d",&direccion);
    }while(direccion!=ADELANTE&&direccion!=ATRAS);

    //Calcular destino
    *destinoFuturoJ=destinoJugador(jugador->posicion_actual,pasos,direccion);

    // encolar el movimiento

    movJugador.tipo_entidad=JUGADOR;
    movJugador.entidad=jugador;
    movJugador.nodoDestino=*destinoFuturoJ;


    colaLogs.casillasMovidas=pasos;
    if (direccion == ADELANTE)
    {
        colaLogs.tipo_movimiento = 'F';
    }
    else
    {
        colaLogs.tipo_movimiento = 'B';
    }

    colaLogs.casillaDestino = ((NodoRuta*)(*destinoFuturoJ)->info)->pos;


    ponerEnCola(colaTurnos,&movJugador,sizeof(MovimientoCola));
    ponerEnCola(colaHistorial,&colaLogs,sizeof(MovimientoHistorial));
    printf("Movimiento planificado.\n");
}

void planificarTurnoBandidos(Bandido* bandidos, int cantBandidos, tNodoLCDE* destinoJugador, int totalCasillas, tCola* colaTurnos) {

    // Recorremos a todos los bandidos uno por uno
    for (int i = 0; i < cantBandidos; i++) {
        if((bandidos+i)->id!=-1)
        // 1. El bandido tira el dado
        {
            int pasos = generarNumeroEntre(1,4);



            tNodoLCDE* destinoCalculado = destinoBandido((bandidos+i)->posicion_actual, destinoJugador, pasos, totalCasillas);

            // 3. Creamos el "ticket" para la cola
            MovimientoCola movBandido;
            movBandido.tipo_entidad = BANDIDO;
            movBandido.entidad = (bandidos+i);          // Pasamos el bandido
            movBandido.nodoDestino = destinoCalculado;  // Pasamos el nodo que calculamos

            // 4. Metemos el ticket en la cola
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

            // 1. Quitamos al jugador de la casilla vieja
            if (j->posicion_actual != NULL)
            {
               ((NodoRuta*)(j->posicion_actual->info))->hay_jugador = false;
            }

            // 2. Le damos su nueva posicion
            j->posicion_actual = turnoActual.nodoDestino;

            // 3. Activamos al jugador en la casilla nueva
            if (j->posicion_actual != NULL)
            {
                ((NodoRuta*)(j->posicion_actual->info))->hay_jugador = true;
            }
        }
        else if (turnoActual.tipo_entidad == BANDIDO)
        {
            Bandido* b = (Bandido*)turnoActual.entidad;

            // 1. Restamos al bandido de la casilla vieja
            if (b->posicion_actual != NULL && ((NodoRuta*)(b->posicion_actual->info))->cant_bandidos > 0)
            {
                ((NodoRuta*)(b->posicion_actual->info))->cant_bandidos--;
            }

            // 2. Le damos su nueva posicion
            b->posicion_actual = turnoActual.nodoDestino;

            // 3. Sumamos al bandido en la casilla nueva
            if (b->posicion_actual != NULL)
            {
                ((NodoRuta*)(b->posicion_actual->info))->cant_bandidos++;
            }
        }
    }
}

void verificarChoques(JugadorPartida* jugador, Bandido* bandidos, int cantBandidos, tListaCD* mapa)
{

    // 1. Leemos en que casilla esta parado el jugador AHORA
    NodoRuta* CasillaActual = ((NodoRuta*)jugador->posicion_actual->info);
    bool caiEnOasis=false;//ESTA VARIABLE LA UTILIZO PARA SABER SI TENGO UNA PROTECCION VIEJA O ES NUEVA
    // 2. Evaluamos si esta protegido

    if(CasillaActual->terreno == OASIS)
    {
        printf("Caiste en el oasis! estaras protegido el siguiente turno!\n");
        jugador->protegido_por_oasis=true;
        caiEnOasis=true;//CADA VEZ QUE CAIGO EN EL OASIS, EN ESTA LLAMADA A LA FUNCION ACTIVAMOS LA PROTECCION AL JUGADOR
        CasillaActual->terreno=VACIO;
    }

    // 3. Miramos choques con los enemigos
    for (int i = 0; i < cantBandidos; i++)
    {

        if ((bandidos + i)->posicion_actual != NULL)   // Si el bandido esta vivo
        {

            if (jugador->posicion_actual == (bandidos + i)->posicion_actual)
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
                    printf("\nCRITICO: El bandido %d te atrapo en la casilla %d.\n",
                           (bandidos + i)->id,
                           CasillaActual->pos);

                    jugador->vidas--;

                    int hayOtroBandido = 0;

                    for (int j = 0; j < cantBandidos; j++) {
                        if (j != i && (bandidos + j)->id != -1 && (bandidos + j)->posicion_actual == jugador->posicion_actual) {
                            hayOtroBandido = 1;
                            break;
                        }
                    }
                    if (!hayOtroBandido)
                        ((NodoRuta*)jugador->posicion_actual->info)->hay_jugador = false;

                    (bandidos + i)->posicion_actual = NULL;


                    printf("El bandido %d fue eliminado de la ruta.\n",(bandidos + i)->id);
                    ((NodoRuta*)jugador->posicion_actual->info)->cant_bandidos--;
                    (bandidos+i)->id=-1;

                    if (jugador->vidas > 0)
                    {
                        jugador->posicion_actual = obtenerNodoPorPosicion(mapa, 1);
                        ((NodoRuta*)jugador->posicion_actual->info)->hay_jugador = true;
                        printf("Has sido devuelto al campamento inicial (Casilla 1).\n");
                    }
                    return;
                }
            }
        }
    }


    // 4. Evaluamos los terrenos beneficiosos / perjudiciales

    if (CasillaActual->terreno == VIDA_EXTRA)
    {
        printf("\nEncontraste una vida extra.\n");
        jugador->vidas++;
        ((NodoRuta*)jugador->posicion_actual->info)->terreno = VACIO;

    }
    else if (CasillaActual->terreno == PREMIO)
    {
        printf("\nRecogiste un premio.\n");
        jugador->puntos += 100;
        ((NodoRuta*)jugador->posicion_actual->info)->terreno = VACIO;

    }
    else if (CasillaActual->terreno==TORMENTA)
    {
        if(jugador->protegido_por_oasis==true)
            printf("Atravesaste una tormenta, pero fuiste protegido por el oasis!\n");
        else
        {
            printf("\nCaiste en una tormenta. Pierdes tu proximo turno.\n");
            jugador->pierde_proximo_turno = true;
            ((NodoRuta*)jugador->posicion_actual->info)->terreno = VACIO;
        }


    }
    if(jugador->protegido_por_oasis==true&&caiEnOasis==false)
        jugador->protegido_por_oasis=false; //si el jugador tiene un escudo activo, y no cayo en el oasis, significa q la proteccion viene de un turno anterior, hay q desactivarla
}
void mostrarHistorialMovimientos(tCola* historial) {
    MovimientoHistorial mov;
    int turno = 1;

    printf("\n------------------------------------------\n");
    printf("        HISTORIAL DE MOVIMIENTOS        \n");
    printf("-------------------------------------\n");

    // sacarDeCola devuelve 1 si saco algo, 0 si la cola se vacio
    while (sacarDeCola(historial, &mov, sizeof(MovimientoHistorial))) {


        char* direccionTexto = (mov.tipo_movimiento == 'F') ? "adelante" : "atras";

        printf("Turno %d: El jugador se movio %d casillas hacia %s y llego a la casilla %d.\n", turno, mov.casillasMovidas, direccionTexto, mov.casillaDestino);

        turno++;
    }



    printf("-------------------------------------\n");
}
