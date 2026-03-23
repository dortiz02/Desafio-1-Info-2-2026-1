#include "utils.h"
#include <iostream>

// ------------------------------------------------------------
void limpiarPantalla() {
    std::cout << "\033[2J\033[H";
}

// ------------------------------------------------------------
void imprimirBits(unsigned int valor, int nbits) {
    for (int i = nbits - 1; i >= 0; i--)
        std::cout << ((valor >> i) & 1 ? '1' : '0');
    std::cout << '\n';
}

// ------------------------------------------------------------
void imprimirMascara4x4(unsigned short mascara) {
    std::cout << "Mascara 4x4:\n";
    for (int fila = 0; fila < 4; fila++) {
        unsigned char bits = static_cast<unsigned char>(
            (mascara >> (12 - fila * 4)) & 0xF);
        for (int col = 3; col >= 0; col--)
            std::cout << ((bits >> col) & 1 ? '#' : '.');
        std::cout << '\n';
    }
}

// ------------------------------------------------------------
char aMinuscula(char c) {
    if (c >= 'A' && c <= 'Z')
        return static_cast<char>(c + ('a' - 'A'));
    return c;
}
