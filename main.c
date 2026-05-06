#include "headers/main.h"
#include "headers/juego.h"
#include "headers/mapa.h"
#include "headers/tda/cola.h"

int main()
{
    //printf("HOLA MUNDO");
    srand(time(NULL));

    Config configuracion;
    leerConfig(&configuracion);

    tListaCD mapa;
    crearListaCD(&mapa);

    tCola colaTurnos;
    crearCola(&colaTurnos);

    Bandido listaBandidos[configuracion.maximo_bandidos];
    generarTablero(&configuracion,listaBandidos,&mapa);

    JugadorPartida Jugador;
    Jugador.vidas=configuracion.vidas_inicio;
    Jugador.posicion_actual=mapa;
    Jugador.puntos=0;
    Jugador.protegido_por_oasis=false;


    int gano=0;
    while(Jugador.vidas>0&&!gano)
    {
        printf("\n==================================\n");
        printf("Vidas: %d\n", Jugador.vidas);

        //Variable para guardar el destino del jugador y pasársela a los bandidos
        tNodoLCDE* destinoJugador = NULL;

        //FASE1:PLANIFICACIÓN
        if(Jugador.pierde_proximo_turno==false)
            planificarTurnoJugador(&Jugador, &colaTurnos, &destinoJugador);
        else
        {
            printf("Caiste en una tormenta! perdiste tu turno.. los bandidos avanzan hacia ti..");
            destinoJugador=Jugador.posicion_actual;
            Jugador.pierde_proximo_turno=false;
        }
        planificarTurnoBandidos(listaBandidos, configuracion.maximo_bandidos, destinoJugador, configuracion.cantidad_posiciones, &colaTurnos);

        //FASE2:Movimientos
        actualizarPosiciones(&colaTurnos);
        verificarChoques(&Jugador,listaBandidos,configuracion.maximo_bandidos,&mapa);
        if(((NodoRuta*)Jugador.posicion_actual->info)->terreno==SALIDA)
        {
            gano=1;



        }

        if (Jugador.vidas > 0&&!gano)
        {
            printf("\n--- ESTADO DEL TABLERO ---\n");
            //mostramos el tablero
            mostrarListaArchCD(&mapa, stdout, mostrarPos);
        }
    }
    //imprimirConfig(&configuracion);
    if(gano)
    {
        printf("\n--- ESTADO DEL TABLERO ---\n");
        mostrarListaArchCD(&mapa, stdout, mostrarPos);
        printf("Llegaste  a Ciudad Refugio, juego terminado. felicidades!");
        printf("\n Cantidad de puntos obtenidos %d",Jugador.puntos);

    }
    else
        printf("Mala suerte! los bandidos acabaron contigo. Partida terminada!");
    vaciarCola(&colaTurnos);
    vaciarListaCD(&mapa);
    return 0;
}
