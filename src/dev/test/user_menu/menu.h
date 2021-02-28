// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __MENU_H__
#define __MENU_H__

// TODO: generar esto automáticamente
// main: "a1" -> main_b
// main: "a2", &Main::a2
// ...
// menu_b: "b1", &Main::b1
// menu_b: "b2" -> main_c
// ...
//
// Y generar todo esto automáticamente.
// y que genere el const char* y los constexpr.


// poderla poner en flash
constexpr const char* menu_str[] = 
{"a1", "a2", "b1", "b2", "c1", "c2", "c3", "d1", "d2", "a3"};

// Posicion de la cadena dentro de la memoria del menu (flash o no).
constexpr size_t menu_a1 = 0;
constexpr size_t menu_a2 = 1;
constexpr size_t menu_b1 = 2;
constexpr size_t menu_b2 = 3;
constexpr size_t menu_c1 = 4;
constexpr size_t menu_c2 = 5;
constexpr size_t menu_c3 = 6;
constexpr size_t menu_d1 = 7;
constexpr size_t menu_d2 = 8;
constexpr size_t menu_a3 = 9;


using Option = dev::Option<Main>;


// REESTRICCIÓN:
// Ordenar todos los menús por el menu_id.
// De esa forma a la hora de buscarlos basta con anotar el primer elemento y
// el último y no es necesario tener que andar buscándolos aleatoriamente.
//
// Dos opciones:
//  1. Si se genera automáticamente esto la reestricción se garantiza que se
//  cumple.
//  2. Si no, crear una metafuncion que revise se garantice y si no de error
//  en tiempo de compilación.
using Options = atd::static_array<Option,
                                  Option{0, menu_a1, nullptr, 1},
                                  Option{0, menu_a2, &Main::a2, 0},
				  Option{0, menu_a3, nullptr, 3},
                                  Option{1, menu_b1, &Main::b1, 0},
                                  Option{1, menu_b2, nullptr,2},
                                  Option{2, menu_c1, &Main::c1, 0},
                                  Option{2, menu_c2, &Main::c2, 0},
                                  Option{2, menu_c3, &Main::c3, 0},
                                  Option{3, menu_d1, &Main::d1, 0},
                                  Option{3, menu_d2, &Main::d2, 0}
				  >;



#endif
