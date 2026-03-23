#include "input.h"
#include "utils.h"
#include <iostream>

// ------------------------------------------------------------
char leerAccion() {
    std::cout << "Accion: [A]Izq  [D]Der  [S]Bajar  [W]Rotar  [Q]Salir: ";
    char accion = 0;
    std::cin >> accion;
    return aMinuscula(accion);
}
