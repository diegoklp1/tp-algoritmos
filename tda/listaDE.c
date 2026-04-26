#include "../tda_headers/listaDE.h"

void crearLista(tLista* p)
{
    *p = NULL;
}

int insertarAlFinal(tLista* p, const void *d, unsigned cantBytes) {
    tNodo* act = *p;
    tNodo* nue;

    if(act)
        while(act->sig)
            act = act->sig;
    if((nue=(tNodo*)malloc(sizeof(tNodo))) == NULL ||
       (nue->info = malloc(cantBytes)) == NULL)
    {
        free(nue);
        return 0;
    }
    memcpy(nue->info, d, cantBytes);
    nue->tamInfo = cantBytes;
    nue->sig = NULL;
    nue->ant = act;
    if(act)
        act->sig = nue;
    *p = nue;
    return 1;
}

int mostrarDeIzqADer(const tLista* p, void(*mostrar)(const void*))
{
    tNodo* act = *p;
    int cant = 0;

    if(act)
    {
        mostrar(NULL);
        while(act->ant)
            act = act->ant;
        while(act)
        {
            mostrar(act->info);
            act = act->sig;
            cant++;
        }
    }
    return cant;
}

int vaciarLista(tLista* p) {
    int cant = 0;
    tNodo *act = *p;
    if(act)
    {
        while(act->ant)
            act = act->ant;
        while(act)
        {
            tNodo *aux = act->sig;
            free(act->info);
            free(act);
            act = aux;
            cant++;
        }
        *p = NULL;
    }
    return cant;
}
