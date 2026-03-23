#include "tablero.h"
#include <iostream>

Tablero* crearTablero() {
    int ancho = 0, alto = 0;
    do {
        std::cout << "Ancho (multiplo de 8, min 8, max 32): ";
        std::cin >> ancho;
        if (ancho < 8 || ancho > 32 || ancho % 8 != 0)
            std::cout << "  Error: ancho invalido.\n";
    } while (ancho < 8 || ancho > 32 || ancho % 8 != 0);
    do {
        std::cout << "Alto (min 8): ";
        std::cin >> alto;
        if (alto < 8) std::cout << "  Error: minimo 8.\n";
    } while (alto < 8);

    Tablero* t      = new Tablero();
    t->ancho        = ancho;
    t->alto         = alto;
    t->mascaraLlena = (ancho == 32) ? 0xFFFFFFFFu : (1u << ancho) - 1u;
    t->filas        = new unsigned int[alto];
    for (int i = 0; i < alto; i++) t->filas[i] = 0u;
    std::cout << "\nTablero creado: " << ancho << " x " << alto << "\n\n";
    return t;
}
void destruirTablero(Tablero* t) {
    if (t != nullptr) { delete[] t->filas; delete t; }
}
void imprimirTablero(Tablero* t, Pieza* pieza) {
    if (t == nullptr) return;
    std::cout << '+';
    for (int x = 0; x < t->ancho; x++) std::cout << '-';
    std::cout << "+\n";
    for (int y = 0; y < t->alto; y++) {
        unsigned int fila = t->filas[y];
        if (pieza != nullptr) {
            int rel = y - pieza->py;
            if (rel >= 0 && rel < MASCARA_TAMANIO) {
                unsigned int fp = obtenerFilaPieza(pieza->mascara, rel);
                int shift = t->ancho - MASCARA_TAMANIO - pieza->px;
                fila |= (shift >= 0) ? fp << shift : fp >> (-shift);
            }
        }
        std::cout << static_cast<char>(CHAR_BORDE);
        for (int x = t->ancho - 1; x >= 0; x--)
            std::cout << static_cast<char>((fila >> x) & 1u ? CHAR_OCUPADO : CHAR_VACIO);
        std::cout << static_cast<char>(CHAR_BORDE) << '\n';
    }
    std::cout << '+';
    for (int x = 0; x < t->ancho; x++) std::cout << '-';
    std::cout << "+\n";
}
bool filaLlena(Tablero* t, int fila) {
    if (t == nullptr || fila < 0 || fila >= t->alto) return false;
    return (t->filas[fila] & t->mascaraLlena) == t->mascaraLlena;
}
void eliminarFila(Tablero* t, int fila) {
    if (t == nullptr || fila < 0 || fila >= t->alto) return;
    unsigned int* ptr = t->filas + fila;
    while (ptr > t->filas) { *ptr = *(ptr - 1); ptr--; }
    t->filas[0] = 0u;
}
void fijarPieza(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;
    for (int rel = 0; rel < MASCARA_TAMANIO; rel++) {
        int fa = pieza->py + rel;
        if (fa < 0 || fa >= t->alto) continue;
        unsigned int nibble = obtenerFilaPieza(pieza->mascara, rel);
        if (!nibble) continue;
        int shift = t->ancho - MASCARA_TAMANIO - pieza->px;
        t->filas[fa] |= (shift >= 0) ? nibble << shift : nibble >> (-shift);
    }
}

// ERROR: el for decrementa fila en todos los casos, incluso
// cuando acaba de eliminar una fila.
// Cuando se eliminan dos filas consecutivas (por ejemplo filas
// 8 y 9 estan llenas), al eliminar la fila 9 la fila que estaba
// en la posicion 8 baja a la 9. El for decrementa a 8 y la revisa,
// la elimina, y decrementa a 7. Hasta ahi bien.
// Pero si hay tres filas consecutivas llenas (8, 9, 10), al
// eliminar la 10 la fila anterior baja a esa posicion. El for
// decrementa a 9 sin revisarla de nuevo, y esa fila llena
// queda sin eliminar. El jugador no recibe los puntos y el
// tablero no se limpia correctamente.
// Se detecta llenando 3 o mas filas al mismo tiempo y viendo
// que no todas desaparecen.
int procesarFilasLlenas(Tablero* t) {
    if (t == nullptr) return 0;
    int eliminadas = 0;
    for (int fila = t->alto - 1; fila >= 0; fila--) {
        if (filaLlena(t, fila)) {
            eliminarFila(t, fila);
            eliminadas++;
            // BUG: al eliminar, la nueva fila en esta posicion
            // viene de arriba y puede estar llena tambien.
            // Hay que revisar el mismo indice de nuevo en vez
            // de decrementar. El while correcto no decrementa
            // cuando elimina.
        }
    }
    return eliminadas;
}
