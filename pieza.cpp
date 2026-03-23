#include "pieza.h"
#include <cstdlib>
#include <ctime>
 
static const unsigned short MASCARAS_ORIG[NUM_PIEZAS] = {
    0x0F00, 0x0660, 0x04E0, 0x06C0, 0x0C60, 0x08E0, 0x02E0,
};
static const char* NOMBRES_PIEZAS[NUM_PIEZAS] = {
    "I","O","T","S","Z","J","L"
};
 
unsigned short obtenerMascaraOriginal(int tipo) {
    if (tipo < 0 || tipo >= NUM_PIEZAS) return 0;
    return MASCARAS_ORIG[tipo];
}
unsigned char obtenerFilaPieza(unsigned short mascara, int fila) {
    return static_cast<unsigned char>((mascara >> (12 - fila * 4)) & 0xF);
}
int obtenerOffsetSuperior(unsigned short mascara) {
    for (int f = 0; f < MASCARA_TAMANIO; f++)
        if (obtenerFilaPieza(mascara, f) != 0) return f;
    return 0;
}
int obtenerOffsetInferior(unsigned short mascara) {
    for (int f = MASCARA_TAMANIO - 1; f >= 0; f--)
        if (obtenerFilaPieza(mascara, f) != 0) return f;
    return MASCARA_TAMANIO - 1;
}
const char* obtenerNombrePieza(int tipo) {
    if (tipo < 0 || tipo >= NUM_PIEZAS) return "?";
    return NOMBRES_PIEZAS[tipo];
}
Pieza* crearPiezaAleatoria(int anchoTablero) {
    static bool ini = false;
    if (!ini) {
        srand(static_cast<unsigned int>(time(nullptr)));
        ini = true;
    }
    Pieza* p = new Pieza();
    p->tipo    = rand() % NUM_PIEZAS;
    p->mascara = MASCARAS_ORIG[p->tipo];
    p->px      = (anchoTablero / 2) - (MASCARA_TAMANIO / 2);
    p->py      = -obtenerOffsetSuperior(p->mascara);
    return p;
}
 
// ERROR: la formula del destino usa col_dest = i en vez de col_dest = 3-i.
// Rotar 90 grados horario mueve la celda (i,j) a (j, 3-i).
// Con col_dest = i se hace una transposicion sobre la diagonal,
// que no es una rotacion. Las piezas se "transforman" pero no
// coinciden con los patrones del enunciado: por ejemplo la T
// rotada no tiene la forma de T girada que se espera.
// Se detecta comparando la mascara resultante con la figura 1
// del enunciado despues de presionar W.
void rotarPieza(Pieza* pieza) {
    if (pieza == nullptr) return;
    unsigned short original  = pieza->mascara;
    unsigned short resultado = 0;
    for (int i = 0; i < MASCARA_TAMANIO; i++) {
        for (int j = 0; j < MASCARA_TAMANIO; j++) {
            int bit_src = 15 - i * 4 - j;
            if ((original >> bit_src) & 1u) {
                // BUG: deberia ser (3 - i), no i
                int bit_dst = 15 - j * 4 - i;
                resultado |= static_cast<unsigned short>(1u << bit_dst);
            }
        }
    }
    pieza->mascara = resultado;
}
 
void destruirPieza(Pieza* pieza) { delete pieza; }
