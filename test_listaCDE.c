// Usar estos 2 comandos para compilar y luego ejecutar
// gcc test_listaCDE.c tda/listaCDE.c -o testLista
// ./testLista.exe

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "headers/tda/listaCDE.h"

void mostrarInt(const void* d)
{
    printf("%d ", *(int*)d);
}

int compararInt(const void* a, const void* b)
{
    int valA = *(int*)a;
    int valB = *(int*)b;
    return (valA == valB) ? 1 : 0;
}

int modificarInt(void** d, unsigned* tam, const void* elem, unsigned tamElem)
{
    int* valor = (int*)*d;
    *valor *= 2;
    return 1;
}

void mostrarMenuTDA()
{
    printf("\n========== MENU DE PRUEBAS - LISTA DOBLEMENTE ENLAZADA ==========\n");
    printf("1. Crear Lista\n");
    printf("2. Insertar al comienzo\n");
    printf("3. Insertar al final\n");
    printf("4. Sacar del primero\n");
    printf("5. Sacar del último\n");
    printf("6. Mostrar lista\n");
    printf("7. Verificar si está vacía\n");
    printf("8. Verificar si está llena\n");
    printf("9. Vaciar lista\n");
    printf("10. Buscar elemento\n");
    printf("11. Modificar valor (multiplicar por 2)\n");
    printf("0. Salir\n");
    printf("==================================================================\n");
    printf("Seleccione opción: ");
}

int main()
{
    system("chcp 65001 > nul");
    tListaCD lista = NULL;
    int opcion;
    int valor;
    int resultado;
    int buffer;
    
    printf("PROGRAMA DE PRUEBAS PARA LISTA DOBLEMENTE ENLAZADA\n");
    printf("=====================================================\n\n");
    
    do {
        mostrarMenuTDA();
        scanf("%d", &opcion);
        getchar();
        
        switch(opcion) {
            case 1:
                crearListaCD(&lista);
                printf("✓ Lista creada correctamente\n");
                break;
                
            case 2:
                printf("Ingrese el valor a insertar al comienzo: ");
                scanf("%d", &valor);
                getchar();
                resultado = ponerAlComienzoCD(&lista, &valor, sizeof(int));
                if (resultado) {
                    printf("✓ Valor %d insertado al comienzo\n", valor);
                } else {
                    printf("✗ Error al insertar el valor\n");
                }
                break;
                
            case 3:
                printf("Ingrese el valor a insertar al final: ");
                scanf("%d", &valor);
                getchar();
                resultado = ponerAlFinalCD(&lista, &valor, sizeof(int));
                if (resultado) {
                    printf("✓ Valor %d insertado al final\n", valor);
                } else {
                    printf("✗ Error al insertar el valor\n");
                }
                break;
                
            case 4:
                resultado = sacarPrimeroListaCD(&lista, &buffer, sizeof(int));
                if (resultado) {
                    printf("✓ Valor %d sacado del primero\n", buffer);
                } else {
                    printf("✗ No se puede sacar del primero (lista vacía o error)\n");
                }
                break;
                
            case 5:
                resultado = sacarUltimoListaCD(&lista, &buffer, sizeof(int));
                if (resultado) {
                    printf("✓ Valor %d sacado del último\n", buffer);
                } else {
                    printf("✗ No se puede sacar del último (lista vacía o error)\n");
                }
                break;
                
            case 6:
                if (listaVaciaCD(&lista)) {
                    printf("La lista está vacía\n");
                } else {
                    printf("Contenido de la lista: ");
                    mostrarListaCD(&lista, mostrarInt);
                    printf("\n");
                }
                break;
                
            case 7:
                if (listaVaciaCD(&lista)) {
                    printf("✓ La lista está VACÍA\n");
                } else {
                    printf("✗ La lista NO está vacía\n");
                }
                break;
                
            case 8:
                if (listaLlenaCD(&lista, sizeof(int))) {
                    printf("✓ La lista está LLENA (sin memoria disponible)\n");
                } else {
                    printf("✗ La lista NO está llena (hay espacio disponible)\n");
                }
                break;
                
            case 9:
                vaciarListaCD(&lista);
                printf("✓ Lista vaciada correctamente\n");
                break;
                
            case 10: {
                printf("Ingrese el valor a buscar: ");
                scanf("%d", &valor);
                getchar();
                resultado = buscarElementoLista(&lista, &valor, sizeof(int), compararInt, &buffer);
                if (resultado) {
                    printf("✓ Elemento %d encontrado en la lista\n", valor);
                } else {
                    printf("✗ Elemento %d NO encontrado en la lista\n", valor);
                }
                break;
            }
                
            case 11: {
                printf("Ingrese el valor a modificar (será multiplicado por 2): ");
                scanf("%d", &valor);
                getchar();
                resultado = modificarValor(&lista, &valor, sizeof(int), compararInt, modificarInt, &valor);
                if (resultado) {
                    printf("✓ Valor %d modificado exitosamente\n", valor);
                } else {
                    printf("✗ No se encontró el valor o no se pudo modificar\n");
                }
                break;
            }
                
            case 0:
                printf("Saliendo del programa...\n");
                vaciarListaCD(&lista);
                break;
                
            default:
                printf("✗ Opción inválida. Intente de nuevo.\n");
        }
        
    } while(opcion != 0);
    
    return 0;
}
