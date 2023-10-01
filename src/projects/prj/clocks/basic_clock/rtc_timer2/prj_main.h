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
 * DESCRIPCION
 *	RTC implementado usando el Timer2 del avr conectado a un cristal
 *	externo de 32 kHz.
 *
 * HISTORIA
 *    Manuel Perez
 *    16/09/2023 Escrito
 *
 ****************************************************************************/
#include "prj_dev.h"
#include "prj_cfg.h"
//#include "prj_strings.h"


class Main{
public:
    Main();
    void run();
 
// Global vbles
    inline static volatile bool show_menu_;

private:
// Types 
    using Date_time = Clock::Date_time;

// User interface
    mcu::UART_iostream uart;


// Functions
    void init_uart();

// Main menu
    void main_hello();
    void main_menu();

// User commands


// Time
    void print_time();
    void get_time();
};



#endif


