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
 *  - DESCRIPCION: Reloj de horno
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    30/01/2021 v0.0
 *
 ****************************************************************************/
#include "dev.h"
#include "keyboard.h"
#include "cfg.h"

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
// Hardware
    LCD lcd_;
    Keyboard keyboard_;

// state: pasarlo a flags si se usan más de 1 flag.
    enum class State {stop, running, alarm };

// Data
    State state_ = State::stop;
    Chronometer::duration alarm_time; // Tiempo en el que lanzamos la alarma   
    
// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_chronometer();


// Window: main
    void window_main();
    void window_state_stop();
    void window_state_running();
    void window_state_alarm();

    void print_time();
};


inline void wait_release_key()
{
    mcu::Micro::wait_ms(time_wait_release_key);
}



#endif

