// Copyright (C) 2023 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/myu
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

// Este microcontrolador dialoga con el test/TWI/slave
#include <mcu_TWI_master.h>
#include <mega.h>
#include <atd_ostream.h>    // print_int_as_hex


// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;


// Devices
// -------
constexpr uint8_t TWI_buffer_size = 10;

using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           myu::TWI_basic,
					   TWI_buffer_size>;

using TWI = mcu::TWI_master<TWI_master_cfg>;

constexpr uint8_t TWI_scl_frequency = 100; // 100 kHz


// Functions
// ---------
void twi_print_error()
{
    UART_iostream uart;

    if (TWI::no_response())
	uart << "Slave no response.\n";

    else if (TWI::eow_data_nack())
	uart << "Error: eow_data_nack\n";

    else if (TWI::prog_error())
	uart << "Error de programación\n";

    else
	uart << "Unknown error\n";
}

void twi_print_state()
{
    UART_iostream uart;

    if (TWI::error())
	uart << "state == error()\n";

    else if (TWI::no_response())
	uart << "state == no_response()\n";

    else if (TWI::prog_error())
	uart << "state == prog_error()\n";

    else if (TWI::read_or_write())
	uart << "state == read_or_write()\n";

    else if (TWI::transmitting())
	uart << "state == transmitting()\n";

    else if (TWI::eow())
	uart << "state == eow()\n";

    else if (TWI::eow_data_nack())
	uart << "state == eow_data_nack()\n";

    else if (TWI::error_buffer_size())
	uart << "state == error_buffer_size()\n";

    else if (TWI::receiving())
	uart << "state == receiving()\n";

    else if (TWI::eor())
	uart << "state == eor()\n";

    else if (TWI::eor_bf())
	uart << "state == eor_bf()\n";

    else if (TWI::ok())
	uart << "state == ok()\n";

    else
	uart << "Unknown state!!!\n";
    
}

void twi_print_state_as_hex()
{
    UART_iostream uart;
    uart << "state = ";
    atd::print_int_as_hex(uart, static_cast<uint8_t>(TWI::state()));
    uart << '\n';

}

void scan_address(TWI::Address addr)
{
    UART_iostream uart;
    uart << "Looking for slave with address " << (int) addr << " ... ";

    if (TWI::probe(addr))
	uart << "***** FOUND *****\n";
    else
	uart << "not found\n";
}

void scan_slaves()
{
    UART_iostream uart;
    uart << "\nScan for slaves\n"
	      "---------------\n"
	      "Scan from address: ";

    uint16_t from16{};
    uart >> from16;
    if (from16 > std::numeric_limits<TWI::Address>::max()){
	uart << "Too big\n";
	return;
    }

    uart << "\n    to address: ";
    uint16_t to16{};
    uart >> to16;
    uart << '\n';

    if (to16 > std::numeric_limits<TWI::Address>::max()){
	uart << "Too big\n";
	return;
    }
    
    if (from16 > to16){
	uart << "Wrong range: from has to be less or equal than to\n";
	return;
    }

    TWI::Address from = static_cast<TWI::Address>(from16);
    TWI::Address to   = static_cast<TWI::Address>(to16);

    for (; from < to; ++from){
	scan_address(from);
	Micro::wait_ms(100);
    }

    // caso en que se quiera buscar un único address (from == to)
    if (to != std::numeric_limits<TWI::Address>::max()){
	scan_address(to);
    }
}






void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void init_TWI()
{
    TWI::turn_on<TWI_scl_frequency>();
}

void hello()
{
    UART_iostream uart;
    uart << "\n\nTWI scan slaves\n"
	        "---------------\n"
		"This program scan looking for devices connected to TWI.\n"
		"Write a range of addresses to scan for a device.\n"
		"Using: SCL frequency = " << (int) TWI_scl_frequency << '\n';
}


int main() 
{
    init_uart();
    init_TWI();

    hello();

 
    while (1) {
	scan_slaves();
    }
}



ISR_TWI
{
    TWI::handle_interrupt();
}

