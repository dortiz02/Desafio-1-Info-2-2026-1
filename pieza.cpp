#include "pieza.h"
#include <cstdlib>   // rand, srand
#include <ctime>     // time

// ============================================================
//  Mascaras originales de los 7 figuras (sin rotar).
//  Grilla 4x4 en 16 bits.
//  [15..12]=fila0, [11..8]=fila1, [7..4]=fila2, [3..0]=fila3
//  Dentro de cada nibble: MSB = columna izquierda (col 0)
// ============================================================
static const unsigned short MASCARAS_ORIG[NUM_PIEZAS] = {
    0x0F00,  // I
    0x0660,  // O
    0x04E0,  // T
    0x06C0,  // S
    0x0C60,  // Z
    0x08E0,  // J
    0x02E0,  // L
};

static const char* NOMBRES_PIEZAS[NUM_PIEZAS] = {
    "I", "O", "T", "S", "Z", "J", "L"
};

// ------------------------------------------------------------
unsigned short obtenerMascaraOriginal(int tipo) {
    if (tipo < 0 || tipo >= NUM_PIEZAS) return 0;
    return MASCARAS_ORIG[tipo];
}

// ------------------------------------------------------------
unsigned char obtenerFilaPieza(unsigned short mascara, int fila) {
    return static_cast<unsigned char>((mascara >> (12 - fila * 4)) & 0xF);
}

// ------------------------------------------------------------
int obtenerOffsetSuperior(unsigned short mascara) {
    for (int fila = 0; fila < MASCARA_TAMANIO; fila++)
        if (obtenerFilaPieza(mascara, fila) != 0) return fila;
    return 0;
}

// ------------------------------------------------------------
int obtenerOffsetInferior(unsigned short mascara) {
    for (int fila = MASCARA_TAMANIO - 1; fila >= 0; fila--)
        if (obtenerFilaPieza(mascara, fila) != 0) return fila;
    return MASCARA_TAMANIO - 1;
}

// ------------------------------------------------------------
const char* obtenerNombrePieza(int tipo) {
    if (tipo < 0 || tipo >= NUM_PIEZAS) return "?";
    return NOMBRES_PIEZAS[tipo];
}

// ------------------------------------------------------------
Pieza* crearPiezaAleatoria(int anchoTablero) {
    static bool semillaInicializada = false;
    if (!semillaInicializada) {
        srand(static_cast<unsigned int>(time(nullptr)));
        semillaInicializada = true;
    }

    Pieza* p = new Pieza();
    p->tipo    = rand() % NUM_PIEZAS;
    p->mascara = MASCARAS_ORIG[p->tipo];
    p->px      = (anchoTablero / 2) - (MASCARA_TAMANIO / 2);
    p->py      = -obtenerOffsetSuperior(p->mascara);
    return p;
}

// ------------------------------------------------------------
//  rotarPieza — Rotacion 90° horaria en tiempo de ejecucion.
//
//  Regla: nueva[j][3-i] = original[i][j]
//  Posicion de bit de (fila, col): 15 - fila*4 - col
// ------------------------------------------------------------
void rotarPieza(Pieza* pieza) {
    if (pieza == nullptr) return;

    unsigned short original  = pieza->mascara;
    unsigned short resultado = 0;

    for (int i = 0; i < MASCARA_TAMANIO; i++) {
        for (int j = 0; j < MASCARA_TAMANIO; j++) {
            int bit_src = 15 - i * 4 - j;
            if ((original >> bit_src) & 1u) {
                int bit_dst = 15 - j * 4 - (3 - i);
                resultado |= static_cast<unsigned short>(1u << bit_dst);
            }
        }
    }
    pieza->mascara = resultado;
}

// ------------------------------------------------------------
void destruirPieza(Pieza* pieza) {
    delete pieza;
}
