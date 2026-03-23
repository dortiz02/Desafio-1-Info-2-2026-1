QT -= gui
QT -= core

CONFIG += console
CONFIG -= app_bundle
CONFIG += c++11

TARGET = tetris

SOURCES += \
    input.cpp \
    main.cpp \
    motor.cpp \
    pieza.cpp \
    src/main.cpp \
    src/tablero.cpp \
    src/pieza.cpp \
    src/motor.cpp \
    src/input.cpp \
    src/utils.cpp \
    tablero.cpp \
    utils.cpp

HEADERS += \
    input.h \
    motor.h \
    pieza.h \
    src/tipos.h \
    src/tablero.h \
    src/pieza.h \
    src/motor.h \
    src/input.h \
    src/utils.h \
    tablero.h \
    tipos.h \
    utils.h
