#ifndef PIEZA_H
#define PIEZA_H

// ============================================================
//  pieza.h
//  Estructura y funciones relacionadas con las piezas.
//
//  Representación:
//    Cada pieza es una máscara de 16 bits (unsigned short)
//    que representa una grilla de 4x4.
//
//    Distribución de bits:
//      [15 14 13 12] = fila 0  (bit 15 = col 0, bit 12 = col 3)
//      [11 10  9  8] = fila 1
//      [ 7  6  5  4] = fila 2
//      [ 3  2  1  0] = fila 3
//
//    Bit = 1 → celda ocupada. Bit = 0 → celda vacía.
//
//  Las rotaciones se calculan EN TIEMPO DE EJECUCION
//  mediante operaciones bitwise. No se guardan precalculadas.
// ============================================================

#include "tipos.h"

// Identificadores de cada tipo de pieza
#define PIEZA_I 0
#define PIEZA_O 1
#define PIEZA_T 2
#define PIEZA_S 3
#define PIEZA_Z 4
#define PIEZA_J 5
#define PIEZA_L 6

struct Pieza {
    unsigned short mascara; // Estado actual 4x4 (puede estar rotada)
    int tipo;               // Tipo original (PIEZA_I, PIEZA_O, etc.)
    int px;                 // Columna de la esquina sup-izq en el tablero
    int py;                 // Fila de la esquina sup-izq (puede ser negativa al spawn)
};

// ── Prototipos ──────────────────────────────────────────────

Pieza* crearPiezaAleatoria(int anchoTablero);
void   rotarPieza(Pieza* pieza);
void   destruirPieza(Pieza* pieza);

unsigned short obtenerMascaraOriginal(int tipo);
unsigned char  obtenerFilaPieza(unsigned short mascara, int fila);
int            obtenerOffsetSuperior(unsigned short mascara);
int            obtenerOffsetInferior(unsigned short mascara);
const char*    obtenerNombrePieza(int tipo);

#endif // PIEZA_H
