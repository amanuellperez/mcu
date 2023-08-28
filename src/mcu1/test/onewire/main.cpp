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

#include <avr_atmega.h>	// TODO: orden!!!
namespace mcu = atmega; // TODO: orden
using Micro = mcu::Micro;
#include <atd_ostream.h>
#include "../../dev_one_wire.h"

// Devices
// -------

constexpr uint8_t test_pin = 15;

using Pin = mcu::Pin<test_pin>;
using Cfg = dev::One_wire_cfg<mcu::Micro, Pin>;

using One_wire = dev::One_wire<Cfg>;
using Search = dev::One_wire_search<Cfg>;



// Functions
// ---------
void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();
}



void print_menu()
{
    mcu::UART_iostream uart;

    uart << "\n----------------\n";

    

}

void print_family_name(uint8_t code)
{
    mcu::UART_iostream uart;
    switch (code){
	break; case 0x28: uart << "DS18B20";
	break; default:  uart << "Unknown or wrong";
    }
}

void DS18B20_test()
{
    mcu::UART_iostream uart;
    
    uart << "\nDS18B20 basic test\n"
	    "------------------\n"
	    "Basic test: connect only one DS18B20\n"
	    "Press a key to continue\n";

    char ans{};
    uart >> ans;



    // Read the contents of the scratchpad
    static constexpr uint8_t scratchpad_size = 9;
    uint8_t scratchpad[scratchpad_size];

    if (!One_wire::reset()){
	uart << "NO devices found!!!\n";
	return;
    }
    else
	uart << "Device detected\n";

    One_wire::skip_rom();
    One_wire::write(0xBE);
    for (uint8_t i = 0; i < scratchpad_size; ++i){
	scratchpad[i] = One_wire::read();
	atd::print_int_as_hex(uart, scratchpad[i]);
	uart << ' ';
    }

    uart << '\n';

    
    One_wire::reset();
    One_wire::write(0xCC); // Skip ROM command to select a single device

    uart << "Convert T command ... ";
    One_wire::write(0x44);
    while (One_wire::read_bit() == 0)
	uart << '.';

    uart << "OK\n";


}


void basic_test()
{
    mcu::UART_iostream uart;

    uart << "\nBasic test\n"
	      "----------\n"
	    "Connect the oscilloscope to pin " << (int) test_pin << '\n' <<
	    "1. Write ones bit (at most 15 us low)\n"
	    "2. Write zero bit (at least 60 us low)\n";

    char ans{};
    uart.get(ans);

    if (ans == '2'){
	uart << "Writing zeros\n";
	while(1) One_wire::write_bit_zero();
    }

    else{
	uart << "Writing ones\n";
	while(1) One_wire::write_bit_one();
    }

}

void search_test()
{
    mcu::UART_iostream uart;

    uart << "\nSearch test\n"
	      "-----------\n";

    Search search;
    if (search.begin_search()){
	uart << "Device found\n";
	const uint8_t* ROM = search.ROM_code();

	for (uint8_t i = 0; i < Search::ROM_size; ++i){
	    atd::print_int_as_hex(uart, ROM[i]);
	    uart << ' ';
	}
	uart << '\n';
    }

    else
	uart << "No device found\n";

}

void search_iterator_test()
{
    mcu::UART_iostream uart;

    uart << "\nSearch iterators test\n"
	      "---------------------\n";
	      

    uint8_t i = 0;
    for (auto p = Search::begin(); 
	      p != Search::end(); ++p, ++i){
	uart << "Device " << (int) i << " found\n";
	
	uart << "  ROM   : ";

	const uint8_t* ROM = *p;
	for (uint8_t i = 0; i < Search::ROM_size; ++i){
	    atd::print_int_as_hex(uart, ROM[i]);
	    uart << ' ';
	}

	uart << "\n  Family: ";
	print_family_name(ROM[0]);
	uart << '\n';

	uart << "  CRC   : ";
	if (Search::verify_CRC(p.ROM_code()))
	    uart << "OK";
	else
	    uart << "WRONG";



	uart << "\nPress key to continue\n";
	char c{};
	uart >> c;
    }

    uart << "\nTotal devices found: " << (int) i << '\n';

}

int main()
{
    init_uart();

    mcu::UART_iostream uart;

    uart << "\n\nOne wire test\n"
	        "-------------\n"
		"1. Connect devices to pin" << (int) test_pin 
		<< " with a pull-up resistor of 4'7k\n"
		"2. Connect all devices to power supply\n"
		"   (don't use parasite power)\n";


    while(1){
//	print_menu();
	// basic_test();
//	search_test();
	search_iterator_test();
	// DS18B20_test();
    }
}




