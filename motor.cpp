#include "motor.h"

// ERROR: falta verificar fila_abs >= 0 antes de acceder al arreglo.
// Con py = -1 al hacer spawn, cuando i = 0, fila_abs = -1.
// Acceder a t->filas[-1] es comportamiento indefinido en C++:
// lee memoria fuera del arreglo. En la practica hace que
// hayColision retorne true para la pieza recien creada,
// causando Game Over inmediato al iniciar el juego sin que
// el usuario haya hecho nada.
// Se detecta al correr: el programa muestra Game Over en el
// primer turno con el tablero completamente vacio.
static bool colisionConMascaraYOffset(Tablero* t, unsigned short mascara,
                                       int px, int py, int dx, int dy) {
    for (int i = 0; i < MASCARA_TAMANIO; i++) {
        for (int j = 0; j < MASCARA_TAMANIO; j++) {
            int bit_src = 15 - i * 4 - j;
            if (!((mascara >> bit_src) & 1u)) continue;

            int fila_abs = py + dy + i;
            int col_abs  = px + dx + j;

            if (col_abs  < 0)         return true;
            if (col_abs  >= t->ancho)  return true;
            if (fila_abs >= t->alto)   return true;

            // BUG: falta if (fila_abs >= 0) antes de este acceso
            int bit_tablero = t->ancho - 1 - col_abs;
            if ((t->filas[fila_abs] >> bit_tablero) & 1u)
                return true;
        }
    }
    return false;
}

bool hayColision(Tablero* t, Pieza* pieza, int dx, int dy) {
    if (t == nullptr || pieza == nullptr) return true;
    return colisionConMascaraYOffset(t, pieza->mascara,
                                     pieza->px, pieza->py, dx, dy);
}
bool hayColisionConMascara(Tablero* t, Pieza* pieza,
                            unsigned short mascara, int dx, int dy) {
    if (t == nullptr || pieza == nullptr) return true;
    return colisionConMascaraYOffset(t, mascara,
                                     pieza->px, pieza->py, dx, dy);
}
void moverIzquierda(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;
    if (!hayColision(t, pieza, -1, 0)) pieza->px--;
}
void moverDerecha(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;
    if (!hayColision(t, pieza, +1, 0)) pieza->px++;
}
bool bajarPieza(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return false;
    if (hayColision(t, pieza, 0, +1)) { fijarPieza(t, pieza); return true; }
    pieza->py++;
    return false;
}
void girarPieza(Tablero* t, Pieza* pieza) {
    if (pieza == nullptr) return;
    Pieza copia = *pieza;
    rotarPieza(&copia);
    if (!hayColisionConMascara(t, pieza, copia.mascara, 0, 0))
        pieza->mascara = copia.mascara;
}
bool esGameOver(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return true;
    return hayColision(t, pieza, 0, 0);
}
