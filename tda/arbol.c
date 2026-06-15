#include "../headers/tda/arbol.h"

static tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, Cmp cmp);
static tNodoArbol **mayorNodoArbolBinBusq (const tArbolBinBusq *p);
static tNodoArbol **menorNodoArbolBinBusq (const tArbolBinBusq *p);
static unsigned alturaArbolBin (const tArbolBinBusq *p);
static void _recorrerInOrden (const tArbolBinBusq *p, unsigned n, void *params, Accion accion);
static void _escribirInOrden(void *info, unsigned tamInfo, unsigned nivel, void *param);
static int _cargarDesdeArchBinOrdArbol(FILE *arch, tArbolBinBusq *p, unsigned tamInfo, int li, int ls);

void crearArbolBinBusq(tArbolBinBusq *p)
{
    *p = NULL;
}

int insertarArbolBinBusq(tArbolBinBusq *p, const void *d, unsigned tam, Cmp compara)
{
    tNodoArbol *nue;
    int comp;

    while (*p) {
        if ((comp = compara(d, (*p)->info)) < 0)
            p = &(*p)->izq;
        else if (comp > 0)
            p = &(*p)->der;
        else
            return CLA_DUP;
    }

    nue = malloc(sizeof(tNodoArbol));
    if (nue == NULL)
        return SIN_MEM;

    nue->info = malloc(tam);
    if (nue->info == NULL)
    {
        free(nue);
        return SIN_MEM;
    }

    nue->tamInfo = tam;
    memcpy(nue->info, d, tam);
    nue->izq = NULL;
    nue->der = NULL;

    *p = nue;
    return TODO_BIEN;
}

int buscarElemArbolBinBusq(const tArbolBinBusq *p, void *d, unsigned tam, Cmp cmp)
{
    if (!(p = buscarNodoArbolBinBusq(p, d, cmp)))
        return 0;
    memcpy(d, (*p)->info, minimo(tam, (*p)->tamInfo));
    return 1;
}

int eliminarRaizArbolBinBusq(tArbolBinBusq *p)
{
    tNodoArbol **remp;
    tNodoArbol *elim;

    if (!*p)
        return 0;
    free((*p)->info);

    if (!(*p)->izq && !(*p)->der) {
        free(*p);
        *p = NULL;
        return 1;
    }

    remp = alturaArbolBin(&(*p)->izq) > alturaArbolBin(&(*p)->der) ? mayorNodoArbolBinBusq(&(*p)->izq) : menorNodoArbolBinBusq(&(*p)->der);

    elim = *remp;
    (*p)->info = elim->info;
    (*p)->tamInfo = elim->tamInfo;
    *remp = elim->izq ? elim->izq : elim->der;

    free(elim);
    return 1;
}

int eliminarElemArbolBinBusq(tArbolBinBusq *p, void *d, unsigned tam, Cmp compara)
{
    if (!(p = buscarNodoArbolBinBusq(p, d, compara)))
        return 0;
    memcpy(d, (*p)->info, minimo(tam, (*p)->tamInfo));
    return eliminarRaizArbolBinBusq(p);
}

void recorrerEnOrdenArbolBinBusq(const tArbolBinBusq *p, void *params, Accion accion)
{
    _recorrerInOrden(p, 0, params, accion);
}

int crearDesdeArchBinArbol(FILE *arch, const tArbolBinBusq *arbol)
{
    if (!arch) return ERROR_ARCH;
    rewind(arch);
    _recorrerInOrden(arbol, 0, arch, _escribirInOrden);
    return TODO_BIEN;
}

int cargarDesdeArchBinOrdArbol(FILE *arch, tArbolBinBusq *p, unsigned tamInfo)
{
    if (!arch || *p) return ERROR_ARCH;
    fseek(arch, 0L, SEEK_END);
    long cant = ftell(arch) / tamInfo;
    if (cant == 0) return TODO_BIEN;
    return _cargarDesdeArchBinOrdArbol(arch, p, tamInfo, 0, cant-1);
}

static tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, Cmp compara)
{
    int comp;
    while (*p && (comp = compara(d, (*p)->info)))
    {
        if((comp < 0))
			p = &(*p)->izq;
        else
			p = &(*p)->der;
    }
    if (!*p)
        return NULL;
    return (tNodoArbol **)p;
}

static tNodoArbol **mayorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p) return NULL;
    while ((*p)->der) p = &(*p)->der;
    return (tNodoArbol **)p;
}

static tNodoArbol **menorNodoArbolBinBusq(const tArbolBinBusq *p)
{
    if (!*p) return NULL;
    while ((*p)->izq) p = &(*p)->izq;
    return (tNodoArbol **)p;
}

static unsigned alturaArbolBin(const tArbolBinBusq *p)
{
    unsigned hi, hd;
    if (!*p) return 0;
    hi = alturaArbolBin(&(*p)->izq);
    hd = alturaArbolBin(&(*p)->der);
    return (hi > hd ? hi : hd) + 1;
}

static void _recorrerInOrden(const tArbolBinBusq *p, unsigned n, void *params, Accion accion)
{
    if (!*p)
        return;
    _recorrerInOrden(&(*p)->izq, n + 1, params, accion);
    accion((*p)->info, (*p)->tamInfo, n, params);
    _recorrerInOrden(&(*p)->der, n + 1, params, accion);
}

static void _escribirInOrden(void *info, unsigned tamInfo, unsigned nivel, void *param)
{
    fwrite(info, tamInfo, 1, (FILE*)param);
}

static int _cargarDesdeArchBinOrdArbol(FILE *arch, tArbolBinBusq *p, unsigned tamInfo, int li, int ls)
{
    if (li > ls) return TODO_BIEN;

    int m = (li + ls) / 2;
    void *buffer = malloc(tamInfo);
    if (!buffer) return SIN_MEM;

    fseek(arch, (long)m * tamInfo, SEEK_SET);
    if (fread(buffer, tamInfo, 1, arch) != 1) {
        free(buffer);
        return ERROR_ARCH;
    }

    tNodoArbol *nue = malloc(sizeof(tNodoArbol));
    if (!nue) { free(buffer); return SIN_MEM; }
    nue->info = buffer;
    nue->tamInfo = tamInfo;
    nue->izq = NULL;
    nue->der = NULL;
    *p = nue;

    int ri =_cargarDesdeArchBinOrdArbol(arch, &(*p)->izq, tamInfo, li,m - 1);
    int rd =_cargarDesdeArchBinOrdArbol(arch, &(*p)->der, tamInfo, m+1,ls);
    return ri != TODO_BIEN ? ri : rd;
}

