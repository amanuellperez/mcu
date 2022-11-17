// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
 *  - DESCRIPCION: Programa para averiguar el protocolo usado por un mando.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    02/11/2022 v0.0
 *
 ****************************************************************************/
#include "dev.h"
#include "cfg.h"

#include "pulse.h"

class Main{
public:
    Main();
    void run();
    

private:
// Hardware: 
//    Timers timer;    <-- ahora no tiene constructor. ¿lo defino para init
//    automático?
    IR_receiver receiver;  // no lo uso, solo uso su constructor, no lo puedo comentar



// Data
    enum class Work_mode{ help, receive_data, receive_min_data, 
			  receive_raw_data};

    // Última tren de pulsos leidos:
    dev::Train_of_pulses<num_max_pulses> pulse;

    Work_mode mode = Work_mode::receive_data;

    void init_uart();

    void print_instructions();
    void read_pulses();
    void choose_mode_operation();

    void receive_data_menu();
    void receive_data() const;
    void receive_min_data() const;
    void receive_raw_data() const;
    void transmit_data();
    void replay();

// RAW (When we don't know the protocol)
    void print_raw_data() const;

    void generate_38kHz();
    void normal_operation();
};




#endif


