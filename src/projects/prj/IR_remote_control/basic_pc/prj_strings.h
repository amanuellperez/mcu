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

#ifndef __PRJ_STRINGS__
#define __PRJ_STRINGS__

#include "prj_dev.h"


/***************************************************************************
 *				STRINGS
 ***************************************************************************/
// (RRR) Al dejar todos los menús en RAM el size del programa era:
//	    mcu-size xx.elf
//		text	   data	    bss	    dec	    hex	filename
//		8534	   1184	    600	  10318	   284e	xx.elf
//
//	 Al meter todas las strings en PROGMEM quedó:
//
//	    mcu-size xx.elf
//		text	   data	    bss	    dec	    hex	filename
//		10498	    364	    600	  11462	   2cc6	xx.elf
//
//        Observar que el data ha bajado de 1184 a 364!!! De hecho, me empezó
//        a funcionar mal el programa por culpa (creo) de un stackoverflow.
//
//
//constexpr mcu::ROM_string<1> msg_return <-- lo probé y no parece 
//    PROGMEM{"\n"};				  disminuir la memoria RAM

constexpr mcu::ROM_string<37> msg_run_menu1 
    PROGMEM{"\nMenu\n"
	    "----\n"
	    "1. Generate 38kHz on pin "};

constexpr mcu::ROM_string<107> msg_run_menu2 
    PROGMEM{"\n2. Send message\n"
	    "3. Replay\n"
	    "4. Research remote control\n"
	    "5. Clone\n"
	    "6. Test remote control\n"
	    "7. Receive messages\n"};

constexpr mcu::ROM_string<40> msg_generate_38kHz_on_msg 
    PROGMEM{"Generating 38kHz. Press a key to stop.\n"};


constexpr mcu::ROM_string<34> msg_receive_data_menu1 
    PROGMEM{"\n\nConnect the IR receiver to pin "};

constexpr mcu::ROM_string<194> msg_receive_data_menu2 
    PROGMEM{", point the TV remote to it and read the data.\n"
	    "\nMenu\n"
	    "----\n"
	    "h. Show this menu\n"
	    "1. Receive message (default)\n"
	    "2. Receive message (minimal version)\n"
	    "3. Receive and print RAW message\n"
	    "e. End reception\n\n"};


constexpr mcu::ROM_string<2> msg_ok 
    PROGMEM{"OK"};

constexpr mcu::ROM_string<4> msg_fail 
    PROGMEM{"FAIL"};

constexpr mcu::ROM_string<5> msg_error
    PROGMEM{"ERROR"};

constexpr mcu::ROM_string<3> msg_unknown
    PROGMEM{"???"};

constexpr mcu::ROM_string<60> msg_menu_replay
    PROGMEM{"\nReplay\n" 
	    "------\n"
	    "Press a button of your remote control ... "};

constexpr mcu::ROM_string<27> msg_press_a_key_to_replay 
    PROGMEM{"Press a key to replay ... "};

constexpr mcu::ROM_string<18> msg_no_data_received 
    PROGMEM{"No data received.\n"};

constexpr mcu::ROM_string<92> msg_error_buffer_full
    PROGMEM{"ERROR: se ha llenado el buffer de datos.\n"
	    "Modificar el programa aumentando `max_num_data`.\n"};

constexpr mcu::ROM_string<32> msg_line
    PROGMEM{"------------------------------\n"};

constexpr mcu::ROM_string<26> msg_total_number_of_pulses
    PROGMEM{"\nTotal number of pulses: "};

constexpr mcu::ROM_string<56> msg_print_raw_data_header1
    PROGMEM{"RAW data: (time_low, time_high) = period\n"
	    "num pulses = "};

constexpr mcu::ROM_string<12> msg_polarity
    PROGMEM{"Polarity = "};

constexpr mcu::ROM_string<50> msg_ask_NEC_message
    PROGMEM{"Message (write everything in hexadecimal 0x...):\n"};

constexpr mcu::ROM_string<10> msg_address
    PROGMEM{"Address: "};

constexpr mcu::ROM_string<19> msg_inverted_address
    PROGMEM{"Inverted address: "};

constexpr mcu::ROM_string<10> msg_command
    PROGMEM{"Command: "};

constexpr mcu::ROM_string<19> msg_inverted_command
    PROGMEM{"Inverted command: "};

constexpr mcu::ROM_string<30> msg_transmit_data
    PROGMEM{"Transmit data\n"
	    "-------------\n"};

constexpr mcu::ROM_string<41> msg_transmit_data_menu_protocol
    PROGMEM{"Protocol:\n"
	    "(1) NEC (now the only option)\n"};

constexpr mcu::ROM_string<46> msg_transmit_data_menu_first_burst
    PROGMEM{"Length of first burst:\n"
	    "(1) 9.0 ms\n"
	    "(2) 4.5 ms\n"};

constexpr mcu::ROM_string<10> msg_sending
    PROGMEM{"Sending: "};

constexpr mcu::ROM_string<146> msg_research_remote_control_help
    PROGMEM{"Research remote control\n"
	    "-----------------------\n"
	    "Write the name of the button and press the key in the remote control.\n"
	    "Write a blank line to end.\n\n"};

constexpr mcu::ROM_string<21> msg_error_reading_line
    PROGMEM{"Error reading line.\n"};

constexpr mcu::ROM_string<41> msg_test_remote_control
    PROGMEM{"Test remote control\n"
	    "-------------------\n"};

constexpr mcu::ROM_string<71> msg_clone_remote_control
    PROGMEM{"Clone remote control\n"
	    "--------------------\n"
	    "[c] Clear buffer\n"
	    "[e] Return\n"};

constexpr mcu::ROM_string<60> msg_clone_msg_full
    PROGMEM{"Buffer full. Can't add more elements. "
	    "Please clear buffer.\n"};

constexpr mcu::ROM_string<15> msg_clone_msg_clear
    PROGMEM{"Buffer empty.\n"};

#endif


