#include "../tda.h"

typedef struct sNodo
{
    void* info;
    unsigned tamInfo;
    struct sNodo *sig;
    struct sNodo *ant;
} tNodo;

typedef tNodo* tLista;

void crearLista(tLista* p);
int vaciarLista(tLista* p);
int insertarAlFinal(tLista* p, const void *d, unsigned cantBytes);
int mostrarDeIzqADer(const tLista* p, void(*mostrar)(const void*));