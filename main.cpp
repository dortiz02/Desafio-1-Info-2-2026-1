#include <iostream>

#include "tipos.h"
#include "tablero.h"
#include "pieza.h"
#include "motor.h"
#include "input.h"
#include "utils.h"

// ============================================================
//  main.cpp — Loop principal del juego.
// ============================================================

// ------------------------------------------------------------
static int calcularPuntaje(int filasEliminadas) {
    switch (filasEliminadas) {
        case 0:  return 0;
        case 1:  return 100;
        case 2:  return 300;
        case 3:  return 500;
        case 4:  return 800;
        default: return 800 + 200 * (filasEliminadas - 4);
    }
}

// ------------------------------------------------------------
static void imprimirEncabezado(int turno, int puntaje,
                                int filasTotal, Pieza* pieza) {
    std::cout << "Turno: " << turno
              << "  Puntaje: " << puntaje
              << "  Filas: "   << filasTotal << '\n';
    if (pieza != nullptr) {
        std::cout << "Pieza : " << obtenerNombrePieza(pieza->tipo)
                  << "   Pos: (" << pieza->px << ", " << pieza->py << ")\n";
    }
    std::cout << '\n';
}

// ------------------------------------------------------------
int main() {
    std::cout << "=============================\n";
    std::cout << "   TETRIS - Informatica II   \n";
    std::cout << "=============================\n\n";

    Tablero* tablero = crearTablero();
    if (tablero == nullptr) {
        std::cout << "Error critico: no se pudo crear el tablero.\n";
        return 1;
    }

    Pieza* pieza = crearPiezaAleatoria(tablero->ancho);
    if (pieza == nullptr) {
        std::cout << "Error critico: no se pudo crear la pieza inicial.\n";
        destruirTablero(tablero);
        return 1;
    }

    if (esGameOver(tablero, pieza)) {
        std::cout << "El tablero es demasiado pequeno para iniciar.\n";
        destruirPieza(pieza);
        destruirTablero(tablero);
        return 1;
    }

    bool gameOver   = false;
    int  turno      = 1;
    int  puntaje    = 0;
    int  filasTotal = 0;

    // ── Loop principal ───────────────────────────────────────
    while (!gameOver) {
        limpiarPantalla();
        imprimirEncabezado(turno, puntaje, filasTotal, pieza);
        imprimirTablero(tablero, pieza);
        std::cout << '\n';

        char accion = leerAccion();
        turno++;

        switch (accion) {
            case ACCION_IZQUIERDA:
                moverIzquierda(tablero, pieza);
                break;

            case ACCION_DERECHA:
                moverDerecha(tablero, pieza);
                break;

            case ACCION_BAJAR: {
                bool fijada = bajarPieza(tablero, pieza);
                if (fijada) {
                    int eliminadas = procesarFilasLlenas(tablero);
                    filasTotal += eliminadas;
                    puntaje    += calcularPuntaje(eliminadas);

                    if (eliminadas > 0) {
                        limpiarPantalla();
                        imprimirTablero(tablero, nullptr);
                        if (eliminadas == 4)
                            std::cout << "\n*** TETRIS! +"
                                      << calcularPuntaje(eliminadas) << " puntos ***\n";
                        else
                            std::cout << "\n+" << eliminadas << " fila"
                                      << (eliminadas > 1 ? "s" : "")
                                      << " eliminada"
                                      << (eliminadas > 1 ? "s" : "")
                                      << "! +" << calcularPuntaje(eliminadas) << " puntos\n";
                        std::cout << "Presione cualquier tecla...\n";
                        leerAccion();
                    }

                    destruirPieza(pieza);
                    pieza = crearPiezaAleatoria(tablero->ancho);

                    if (pieza == nullptr || esGameOver(tablero, pieza))
                        gameOver = true;
                }
                break;
            }

            case ACCION_ROTAR:
                girarPieza(tablero, pieza);
                break;

            case ACCION_SALIR:
                gameOver = true;
                break;

            default:
                turno--;  // tecla no reconocida: no gastar turno
                break;
        }
    }

    // ── Pantalla de Game Over ─────────────────────────────────
    limpiarPantalla();
    imprimirTablero(tablero, nullptr);
    std::cout << "\n";
    std::cout << "╔══════════════════════════╗\n";
    std::cout << "║       GAME  OVER         ║\n";
    std::cout << "╠══════════════════════════╣\n";
    std::cout << "║  Puntaje final : " << puntaje    << "\n";
    std::cout << "║  Filas totales : " << filasTotal  << "\n";
    std::cout << "║  Turnos        : " << (turno - 1) << "\n";
    std::cout << "╚══════════════════════════╝\n";

    // ── Liberar memoria ───────────────────────────────────────
    destruirPieza(pieza);
    destruirTablero(tablero);

    return 0;
}
