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

#ifndef __CHRONO_MAIN_H__
#define __CHRONO_MAIN_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Cronómetro
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    30/01/2021 Escrito
 *    15/01/2022 Reescrito
 *
 ****************************************************************************/
#include "prj_dev.h"
#include "prj_cfg.h"

#include <avr_time.h>
#include <user_time.h>

/*!
 *  \brief  Application.
 *
 */
class Main {
public:
    Main();
    void run();

private:
// Logical devices
    LCD lcd_;
    Keyboard keyboard_;


// init_hardware()
    void init_lcd();
    void init_keyboard() { }
    void init_chronometer();


// Window: main
    void window_stop();	    // cronómetro parado
    void window_reset();
    void window_running();  //      "     encendido

// Functions
    void wait_press_key();
    void print_time();
};


inline void wait_release_key()
{
    Micro::wait_ms(time_wait_release_key);
}



#endif

