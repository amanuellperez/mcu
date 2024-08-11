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

#include "../../dev_SDD1306.h"
#include <avr_atmega.h>
#include <mcu_TWI_master.h>
#include <mcu_TWI_master_ioxtream.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------


// TWI
// ---
constexpr uint8_t TWI_buffer_size = 10;

using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           myu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master = mcu::TWI_master<TWI_master_cfg>;
using TWI = mcu::TWI_master_ioxtream<TWI_master>;

// CUIDADO: la sabiduría interniana dice que funciona a 100kHz
// pero al hacere las primeras pruebas perdía datos.
// Para ver si se pierden datos:
//  1) pintar toda la pantalla blanca
//  2) borrarla
// Si quedan partes sin borrar es que se están perdiendo datos.
constexpr uint16_t TWI_scl_frequency = 400; // 400 kHz


// Devices
// -------
//using SDD1306 = dev::SDD1306_basic<TWI_master>;

struct SDD1306_cfg {
    using TWI_master = ::TWI_master;
    static constexpr TWI_master::Address twi_address = 0x3C;
};
using SDD1306 = dev::SDD1306_I2C_128x64<SDD1306_cfg>;

struct SDD1306_init_cfg {
};

static constexpr uint8_t control_byte = 0x00;
static constexpr uint8_t data_byte    = 0x40;



// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void init_TWI()
{
    TWI_master::turn_on<TWI_scl_frequency>();
}

void init_sdd1306()
{
    myu::UART_iostream uart;
    uart << "Connecting with SDD1306 ... ";

    if (SDD1306::probe()){
	uart << "OK\n";
	return;
    }

    // else
    uart << "FAIL!!! Can't connect with device\n";
    Micro::wait_ms(1000);

}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nSDD1306 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect SDD1306 to TWI"
		<< "\n\n";
}






void blink(uint8_t x, uint8_t page)
{
//    // Calculate the page and column
//    uint8_t page = y / 8; // Each page is 8 pixels high
			     
    for (uint8_t i = 0; i < 1; ++i){
	TWI twi{SDD1306::address};
	twi << control_byte;
	twi << uint8_t(0xB0 + page); // Set page address
	twi << uint8_t(0x00 + x); // Set lower column address
	twi << uint8_t(127); // Set higher column address
	twi.close();

	twi.open(SDD1306::address);
	twi << data_byte;
	twi << uint8_t{0xFF};
	twi.close();

	Micro::wait_ms(500);

	twi << control_byte;
	twi << uint8_t(0xB0 + page); // Set page address
	twi << uint8_t(0x00 + x); // Set lower column address
	twi << uint8_t(127); // Set higher column address
	twi.close();

	twi.open(SDD1306::address);
	twi << data_byte;
	twi << uint8_t{0x00};
	Micro::wait_ms(500);
	twi.close();
    }

}



void send_data_waiting(TWI& twi, uint8_t x, bool wait)
{
    twi << x;
    if (!wait)
	return;

    twi.close();
    Micro::wait_ms(400);
    twi.open(SDD1306::address);
    twi << data_byte;
}

void prueba()
{
    myu::UART_iostream uart;

    TWI twi(SDD1306::address);
    twi << control_byte;

//    twi << uint8_t(0x21) << uint8_t{0} << uint8_t{127}; // column add start at 0 y acaba en 127
//    twi << uint8_t{0x22} << uint8_t{0x00} << uint8_t{0x07}; // page address start at 0, end at 7
    
    twi << uint8_t(0x21) << uint8_t{0} << uint8_t{127}; // column add start at 0 y acaba en 127
    twi << uint8_t{0x22} << uint8_t{0x01} << uint8_t{0x02}; // page address start at 0, end at 7
							    
    if (twi.error())
	uart << "1.FAIL\n";

    twi.close();

    twi.open(SDD1306::address);
    twi << data_byte;

    uint16_t colmax = 10;
    bool wait = false;

    for (int j = 0; j < 2; ++j){
	for (uint16_t i = 0; i < colmax - 1; ++i){
	    send_data_waiting(twi, uint8_t{0x81}, wait);
	}

	twi << uint8_t{0xFF};

	for (uint16_t i = 0; i < colmax - 1; ++i){
	    send_data_waiting(twi, uint8_t{0x18}, wait);
	}
	twi << uint8_t{0xFF};
    }

    twi.close();
}






// Main
// ----
int main() 
{
    using Rect = dev::PageCol_rectangle;

    init_uart();
    hello();

    init_TWI();

    SDD1306::init<SDD1306_init_cfg>();

    myu::UART_iostream uart;

    while(1){
	SDD1306::fill(0xF0);
	Micro::wait_ms(700);

	SDD1306::clear();

	for (uint8_t page = 0; page < 8; ++page){
	    SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0xFF);
	    Micro::wait_ms(300);
	    SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0x00);
	}

	prueba();


	char opt{};
	uart >> opt;

	switch (opt){
	    break; default: uart << "What???\n";

	}
    }
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}


