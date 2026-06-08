#include "headers/main.h"
#include "headers/juego.h"
#include "headers/mapa.h"
#include "headers/tda/cola.h"
#include "headers/menu.h"
#include "headers/indice.h"
#include "time.h"
#include "headers/historial.h"

int main()
{
    char nombreJugador[21];
    srand(time(NULL));

    Config configuracion;
    leerConfig(&configuracion);

    tArbolBinBusq indice;
    iniciarIndiceJugadores(&indice);

    while(mostrarMenu(&indice)==1)
    {
        loginJugador(&indice,nombreJugador);

        tListaCD mapa;
        crearListaCD(&mapa);

        tCola colaTurnos,colaHistorialMovs;
        crearCola(&colaTurnos);
        crearCola(&colaHistorialMovs);

        Bandido listaBandidos[configuracion.maximo_bandidos];
        generarTablero(&configuracion,listaBandidos,&mapa);

        JugadorPartida Jugador;
        strncpy(Jugador.nombre, nombreJugador, sizeof(Jugador.nombre) - 1);
        Jugador.vidas=configuracion.vidas_inicio;
        Jugador.posicion_actual=mapa;
        Jugador.puntos=0;
        Jugador.movimientos=0;
        Jugador.protegido_por_oasis=false;
        Jugador.pierde_proximo_turno=false;


        int gano=0;
        while(Jugador.vidas>0&&!gano)
        {
            printf("\n==================================\n");
            printf("Vidas: %d\n", Jugador.vidas);

            //Variable para guardar el destino del jugador y pasarsela a los bandidos
            tNodoLCDE* destinoJugador = NULL;

            //FASE1:PLANIFICACI�N
            if(Jugador.pierde_proximo_turno==false)
                planificarTurnoJugador(&Jugador, &colaTurnos, &destinoJugador,&colaHistorialMovs);
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

            if(((NodoRuta*)Jugador.posicion_actual->info)->terreno==SALIDA&&Jugador.vidas>0)
                gano=1;

            if (Jugador.vidas > 0&&!gano)
            {
                printf("\n--- ESTADO DEL TABLERO ---\n");
                //mostramos el tablero
                mostrarListaArchCD(&mapa, stdout, mostrarPos);
            }
        }

        actualizarJugador(&indice, nombreJugador, Jugador.puntos);
        //imprimirConfig(&configuracion);
        guardarPartida(nombreJugador, Jugador.puntos, Jugador.movimientos, gano);
        if(gano)
        {
            printf("\n--- ESTADO DEL TABLERO ---\n");
            mostrarListaArchCD(&mapa, stdout, mostrarPos);
            printf("Llegaste  a Ciudad Refugio, juego terminado. felicidades!");
            printf("\n Cantidad de puntos obtenidos %d\n",Jugador.puntos);
        }
        else
            printf("Mala suerte! los bandidos acabaron contigo. Partida terminada!\n");


        vaciarCola(&colaTurnos);
        vaciarListaCD(&mapa);
        mostrarHistorialMovimientos(&colaHistorialMovs);
        vaciarCola(&colaHistorialMovs);

    }
    guardarIndiceJugadores(&indice);
    liberarIndiceJugadores(&indice);

    return 0;
}
