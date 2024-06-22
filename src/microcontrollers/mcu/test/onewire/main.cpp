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

#include "../../dev_one_wire.h"

#include <atd_ostream.h>
#include <avr_atmega.h>	

// Microcontroller
// ----------------
namespace my_mcu = atmega; 
using Micro   = my_mcu::Micro;

// Pin connections
// ---------------
constexpr uint8_t test_pin = 15;


// Devices
// -------
using Cfg      = dev::One_wire_cfg<my_mcu::Micro, test_pin>;
using One_wire = dev::One_wire<Cfg>;
using Device = dev::One_wire_device;
using Search = dev::One_wire_search<Cfg>;



// Functions
// ---------
void init_uart()
{
    my_mcu::UART_iostream uart;
    my_mcu::basic_cfg(uart);
    uart.turn_on();
}



void print_menu()
{
    my_mcu::UART_iostream uart;

    uart << "\n----------------\n"
	    "[b]asic test\n"
	    "[r]ead rom (only one device connected to the 1-wire)\n"
	    "[s]earch command\n"
	    "[D]S1820 test\n"
            "----------------\n";


    

}

void print_device_found(const Device& dev)
{
    my_mcu::UART_iostream uart;

    uart << "Device found\n";
    
    uart << "  ROM   : ";

    print_rom_as_hex(uart, dev);

    uart << "\n  Family: ";
    print_family_name(uart, dev);
    uart << '\n';

    uart << "  CRC   : ";
    if (dev.is_CRC_ok())
	uart << "OK";
    else
	uart << "WRONG";
}

void DS18B20_convert_T()
{
    my_mcu::UART_iostream uart;

    One_wire::reset();
    One_wire::write(0xCC); // Skip ROM command to select a single device

    uart << "Convert T command ... ";
    One_wire::write(0x44);

    uint16_t time_out = 0;
    while (One_wire::read_bit() == 0) {
	Micro::wait_ms(1);
	++time_out;
	if (time_out > 3000){ // esperamos 3 segundos máximo
	    uart << "TIME OUT\n";
	    return;
	}
    }

    uart << "OK\n";
}

void DS18B20_print_T(uint8_t T0, uint8_t T1)
{
    int8_t T = ((T0 & 0xF0) >> 4) | ((T1 & 0x0F) << 4);
    if (T1 & 0xF0)
	T = -T;
    
    uint8_t T_dec = (T0 & 0x0F);

    my_mcu::UART_iostream uart;
    uart << (int) T << '.' << (int) T_dec;
}


void DS18B20_print_scratchpad(const uint8_t* scratchpad)
{
    my_mcu::UART_iostream uart;

    uart << "Scratchpad\n"
	    "\tTemperature (LSB MSB = 0x50 0x05)= ";

    atd::print_int_as_hex(uart, scratchpad[0]);
    uart << ' ';
    atd::print_int_as_hex(uart, scratchpad[1]);
    uart << " ---> ";
    DS18B20_print_T(scratchpad[0], scratchpad[1]);

    uart << "\n\tTH = ";
    atd::print_int_as_hex(uart, scratchpad[3]);
    uart << "\n\tTL = ";
    atd::print_int_as_hex(uart, scratchpad[4]);
    uart << "\n\tReserved (0xFF 0x?? 0x10) = ";
    atd::print_int_as_hex(uart, scratchpad[5]);
    uart << ' ';
    atd::print_int_as_hex(uart, scratchpad[6]);
    uart << ' ';
    atd::print_int_as_hex(uart, scratchpad[7]);
    uart << "\n\tCRC = ";
    atd::print_int_as_hex(uart, scratchpad[8]);
    uart << " (";
    if (scratchpad[8] == atd::CRC8_Maxim(scratchpad, 8))
	uart << "OK";
    else
	uart << "WRONG";

    uart << ")\n";
}


void DS18B20_read_scratchpad()
{
    my_mcu::UART_iostream uart;

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
    for (uint8_t i = 0; i < scratchpad_size; ++i)
	scratchpad[i] = One_wire::read();

    DS18B20_print_scratchpad(scratchpad);
}

void DS18B20_test()
{
    my_mcu::UART_iostream uart;
    
    uart << "\nDS18B20 basic test\n"
	    "------------------\n"
	    "Basic test: connect only one DS18B20\n"
	    "Press a key to continue\n";

    char ans{};
    uart >> ans;

    DS18B20_read_scratchpad();
    DS18B20_convert_T();
    DS18B20_read_scratchpad();
}


void basic_test()
{
    my_mcu::UART_iostream uart;

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

void search_test(Search::Type type)
{
    my_mcu::UART_iostream uart;

    uint8_t n = 0;

    Search search{type};
    Device dev;

    while (search(dev)){
	++n; 

	print_device_found(dev);


	uart << "\nPress key to continue\n";
	char c{};
	uart >> c;
    }

    uart << "\nTotal devices found: " << (int) n << '\n';

}


void search_test()
{
    my_mcu::UART_iostream uart;

    uart << "\nSearch test\n"
	      "-----------\n"
	      "1. Normal search\n"
	      "2. Alarm search\n";

    char ans{};
    uart >> ans;

    if (ans == '2'){
	uart << "\nAlarm search\n";
	search_test(Search::Type::alarm_search);
    }

    else{
	uart << "\nNormal search\n";
	search_test(Search::Type::normal_search);
    }
}

void read_rom_test()
{
    my_mcu::UART_iostream uart;
    uart << "Read rom command\n";

    if (!One_wire::reset()){
	uart << "No device found\n";
	return;
    }

    Device dev;
    One_wire::read_rom(dev);

    print_device_found(dev);


}

	      

int main()
{
    init_uart();

    my_mcu::UART_iostream uart;

    uart << "\n\nOne wire test\n"
	        "-------------\n"
		"1. Connect devices to pin" << (int) test_pin 
		<< " with a pull-up resistor of 4'7k\n"
		"2. Connect all devices to power supply\n"
		"   (don't use parasite power)\n";


    while(1){
	print_menu();

	char ans{};
	uart >> ans;

	switch(ans){
	    break; case 'b':
		   case 'B': basic_test();

	    break; case 'd':
		   case 'D': DS18B20_test();


	    break; case 'r':
		   case 'R': read_rom_test();

	    break; case 's':
		   case 'S': search_test();

	    break; default: uart << "Unknown option\n";
	}

    }
}




