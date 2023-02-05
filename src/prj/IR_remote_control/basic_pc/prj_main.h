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
 *  - DESCRIPCION: Programa para averiguar el protocolo usado por un mando.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    02/11/2022 v0.0
 *
 ****************************************************************************/
#include "prj_dev.h"
#include "prj_cfg.h"
#include "prj_strings.h"

#include "dev_train_of_pulses.h"
#include "dev_NEC_protocol.h"	// NEC_message

class Main{
public:
    Main();
    void run();
    
    inline static volatile bool user_abort;

private:
// Hardware


// Data
    enum class Receive_cfg{ print_pulses, print_pulses_min, 
			  print_pulses_raw};

    // Última tren de pulsos leidos:
    dev::Train_of_pulses<num_max_pulses> pulse;

    Receive_cfg mode = Receive_cfg::print_pulses;

    atd::Array<dev::NEC_message, num_max_NEC_messages> msg;

// Functions
    void init_uart();

    void print_run_menu();

    void read_pulses();

    void receive_menu();
    bool receive();
    void receive_print();
    void receive_print_help();
    bool receive_read_uart();

    void print_pulses() const;
    void print_pulses_min() const;
    void print_pulses_raw() const;
    void send_message();
    void replay();

    void research_remote_control();
    void research_remote_control_read(char* cmd);

    void clone_remote_control();
	void clone_pulses();
	bool clone_read_uart();
	void clone_msg_clear();

    void test_remote_control();

// RAW (When we don't know the protocol)
    void print_raw_data() const;

// To test
    void generate_38kHz();
};




#endif


