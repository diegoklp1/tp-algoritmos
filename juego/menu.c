#include "../headers/juego.h"
#include "../headers/mapa.h"

int mostrarMenu()
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
                verRanking();
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
            // Remover el salto de línea generado por fgets
            nombre[strcspn(nombre, "\n")] = 0;

            // Validar que el nombre no esté vacío
            if (strlen(nombre) > 0) {
                valido = 1;
            } else {
                printf("Error: El nombre no puede estar vacío.\n");
            }
        }
    }

    // stubs para gestionar los datos
    if (buscarJugador(nombre)) {
        printf("Bienvenido nuevamente, %s\n", nombre);
    } else {
        printf("Jugador nuevo detectado. Procediendo al alta...\n");
        altaJugador(nombre);
        printf("Jugador '%s' registrado con exito.\n", nombre);
    }
}

// stubs

void verRanking()
{
    printf("\n--- RANKING DE JUGADORES ---\n");
    printf("recorriendo el arbol binario de búsqueda para mostrar puntuaciones...\n"); // stub
}

int buscarJugador(const char* nombre)
{
    printf("Buscando al jugador '%s' en el indice (Arbol Binario)...\n", nombre);
    // Retornamos 0 para simular que no existe y probar el alta.
    // Cambiar a 1 para simular que el jugador ya existe.
    return 0;
}

void altaJugador(const char* nombre)
 {
    printf("Escribiendo al jugador '%s' en el archivo binario y actualizando el arbol...\n", nombre); //stub
}
