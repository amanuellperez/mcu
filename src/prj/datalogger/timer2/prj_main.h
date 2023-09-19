// Copyright (C) 2023 Manuel Perez 
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
 *  - DESCRIPCION: Datalogger de T
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    19/09/2023 Escrito
 *
 ****************************************************************************/
#include "prj_dev.h"
//#include "prj_cfg.h"
//#include "prj_strings.h"

#include <ostream>

class Main{
public:
    Main();
    void hello();
    void run();
    
// Global vbles
    inline static volatile bool show_menu_;

private:
// User interface
    mcu::UART_iostream uart;

// Hardware
    Sensor sensor_;

// FUNCTIONS
// init
    void init_uart();
    void init_sensor();
 
// Main menu
    void main_menu();

// Sensor
    void print_error(std::ostream& out, Sensor::Errno error);
};




#endif


