// Copyright (C) 2021 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcu
//
// This file is part of the MCU++ Library.
//
// MCU++ Library is a free library: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.


#pragma once

#ifndef __CFG_H__
#define __CFG_H__

#include <array>
#include <math.h>
#include "calc_math.h" // Log...
#include "symbol.h"

// Resolución de la calculadora
constexpr int double_ndigits = 4;

// Frecuencias (realmente periodos en ms) de muestreo
constexpr int Tclock_error = 100; // si hay error miramos el teclado cada 100ms


// Constantes, variables y funciones
// ---------------------------------
// TODO: revisar números
inline static constexpr std::array<Constant, 5> constants0 = {
    "PI",   3.14159265358979323846,
    "E",    2.71828182845904523536,
    "GAMMA",0.57721566490153286060, // Euler
    "DEG", 57.29577951308232087680, // deg/rad
    "PHI",  1.61803398874989484820, // golden-ratio
};

inline static constexpr std::array<Built_in_function, 8> builtins0 = {
    "sin", sin,
    "cos", cos,
    "tan", tan,
    "ln", Log,		// checks argument
    "log", Log10,	// checks argument
    "exp", Exp,		// checks argument
    "s", Sqrt,	// checks argument
    "int", integer,
};


// Registros de memoria
inline static constexpr std::array<Variable, 4> variables0 = {
    "A", 0,
    "B", 0,
    "C", 0,
    "D", 0
};


#endif
