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
    uint16_t sw_period_ = 1; // us

// init: hardware
    void init_lcd();
    void init_keyboard() { }
    void init_signal_generator(uint16_t period_in_us);

//  main
    void window_main();
    void show_window_main();

//  sw_generator
    void window_sw_generator();
    void show_window_sw_generator();

//  sweep
    void window_sweep_menu();
    void window_sweep_run();
    bool sw_generator_run();

// cfg
    void window_cfg();


// Signal generator
    void turn_on();
    void turn_off();
    void next_frequency();
    void previous_frequency();

    // TODO: esto queda confuso, es mejor asociar directamente a cada opción
    // la función a la que llamar:
    // menu = {{"SW generator", window_sw_generator},
    //         {"sweep", window_sweep_menu},
    //         {"cfg", window_cfg}};
    static constexpr std::array window_main_menu
	    { "SW generator", "sweep", "cfg"};

    static constexpr uint8_t sw_generator_opt = 0;
    static constexpr uint8_t sweep_opt = 1;
    static constexpr uint8_t cfg_opt = 2;

    static constexpr std::array period_opts
    {"1", "8", "64", "256", "1024"};

    uint16_t index2period(uint16_t i)
    {
	switch(i){
	    case 0: return 1;
	    case 1: return 8;
	    case 2: return 64;
	    case 3: return 256;
	    case 4: return 1024;
	}

	return 1;
    }

    uint16_t period2index(uint16_t p)
    {
	switch(p){
	    case 1: return 0;
	    case 8: return 1;
	    case 64: return 2;
	    case 256: return 3;
	    case 1024: return 4;
	}

	return 0;
    }
};


inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}



#endif

