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
#include "hwd_dev.h"
//#include "prj_cfg.h"
//#include "prj_strings.h"
#include "prj_alarm.h"

#include <ostream>

class Main{
public:
    Main();

    void hello();
    void user_cfg();

    void run();
    
// User interface (debería de ser `volatile` ya que accedemos en las
// interrupciones)
    inline static UART uart;

// Global vbles
    inline static volatile bool reset_;

private:
// Types    
    using time_point = Clock::time_point;

// Data
    Alarm next_alarm_;

// Hardware
    Sensor sensor_;

// FUNCTIONS
// init
    void init_hwd();
    void init_uart();
    void init_sensor();
 
// Menus
    void menu_options();
    void menu_time();

    // Menu específico del sensor
    void menu_sensor(char option);
    void print_menu_sensor();

// Functions
    void reset();

// Info
    void print_options();

// Clock
    time_point atomic_now();
    void update_alarm();
    void change_time_settings(const uint32_t& t, const uint32_t& t_incr);

    void print_time_options() const;
    
// Sensor
    void print_data(std::ostream& out);

    void print_sensor_options();
    
};


inline Main::time_point Main::atomic_now()
{
    Micro::Disable_interrupts lock;
    return Clock::now();
}

inline void logic_error()
{ Main::uart << "Logic error: program can't be here\n"; }

inline void Main::print_time_options() const
{ next_alarm_.print(uart); }

inline void Main::reset()
{ next_alarm_.reset(); }


#endif


