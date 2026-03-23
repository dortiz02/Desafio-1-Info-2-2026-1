#include "motor.h"

// ============================================================
//  motor.cpp — Logica central: colisiones, movimientos.
//
//  Esquema de bits del tablero:
//    Col 0 = bit (ancho-1)  [MSB = izquierda]
//    Col k = bit (ancho-1-k)
// ============================================================

// ------------------------------------------------------------
//  Nucleo de colision: itera celdas activas de la mascara.
// ------------------------------------------------------------
static bool colisionConMascaraYOffset(Tablero* t, unsigned short mascara,
                                       int px, int py, int dx, int dy) {
    for (int i = 0; i < MASCARA_TAMANIO; i++) {
        for (int j = 0; j < MASCARA_TAMANIO; j++) {
            int bit_src = 15 - i * 4 - j;
            if (!((mascara >> bit_src) & 1u)) continue;

            int fila_abs = py + dy + i;
            int col_abs  = px + dx + j;

            if (col_abs  < 0)        return true;
            if (col_abs  >= t->ancho) return true;
            if (fila_abs >= t->alto)  return true;

            if (fila_abs >= 0) {
                int bit_tablero = t->ancho - 1 - col_abs;
                if ((t->filas[fila_abs] >> bit_tablero) & 1u)
                    return true;
            }
        }
    }
    return false;
}

// ------------------------------------------------------------
bool hayColision(Tablero* t, Pieza* pieza, int dx, int dy) {
    if (t == nullptr || pieza == nullptr) return true;
    return colisionConMascaraYOffset(t, pieza->mascara,
                                     pieza->px, pieza->py, dx, dy);
}

// ------------------------------------------------------------
bool hayColisionConMascara(Tablero* t, Pieza* pieza,
                            unsigned short mascara, int dx, int dy) {
    if (t == nullptr || pieza == nullptr) return true;
    return colisionConMascaraYOffset(t, mascara,
                                     pieza->px, pieza->py, dx, dy);
}

// ------------------------------------------------------------
void moverIzquierda(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;
    if (!hayColision(t, pieza, -1, 0)) pieza->px--;
}

// ------------------------------------------------------------
void moverDerecha(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;
    if (!hayColision(t, pieza, +1, 0)) pieza->px++;
}

// ------------------------------------------------------------
bool bajarPieza(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return false;
    if (hayColision(t, pieza, 0, +1)) {
        fijarPieza(t, pieza);
        return true;
    }
    pieza->py++;
    return false;
}

// ------------------------------------------------------------
//  girarPieza — prueba la rotación en copia temporal antes
//  de aplicarla. Si colisiona, no rota.
// ------------------------------------------------------------
void girarPieza(Tablero* t, Pieza* pieza) {
    if (pieza == nullptr) return;
    Pieza copia = *pieza;       // copia por valor (struct)
    rotarPieza(&copia);
    if (!hayColisionConMascara(t, pieza, copia.mascara, 0, 0))
        pieza->mascara = copia.mascara;
}

// ------------------------------------------------------------
bool esGameOver(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return true;
    return hayColision(t, pieza, 0, 0);
}
