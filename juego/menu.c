#include "../headers/juego.h"
#include "../headers/mapa.h"
#include "../headers/indice.h"

int mostrarMenu(tArbolBinBusq *indice)
{
    int opcion;
    printf("========================================\n");
    printf("   BIENVENIDO A CARAVANA DEL DESIERTO   \n");
    printf("========================================\n");
    do {
        printf("\n--- MENU PRINCIPAL ---\n");
        printf("1. Jugar una nueva partida\n");
        printf("2. Ver el ranking de jugadores\n");
        printf("3. Salir del juego\n");
        printf("Seleccione una opcion: ");

        // validacion para evitar loops si ingresan letras
        if (scanf("%d",&opcion)!= 1) {
            while(getchar() != '\n'); // limpia buffer
            opcion = 0;
        } else {
            while(getchar() != '\n');
        }

        switch (opcion) {
            case 1:
                return 1;
                break;
            case 2:
                mostrarRankingAlfabetico(indice);
                break;
            case 3:
                printf("Saliendo del juego\n");
                break;
            default:
                printf("opcion incorrecta, intente con otra.\n");
        }
    } while (opcion != 3);
    return 3;
}

void ingresarYValidarNombre(char* nombre)
{
    int valido = 0;

    while (!valido) {
        printf("Ingrese su nombre de jugador (max 20 caracteres): ");
        if (fgets(nombre, 21, stdin) != NULL) {
            // Remover el salto de l�nea generado por fgets
            nombre[strcspn(nombre, "\n")] = 0;

            // Validar que el nombre no est� vac�o
            if (strlen(nombre) > 0) {
                valido = 1;
            } else {
                printf("Error: El nombre no puede estar vac�o.\n");
            }
        }
    }
}

