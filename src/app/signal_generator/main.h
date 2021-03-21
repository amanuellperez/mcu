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
struct Main {
    Main();
    void run();

// Hardware
    LCD lcd_;
    Keyboard keyboard_;
    OK_key ok_key_;
    Square_wave_generator speaker_;

    enum class Key{ ok, up, down, cancel, none };

// Data
    avr::Frequency freq_gen_;
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
    void show_first_window_sw_generator();
    void show_window_sw_generator();
    Key sw_generator_scan_keyboard();

// cfg
    void window_cfg();


// Signal generator
    void turn_on();
    void turn_off();
    void next_frequency(const avr::Frequency& f);
    void previous_frequency(const avr::Frequency& f);
    void print_without_decimals(std::ostream& out, const avr::Frequency& f);
    void print(std::ostream& out, const avr::Frequency& f);


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

