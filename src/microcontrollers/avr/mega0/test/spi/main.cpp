// Copyright (C) 2024 Manuel Perez 
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

#include "../../mega0_spi_basic.h"
#include "../../mega0_usart_basic.h"
#include "../../mega0_uart.h"
#include "../../mega0_cfg.h"
#include "../../mega0_debug.h"
#include <mcu_UART_iostream.h>

			    
namespace myu = mega0_;

// USART
using UART_8bits = myu::UART_8bits<myu::cfg_40_pins::USART1>;
using UART_iostream = mcu::UART_iostream<UART_8bits>;

// SPI
using SPI = myu::SPI_basic<myu::cfg_40_pins::SPI0>;




void unkwon_option()
{
    UART_iostream uart;
    uart << "Unkwon option\n\n";
}

void init()
{
    myu::cfg_40_pins::pins::init();
    
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void hello()
{
    UART_iostream uart;
    uart << "\n\nSPI basic test\n"
	        "--------------\n";
}

void print_registers()
{
    UART_iostream uart;
    uart << "\nRegisters\n"
	      "---------\n"
	      "TODO\n";
    print_spi_registers<SPI>(uart);

}

void data()
{
    UART_iostream uart;
    uart << "\nData\n"
	      "----\n"
	      "1. write data\n"
	      "2. read data\n";

    char opt{};
    uart >> opt;

    uint16_t x{};

    switch(opt){
	break; case '1': 
		uart << "\nWrite uint8_t: ";
		uart >> x;
		SPI::data(static_cast<uint8_t>(x));

	break; case '2': 
		uart << "\ndata = [";
		atd::print_int_as_hex(uart, SPI::data());
		uart << "]\n";

	break; default: unkwon_option();
    }

}

void data_order()
{
    UART_iostream uart;
    uart << "\nData order\n"
	    "----------\n"
	    "1. MSB first\n"
	    "2. LSB first\n";

    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::data_order_MSB();
	break; case '2': SPI::data_order_LSB();
	break; default: unkwon_option();
    }

}

void host_client_mode()
{
    UART_iostream uart;
    uart << "\n\n1. Host mode (as a master)\n"
	        "2. Client mode (as a slave)\n";

    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::host_mode();
	break; case '2': SPI::client_mode();
	break; default: unkwon_option();
    }
}

void clock_normal_double_speed()
{
    UART_iostream uart;

    uart << "\n\n1. Clock normal speed\n"
	        "2. Clock double speed\n";

    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::clock_normal_speed();
	break; case '2': SPI::clock_double_speed();
	break; default: unkwon_option();
    }
}

void clock_peripheral_prescaler()
{
    UART_iostream uart;

    uart << "\n\nClock prescaler\n"
		"---------------\n"
		"1. Divide by 2\n"
		"2. Divide by 16\n"
		"3. Divide by 64\n"
		"4. Divide by 128\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::clock_peripheral_divide_by_2();
	break; case '2': SPI::clock_peripheral_divide_by_16();
	break; case '3': SPI::clock_peripheral_divide_by_64();
	break; case '4': SPI::clock_peripheral_divide_by_128();
	break; default: unkwon_option();
    }

}

void enable_or_disable()
{
    UART_iostream uart;

    uart << "\n\n1. Enable SPI\n"
	        "2. Disable SPI\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::enable();
	break; case '2': SPI::disable();
	break; default: unkwon_option();
    }
}


void buffer_mode()
{
    UART_iostream uart;
    
    uart << "\n\nBuffer mode\n"
	        "1. Enable\n"
	        "2. Disable\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::buffer_mode_enable();
	break; case '2': SPI::buffer_mode_disable();
	break; default: unkwon_option();
    }

}


void enable_disable_ss_line()
{
    UART_iostream uart;

    uart << "\n\n1. Enable client select (SS) line\n"
	        "2. Disble client select (SS) line\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case '1': SPI::enable_client_select_line();
	break; case '2': SPI::disable_client_select_line();
	break; default: unkwon_option();
    }

}

void mode0123()
{
    UART_iostream uart;

    uart << "\n\nMode\n"
	        "----\n"
		"0. Mode 0\n"
		"1. Mode 1\n"
		"2. Mode 2\n"
		"3. Mode 3\n";


    char opt{};
    uart >> opt;
    switch(opt){
	break; case '0': SPI::mode_0();
	break; case '1': SPI::mode_1();
	break; case '2': SPI::mode_2();
	break; case '3': SPI::mode_3();
	break; default: unkwon_option();
    }

}


void run()
{
    UART_iostream uart;
    uart << "\n\nMenu\n"
	    "----\n"
	    "0. print registers\n"
	    "1. [read|write] data\n\n"
	    "CTRLA register\n"
	    "--------------\n"
	    "2. data order [MSB|LSB]\n"
	    "3. [host|client] mode\n"
	    "4. clock [normal|double] speed\n"
	    "5. clock peripheral prescaler\n"
	    "6. [enable|disable]\n"
	    "\n"
	    "CTRLB register\n"
	    "--------------\n"
	    "7. buffer mode [enable|disable]\n"
	    "8. TODO\n"
	    "9. [enable|disable] client select (SS) line\n"
	    "10. mode [0|1|2|3]\n"
	    "\n"
	    "INTCRTL register\n"
	    "----------------\n"
	    "11. TODO\n";

	uint16_t opt{};
	uart >> opt;

	switch (opt){
	    break; case 0: print_registers();
	    break; case 1: data();

	    break; case 2: data_order();
	    break; case 3: host_client_mode();
	    break; case 4: clock_normal_double_speed();
	    break; case 5: clock_peripheral_prescaler();
	    break; case 6: enable_or_disable();

	    break; case 7: buffer_mode();
	    break; case 9: enable_disable_ss_line();
	    break; case 10: mode0123();
	    
	}

}

int main()
{
    init();
 
    hello();


    while (1){
	run();
    }
}

