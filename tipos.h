#ifndef TIPOS_H
#define TIPOS_H

// ============================================================
//  tipos.h
//  Tipos base y constantes compartidas por todo el proyecto.
// ============================================================

// Número de tipos de piezas
#define NUM_PIEZAS 7

// Tamaño de la mascara de cada pieza (grilla 4x4 en un unsigned short)
#define MASCARA_TAMANIO 4

// Acciones del usuario
#define ACCION_IZQUIERDA 'a'
#define ACCION_DERECHA   'd'
#define ACCION_BAJAR     's'
#define ACCION_ROTAR     'w'
#define ACCION_SALIR     'q'

// Caracteres de visualizacion
#define CHAR_OCUPADO '#'
#define CHAR_VACIO   '.'
#define CHAR_BORDE   '|'

#endif // TIPOS_H
