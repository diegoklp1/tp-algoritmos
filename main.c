#include "headers/main.h"
#include "headers/juego.h"
#include "headers/mapa.h"


int main()
{
    printf("HOLA MUNDO");
    Config configuracion;
    leerConfig(&configuracion);
    imprimirConfig(&configuracion);
    generarTablero(&configuracion);
    
    return 0;
}
