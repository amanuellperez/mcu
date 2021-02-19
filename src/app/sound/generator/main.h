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

#ifndef __SOUND_GENERATOR_MAIN_H__
#define __SOUND_GENERATOR_MAIN_H__


#include "dev.h"
#include "cfg.h"

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
    LCD_ostream lcd_;
    Keyboard keyboard_;
    OK_key ok_key_;
    Square_wave_generator speaker_;

// Data
    avr::Hertz freq_gen_{440u};
    bool on_ = false;	    

// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_speaker();

// Window: main
    void window_main();
    void show_window_main();

// Speaker
    void turn_on();
    void turn_off();
    void freq_gen_add(uint8_t x);
    void freq_gen_substract(uint8_t x);
    void update_frequency();

};


inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}



#endif

