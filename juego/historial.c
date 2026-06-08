#include "../headers/historial.h"

int guardarPartida(const char* nombre, int puntos, int movimientos, int resultado) {
    FILE* pf = fopen(ARCHIVO_PARTIDAS, "ab");
    if (!pf) return 0;

    tPartida nuevaPartida;
    strncpy(nuevaPartida.nombreJugador, nombre, MAX_NOMBRE - 1);
    nuevaPartida.nombreJugador[MAX_NOMBRE - 1] = '\0';
    nuevaPartida.puntos = puntos;
    nuevaPartida.movimientos = movimientos;
    nuevaPartida.resultado = resultado;

    // Obtener la fecha y hora del sistema
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);
    strftime(nuevaPartida.fecha, 20, "%d/%m/%Y %H:%M", tm_info);

    fwrite(&nuevaPartida, sizeof(tPartida), 1, pf);
    fclose(pf);

    return 1;
}

void mostrarHistorialJugador(const char* nombre) {
    FILE* pf = fopen(ARCHIVO_PARTIDAS, "rb");
    if (!pf) {
        printf("Aun no hay partidas registradas.\n");
        return;
    }

    tPartida partidaActual;
    int encontradas = 0;

    while (fread(&partidaActual, sizeof(tPartida), 1, pf) == 1)
    {
        if (strcmp(partidaActual.nombreJugador, nombre) == 0)
        {
            if(encontradas == 0)
            {
                printf("\n");
                printf("==========================================================\n");
                printf("                 HISTORIAL DE PARTIDAS - %s\n", nombre);
                printf("==========================================================\n");
                printf("| %-16s | %-8s | %-11s | %-10s |\n",
                    "FECHA","PUNTOS","MOVIMIENTOS","RESULTADO");
                printf("----------------------------------------------------------\n");
            }

            encontradas++;

            printf("| %-16s | %8d | %11d | %-10s |\n",
                partidaActual.fecha,
                partidaActual.puntos,
                partidaActual.movimientos,
                partidaActual.resultado ? "Victoria" : "Derrota");
        }
    }

    if (encontradas == 0)
            printf("| %s |\n","No se encontraron partidas para este jugador.");
    else
    {
        printf("==========================================================\n");
        printf("Total de partidas encontradas: %d\n\n", encontradas);
    }
    fclose(pf);
}
