#include "../headers/tda/arbol.h"

#define BANDERA_NULO  0
#define BANDERA_NODO  1

static tNodoArbol **buscarNodoArbolBinBusq(const tArbolBinBusq *p, const void *d, Cmp cmp);
static tNodoArbol **mayorNodoArbolBinBusq (const tArbolBinBusq *p);
static tNodoArbol **menorNodoArbolBinBusq (const tArbolBinBusq *p);
static unsigned alturaArbolBin (const tArbolBinBusq *p);
static void _recorrerInOrden (const tArbolBinBusq *p, unsigned n, void *params, Accion accion);
static void _guardarPreOrdenRec (const tArbolBinBusq *p, FILE *pf);
static int _cargarPreOrdenRec (tArbolBinBusq *p, FILE *pf);

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
    if (!reservarMemoriaNodo(nue, sizeof(tNodoArbol), nue->info, tam))
        return SIN_MEM;

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

int guardarIndiceArbolBinBusq(const tArbolBinBusq *p, const char *nomArch)
{
    FILE *pf;
    if (!(pf = fopen(nomArch, "wb")))
        return ERROR_ARCH;
    _guardarPreOrdenRec(p, pf);
    fclose(pf);
    return TODO_BIEN;
}

int cargarIndiceArbolBinBusq(tArbolBinBusq *p, const char *nomArch)
{
    FILE *pf;
    int r;

    if (*p)
        return SIN_INICIALIZAR;
    if (!(pf = fopen(nomArch, "rb")))
        return ERROR_ARCH;

    r = _cargarPreOrdenRec(p, pf);
    fclose(pf);
    return r;
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

static void _guardarPreOrdenRec(const tArbolBinBusq *p, FILE *pf)
{
    unsigned char bandera;

    if (!*p) {
        bandera = BANDERA_NULO;
        fwrite(&bandera, sizeof(unsigned char), 1, pf);
        return;
    }

    bandera = BANDERA_NODO;
    fwrite(&bandera,sizeof(unsigned char), 1,pf);
    fwrite(&(*p)->tamInfo, sizeof(unsigned),1,pf);
    fwrite((*p)->info,1,(*p)->tamInfo, pf);

    _guardarPreOrdenRec(&(*p)->izq, pf);
    _guardarPreOrdenRec(&(*p)->der, pf);
}

static int _cargarPreOrdenRec(tArbolBinBusq *p, FILE *pf)
{
    unsigned char bandera;
    unsigned tamInfo;
    tNodoArbol* nue;

    if (fread(&bandera, sizeof(unsigned char), 1, pf) != 1)
        return ERROR_ARCH;

    if (bandera == BANDERA_NULO) {
        *p = NULL;
        return TODO_BIEN;
    }

    if (fread(&tamInfo, sizeof(unsigned), 1, pf) != 1)
        return ERROR_ARCH;

    nue = (tNodoArbol *)malloc(sizeof(tNodoArbol));
    if (!nue) return SIN_MEM;
    nue->info = malloc(tamInfo);
    if (!nue->info) { free(nue); return SIN_MEM; }

    if (fread(nue->info, 1, tamInfo, pf) != tamInfo) {
        free(nue->info);
        free(nue);
        return ERROR_ARCH;
    }

    nue->tamInfo = tamInfo;
    nue->izq = NULL;
    nue->der = NULL;
    *p = nue;

    if (_cargarPreOrdenRec(&(*p)->izq, pf) != TODO_BIEN) return ERROR_ARCH;
    if (_cargarPreOrdenRec(&(*p)->der, pf) != TODO_BIEN) return ERROR_ARCH;

    return TODO_BIEN;
}
