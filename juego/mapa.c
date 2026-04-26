#include "../juego.h"
#include "../main.h" //ver si así está bien(que solicite ambos .h o separar en otro .h)

void mostrarTablero(char tablero[25])
{
    for(int i=0; i<25; i++)
    {
        printf("%c\n", tablero[i]);
    }
}

int generarTablero(Config* c)
{
    FILE* tableroArch = fopen(NOMBRE_ARCHIVO, "w+t");
    char tablero[25] = {0};
//    if(tableroArch)
//    {
//        return ERROR;
//    }
    for(int i=0; i<25; i++)
    {
        tablero[i] = VACIO;
    }
    tablero[0] = JUGADOR;
    tablero[generarNumeroEntre(0, 25)] = SALIDA;
    int num;
    for(int i=0; i<5; i++)
    {
        if(i==0)
        {
            printf("Generando bandidos\n");
            for(int j=0; j<c->maximo_bandidos; j++)
            {
                do
                {
                    num = generarNumeroEntre(0, 25);
                }
                while(tablero[num] != VACIO);
                tablero[num] = BANDIDO;
            }
        }
        else if(i==1)
        {
            for(int j=0; j<c->maximo_premios; j++)
            {
                do
                {
                    num = generarNumeroEntre(0, 25);
                }
                while(tablero[num] != VACIO);
                tablero[num] = PREMIO;

            }
        }
        else if(i==2)
        {
            for(int j=0; j<c->maximo_vidas_extra; j++)
            {
                do
                {
                    num = generarNumeroEntre(0, 25);
                }
                while(tablero[num] != VACIO);
                tablero[num] = VIDA_EXTRA;

            }
        }
        else if(i==3)
        {
            for(int j=0; j<c->maximo_oasis; j++)
            {
                do
                {
                    num = generarNumeroEntre(0, 25);
                }
                while(tablero[num] != VACIO);

                tablero[num] = OASIS;

            }
        }
        else if(i==4)
        {
            for(int j=0; j<c->maximo_tormentas; j++)
            {
                do
                {
                    num = generarNumeroEntre(0, 25);
                }
                while(tablero[num] != VACIO);
                tablero[num] = TORMENTA;
            }
        }
    }
    mostrarTablero(tablero);
    fclose(tableroArch);
}