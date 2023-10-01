// Copyright (C) 2022 Manuel Perez 
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
 *  - DESCRIPCION: Órgano de juguete
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    18/12/2022 v0.0
 *
 ****************************************************************************/
#include "prj_dev.h"
//#include "prj_cfg.h"
//#include "prj_strings.h"

#include "mus_musician.h"

class Main{
public:
    Main();
    void run();
    
private:
// Hardware
    using Musician = music::Musician<SWG0_ms>;
    using Octave   = music::Octave;
    using Step	   = music::Step;

// Data


// Functions
    void init_uart();

    void musical_scale_all();
    void musical_scale_one_line();
	void musical_scale(Octave octave, uint16_t t);

    void organ_toy();
	bool organ_toy_UART_are_data_unread(const uint16_t& counter);

    void play_song();
    void play_siren();


};




#endif


