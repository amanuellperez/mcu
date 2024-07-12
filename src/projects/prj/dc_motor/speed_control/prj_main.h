// Copyright (C) 2024 Manuel Perez 
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

#ifndef __PRJ_MAIN_H__
#define __PRJ_MAIN_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Experimentando a controlar la velocidad de giro de un DC-motor.
 *
 * HISTORIA
 *    Manuel Perez
 *    05/06/2024 Escrito
 *
 ****************************************************************************/
#include "hwd_dev.h"
//#include "prj_cfg.h"
//#include "prj_strings.h"


class Main{
public:
    Main();

    void hello();

    void run();
    
// User interface 
    inline static myu::UART_iostream uart;

// Global vbles

private:
// Types    
    using Direction = Motor::Direction;

// Data

// Hardware

// FUNCTIONS
// init
    void init_hwd();
    void init_uart();
 
// Menus
    void turn();
    void measure_speed();
    void table_speed();
	void table_speed_impl(uint8_t nmes, bool verbose);
    void turn_speed_control();

// Functions
    Direction ask_direction();   
    
};

// (???) Definirlo fuera de Main? Necesario si uso interrupciones y las quiero
// trazar.
// TODO: Crear flujos: cout, cerr, y clog? Todos vinculados a UART. El
// harwador será el responsable de asignarlos (es quien sabe si se va a usar
// UART o no y cómo). Y de hecho, ¿por qué no crear cin también?
// Así cuando la interacción con el usuario sea a través del ordenador vía
// pantalla se usarían cin/cout. Si no es via PC se podrían usar para trazar
// el programa.
inline void logic_error()
{ Main::uart << "Logic error: program can't be here\n"; }

#endif


