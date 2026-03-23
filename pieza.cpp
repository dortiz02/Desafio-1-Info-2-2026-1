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
    // ERROR: falta restar (MASCARA_TAMANIO / 2) al calcular px.
    // La mascara ocupa 4 columnas, asi que para centrarla hay que
    // restar 2 al punto central. Sin esto la pieza aparece
    // desplazada 2 columnas a la derecha del centro real.
    // Se detecta visualmente: todas las piezas salen corridas
    // hacia la derecha.
    p->px = anchoTablero / 2;
    p->py = -obtenerOffsetSuperior(p->mascara);
    return p;
}
 
void rotarPieza(Pieza* pieza) { (void)pieza; } // TODO hito 4
void destruirPieza(Pieza* pieza) { delete pieza; }
 
