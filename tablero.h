#ifndef TABLERO_H
#define TABLERO_H

// ============================================================
//  tablero.h
//  Estructura y funciones del tablero de juego.
//
//  Representación con bits:
//    Cada elemento del arreglo filas[] representa UNA FILA.
//    Cada bit de ese entero representa UNA COLUMNA:
//      bit más significativo (izquierda) = columna 0
//      bit 0 (derecha)                  = columna (ancho-1)
//    bit = 1 → celda ocupada, bit = 0 → celda vacía
// ============================================================

#include "tipos.h"
#include "pieza.h"

struct Tablero {
    unsigned int* filas;       // Arreglo dinámico de filas (new[])
    int ancho;                 // Número de columnas (múltiplo de 8)
    int alto;                  // Número de filas (mínimo 8)
    unsigned int mascaraLlena; // (1 << ancho) - 1
};

// ── Prototipos ──────────────────────────────────────────────

Tablero* crearTablero();
void     destruirTablero(Tablero* t);
void     imprimirTablero(Tablero* t, Pieza* pieza);
bool     filaLlena(Tablero* t, int fila);
void     eliminarFila(Tablero* t, int fila);
void     fijarPieza(Tablero* t, Pieza* pieza);
int      procesarFilasLlenas(Tablero* t);

#endif // TABLERO_H
