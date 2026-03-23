#include "tablero.h"
#include <iostream>   // cout, cin

// ------------------------------------------------------------
Tablero* crearTablero() {
    int ancho = 0, alto = 0;

    do {
        std::cout << "Ingrese el ancho del tablero (multiplo de 8, min 8, max 32): ";
        std::cin  >> ancho;
        if (ancho < 8)
            std::cout << "  Error: el ancho minimo es 8.\n";
        else if (ancho > 32)
            std::cout << "  Error: el ancho maximo soportado es 32.\n";
        else if (ancho % 8 != 0)
            std::cout << "  Error: el ancho debe ser multiplo de 8.\n";
    } while (ancho < 8 || ancho > 32 || ancho % 8 != 0);

    do {
        std::cout << "Ingrese el alto del tablero (minimo 8): ";
        std::cin  >> alto;
        if (alto < 8)
            std::cout << "  Error: el alto minimo es 8.\n";
    } while (alto < 8);

    Tablero* t    = new Tablero();
    t->ancho      = ancho;
    t->alto       = alto;
    t->mascaraLlena = (ancho == 32) ? 0xFFFFFFFFu : (1u << ancho) - 1u;
    t->filas      = new unsigned int[alto];

    for (int i = 0; i < alto; i++)
        t->filas[i] = 0u;

    std::cout << "\nTablero creado: " << ancho << " x " << alto << "\n\n";
    return t;
}

// ------------------------------------------------------------
void destruirTablero(Tablero* t) {
    if (t != nullptr) {
        delete[] t->filas;
        delete t;
    }
}

// ------------------------------------------------------------
//  imprimirTablero — imprime bit a bit, superponiendo la pieza
//  activa sobre una copia de la fila (no modifica t->filas).
// ------------------------------------------------------------
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
                unsigned int fp    = obtenerFilaPieza(pieza->mascara, rel);
                int          shift = t->ancho - MASCARA_TAMANIO - pieza->px;
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

// ------------------------------------------------------------
bool filaLlena(Tablero* t, int fila) {
    if (t == nullptr || fila < 0 || fila >= t->alto) return false;
    return (t->filas[fila] & t->mascaraLlena) == t->mascaraLlena;
}

// ------------------------------------------------------------
//  eliminarFila — desplaza filas hacia abajo con aritmética
//  de punteros, deja fila 0 vacía.
// ------------------------------------------------------------
void eliminarFila(Tablero* t, int fila) {
    if (t == nullptr || fila < 0 || fila >= t->alto) return;

    unsigned int* ptr = t->filas + fila;
    while (ptr > t->filas) {
        *ptr = *(ptr - 1);
        ptr--;
    }
    t->filas[0] = 0u;
}

// ------------------------------------------------------------
//  fijarPieza — OR bit a bit de cada nibble de la pieza
//  sobre la fila correspondiente del tablero.
// ------------------------------------------------------------
void fijarPieza(Tablero* t, Pieza* pieza) {
    if (t == nullptr || pieza == nullptr) return;

    for (int rel = 0; rel < MASCARA_TAMANIO; rel++) {
        int fila_abs = pieza->py + rel;
        if (fila_abs < 0 || fila_abs >= t->alto) continue;

        unsigned int nibble = obtenerFilaPieza(pieza->mascara, rel);
        if (nibble == 0u) continue;

        int          shift     = t->ancho - MASCARA_TAMANIO - pieza->px;
        unsigned int bits_fila = (shift >= 0) ? nibble << shift : nibble >> (-shift);
        t->filas[fila_abs] |= bits_fila;
    }
}

// ------------------------------------------------------------
//  procesarFilasLlenas — barre de abajo hacia arriba,
//  re-revisa la misma posicion al eliminar.
// ------------------------------------------------------------
int procesarFilasLlenas(Tablero* t) {
    if (t == nullptr) return 0;

    int eliminadas = 0;
    int fila       = t->alto - 1;

    while (fila >= 0) {
        if (filaLlena(t, fila)) {
            eliminarFila(t, fila);
            eliminadas++;
        } else {
            fila--;
        }
    }
    return eliminadas;
}
