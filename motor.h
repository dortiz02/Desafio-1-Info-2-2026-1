#ifndef MOTOR_H
#define MOTOR_H

// ============================================================
//  motor.h
//  Logica central del juego: colisiones, movimientos,
//  gravedad y detección de Game Over.
// ============================================================

#include "tipos.h"
#include "tablero.h"
#include "pieza.h"

bool hayColision(Tablero* t, Pieza* pieza, int dx, int dy);
bool hayColisionConMascara(Tablero* t, Pieza* pieza,
                           unsigned short mascara, int dx, int dy);

void moverIzquierda(Tablero* t, Pieza* pieza);
void moverDerecha(Tablero* t, Pieza* pieza);
bool bajarPieza(Tablero* t, Pieza* pieza);
void girarPieza(Tablero* t, Pieza* pieza);
bool esGameOver(Tablero* t, Pieza* pieza);

#endif // MOTOR_H
