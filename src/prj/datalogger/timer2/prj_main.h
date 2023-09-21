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
    
// User interface (debería de ser `volatile` ya que accedemos en las
// interrupciones)
    inline static UART uart;

// Global vbles
    inline static volatile bool show_main_menu_;

private:
// Data
    Clock::time_point time_next_alarm_;
    Clock::duration incr_alarm_;

// Hardware
    Sensor sensor_;

// FUNCTIONS
// init
    void init_uart();
    void init_sensor();
 
// Menus
    void menu_main();
    void menu_sensor();
    void menu_time();

// Clock
    Clock::time_point atomic_now();
    void print_time(std::ostream& out);
    void update_alarm();
    
// Sensor
    void print_temperature(std::ostream& out);
    void print_result(std::ostream& out, Sensor::Result error);
    Sensor::Result sensor_resolution(uint16_t res);
    
};


inline void Main::print_time(std::ostream& out)
{ out << Clock::now().time_since_epoch().count() << " s"; }

inline Clock::time_point Main::atomic_now()
{
    Micro::Disable_interrupts lock;
    return Clock::now();
}


#endif


