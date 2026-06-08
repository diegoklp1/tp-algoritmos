#include "../headers/juego.h"
#include "../headers/mapa.h"
#include "../headers/indice.h"
#include "../headers/historial.h"
#include "../headers/menu.h"

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
        printf("3. Ver historial de un jugador\n");
        printf("4. Eliminar Jugador\n");
        printf("5. Salir del juego\n");
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
                mostrarRankingPorPuntos();
                break;
            case 3:
                {
                    char nombreHistorial[21];
                    printf("\n");
                    ingresarYValidarNombre(nombreHistorial);
                    mostrarHistorialJugador(nombreHistorial);
                    break;
                }
            case 4:
                {
                    char nombreBaja[MAX_NOMBRE_JUGADOR];
                    printf("\n");
                    ingresarYValidarNombre(nombreBaja);
                    strMayus(nombreBaja, nombreBaja);

                    if (bajaJugador(indice, nombreBaja))
                        printf("Jugador '%s' dado de baja correctamente.\n", nombreBaja);
                    else
                        printf("El jugador '%s' no existe o hubo un error.\n", nombreBaja);
                }
                break;
            case 5:
                printf("Saliendo del juego\n");
                break;
            default:
                printf("opcion incorrecta, intente con otra.\n");
        }
    } while (opcion != 5);
    return 4;
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

