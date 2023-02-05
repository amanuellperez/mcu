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

#ifndef __MAIN_H__
#define __MAIN_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Calculadora científica
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    05/08/2021 v0.0
 *
 ****************************************************************************/
#include "dev.h"
#include <array>

#include "cfg.h"
#include "symbol.h"
#include "trace.h"

class Main {
public:
    Main();
    void run();
    
// global vbles
    static LCD lcd;  // no admite definirlo inline (???)
    inline static Buffer buffer;
    inline static double result;


    using Symbols =
        Symbol_table<builtins0.size(), constants0.size(), variables0.size()>;

    static Symbols symbol;


// gestion de errores
    enum class Error{no, yylex_read, yyerror};

    static void clear_error() {error_ = Error::no;}
    static bool no_error() {return error_ == Error::no;}
    static bool error() {return !no_error();}
    static void set_yyerror() {error_ = Error::yyerror;}
    static void set_yylex_read() {error_ = Error::yylex_read;}



private:
// Hardware
    Keyboard keyboard_;
    inline static Error error_ = Error::no;

// Data
// Necesito garantizar que las cadenas con los nombres de las funciones,
// constantes y variables ("sin", "cos", ...) son almacenadas.
// Si las defino como constexpr (en cfg.h) no tengo garantías de que en tiempo
// de ejecución los punteros que apuntan a las cadenas sigan siendo válidos.
// De hecho, al hacerlo así empezó a generar errores.
// Otra forma de hacerlo, es reservar memoria dinámica en el symbol.init()
// y almacenar todas las cadenas ahi. 
    std::array<Built_in_function, builtins0.size()> builtins_;
    std::array<Constant, constants0.size()> constants_;
    std::array<Variable, variables0.size()> variables_;


// init: hardware
    void init_SPI();
    void init_lcd();
    void init_keyboard() { }

// main
    void msg_error();
    
// pantalla de salida
    void print_result();
    void print_msg_error();

    void clear_screen() {lcd.clear();}
    void clear_output_screen();
};



#endif

