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

    printf("\n======= HISTORIAL DE PARTIDAS: %s          =======\n", nombre);
    printf("%-18s | %-8s | %-12s | %-10s\n", "Fecha", "Puntos", "Movimientos", "Resultado");
    printf("-----------------------------------------------------------\n");

    // Recorrer el archivo leyendo partida por partida
    while (fread(&partidaActual, sizeof(tPartida), 1, pf) == 1) {
        // _strnicmp compara ignorando mayúsculas y minúsculas
        if (_strnicmp(partidaActual.nombreJugador, nombre, MAX_NOMBRE) == 0) {
            encontradas++;
            printf("%-18s | %-8d | %-12d | %-10s\n",
                   partidaActual.fecha,
                   partidaActual.puntos,
                   partidaActual.movimientos,
                   partidaActual.resultado == 1 ? "Victoria" : "Derrota");
        }
    }

    if (encontradas == 0) {
        printf("  No se encontraron partidas para este jugador.\n");
    }
    fclose(pf);
}
