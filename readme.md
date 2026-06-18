# Caravana del Desierto

Trabajo Práctico de Algoritmos y Estructuras de Datos — 2026 C1.

## Compilación

### Con Code::Blocks
Abrir `TP_ALG_CARAVANA.cbp` y compilar con **Build > Build**.

### Con GCC (línea de comandos)
Desde la raíz del proyecto:

```bash
gcc main.c \
    archivos/files.c \
    juego/config.c \
    juego/entidades.c \
    juego/historial.c \
    juego/indice.c \
    juego/mapa.c \
    juego/menu.c \
    juego/utilidades.c \
    tda/arbol.c \
    tda/cola.c \
    tda/listaCDE.c \
    tda/pila.c \
    -o caravana
```

Luego ejecutar:

```bash
./caravana
```

> En Windows reemplazar `./caravana` por `caravana.exe`.

## Archivos con info

El juego genera y usa los siguientes archivos en la raíz del proyecto:

| Archivo | Descripción |
|---|---|
| `config.txt` | Parámetros del juego (vidas, bandidos, etc.) |
| `caravana.txt` | Estado del tablero de la última partida |
| `jugadores.bin` | Registro de jugadores y puntajes |
| `jugadores.idx` | Índice del árbol binario de búsqueda |
| `partidas.bin` | Historial de partidas jugadas |

Decidimos dejar los archivos con algunos datos de prueba para tener al alcance para probar, para empezar de cero se puede borrarlos para que se vuelvan a generar (menos el de config.txt)

## Algunas decisiones que tomamos

- **+5 puntos por ganar** la partida (además de los puntos acumulados durante el juego)
- **+1 punto por premio** recogido en el tablero
- El bandido **apunta al destino futuro del jugador**, no a su posición actual
- El **dado del bandido** va de 1 a 4 (no 1 a 6 como el jugador)
- La **tormenta desaparece** de la casilla cuando el jugador cae en ella (no queda permanente)
- El **oasis también desaparece** al ser pisado
- Al ser **atrapado por un bandido**, el jugador vuelve a la casilla 1 y el bandido se elimina del tablero
- Si hay **bandidos en la casilla 1** cuando el jugador vuelve ahí, los bandidos retroceden 1 casilla para dar lugar
- El jugador **no puede retroceder** si la posición actual menos los pasos daría menor a 1
- Si el jugador **sobrepasa la salida**, rebota y retrocede las casillas sobrantes
- **Login con nombre sugerido**: si el nombre ya existe, ofrece agregar un número aleatorio de 3 dígitos como sufijo
