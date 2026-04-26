#include "../juego.h"
#include "../main.h"
#define NOMBRE_ARCHIVO "caravana.txt"
#define NOMBRE_CONFIG "config.txt"

int leerConfig(Config* configuracion)
{
    FILE* arch_config = fopen(NOMBRE_CONFIG, "rt");
    if (!arch_config)
    {
        return ERROR;
    }

    char linea[256];

    while (fgets(linea, sizeof(linea), arch_config))
    {
        linea[strcspn(linea, "\n")] = '\0';

        char* clave = strtok(linea, ":");
        char* valor_str = strtok(NULL, ":");

        if (clave && valor_str)
        {
            while (*valor_str == ' ')
                valor_str++;

            int valor = atoi(valor_str);

            if (strcmp(clave, "cantidad_posiciones") == 0)
                configuracion->cantidad_posiciones = valor;
            else if (strcmp(clave, "vidas_inicio") == 0)
                configuracion->vidas_inicio = valor;
            else if (strcmp(clave, "maximo_bandidos") == 0)
                configuracion->maximo_bandidos = valor;
            else if (strcmp(clave, "maximo_premios") == 0)
                configuracion->maximo_premios = valor;
            else if (strcmp(clave, "maximo_vidas_extra") == 0)
                configuracion->maximo_vidas_extra = valor;
            else if (strcmp(clave, "maximo_oasis") == 0)
                configuracion->maximo_oasis = valor;
            else if (strcmp(clave, "maximo_tormentas") == 0)
                configuracion->maximo_tormentas = valor;
        }
    }

    fclose(arch_config);
    return OK;
}


void imprimirConfig(const Config* c)
{
    if (!c)
        return;

    printf("Configuracion:\n");
    printf("cantidad_posiciones: %d\n", c->cantidad_posiciones);
    printf("vidas_inicio: %d\n", c->vidas_inicio);
    printf("maximo_bandidos: %d\n", c->maximo_bandidos);
    printf("maximo_premios: %d\n", c->maximo_premios);
    printf("maximo_vidas_extra: %d\n", c->maximo_vidas_extra);
    printf("maximo_oasis: %d\n", c->maximo_oasis);
    printf("maximo_tormentas: %d\n", c->maximo_tormentas);
}