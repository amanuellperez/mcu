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

#ifndef __STROBOSCOPE_MAIN_H__
#define __STROBOSCOPE_MAIN_H__


#include "dev.h"
#include "cfg.h"

/*!
 *  \brief  Application.
 *
 */
struct Main {
    Main();
    void run();

// Hardware
    LCD lcd_;
    Keyrow keyboard_;
    Stroboscope stroboscope_;

//    enum class Key{ ok, up, down, none };


// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_stroboscope() { }

// Keyboard
//    Key scan_keyboard();

//  main
    void window_main();
    void show_window_main();
    void show_window_main_off();
    void choose_freq(uint16_t freq);

};


inline void wait_release_key()
{
    my_mcu::Micro::wait_ms(time_wait_release_key);
}



#endif

