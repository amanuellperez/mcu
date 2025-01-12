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

#include "../../../mega0_spi_hal.h"
#include "../../../mega0_spi_hwd.h"
#include "../../../mega0_uart.h"
#include "../../../mega0_debug.h"
#include "../../../mega0_registers.h"
#include "../../../mega0_debug.h"
#include "../../../mega0_pin_hwd.h"

#include <mcu_UART_iostream.h>

		
// Microcontroller
namespace myu = mega0_;

// Pin
template <uint8_t n>
using Pin = myu::hwd::Pin<n, myu::cfg_40_pins::pins>;

// USART
using UART_8bits = myu::UART_8bits<myu::cfg_40_pins::USART1>;
using UART_iostream = mcu::UART_iostream<UART_8bits>;

// SPI


// Configuración de SPI
struct SPI_master_cfg{
    template <uint8_t n>
    using Pin = myu::hwd::Pin<n, myu::cfg_40_pins::pins>;

    static constexpr uint8_t prescaler = 64;
    //static constexpr uint8_t prescaler = 10; // este no tiene que compilar

    // static constexpr uint32_t frequency_in_hz = (20'000'000 / 6) / prescaler;
    static constexpr uint32_t frequency_in_hz = myu::clk_per() / prescaler;
    static constexpr auto mode = myu::hal::SPI_master_cfg::normal_mode;
    //static constexpr auto mode = myu::hal::SPI_master_cfg::buffer_mode;
};


using SPI = myu::hal::SPI_master<myu::cfg_40_pins::SPI0, SPI_master_cfg>;
using SS_pin = Pin<SPI::Hwd::SS_pin>;


struct SPI_dev_cfg{
    static constexpr bool data_order_LSB = false;
    static constexpr uint8_t polarity    = 0;
    static constexpr uint8_t phase       = 0;
};

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
    uart << "\n\nSPI_master test\n"
	        "---------------\n";
    if (SPI_master_cfg::mode == myu::hal::SPI_master_cfg::normal_mode)
	uart << "Normal mode\n";
    else
	uart << "Buffer mode\n";

    uart << "frequency_in_hz = " << SPI_master_cfg::frequency_in_hz << "Hz \n";

    uart << "\nTo change from normal to buffer mode you have to recompile.\n";
}


void print_registers()
{
    UART_iostream uart;
    uart << "\nRegisters\n"
	      "---------\n";
    SPI::print_registers(uart);

}

void write()
{
    UART_iostream uart;

    uint16_t x{};

    uart << "\nWrite uint8_t: ";
    uart >> x;
    volatile uint8_t data = static_cast<uint8_t>(x);

    uart << "Transmitting data ... ";

    SS_pin::write_zero();   // client select
    
    data = SPI::write(data);

    SS_pin::write_one();    // client deselect

    uart << "OK\n";

    uart << "Received: [";
    atd::print_int_as_hex(uart, data);
    uart << "]\n";

}





// Lo configuro como el ejemplo de Microchip para probar
void spi_init()
{
//    SPI::clock_double_speed();	// observar que el host va mas rápido que el 
    SPI::init();

    SS_pin::as_output();
    SS_pin::write_one();

    SPI::cfg<SPI_dev_cfg>();
    SPI::turn_on();

}


int main()
{
    init();
 
    hello();

    spi_init();


    UART_iostream uart;

    while (1) {
	uart << "\n\nMenu\n"
		    "----\n"
		"0. print registers\n"
		"1. Transmit data\n";

	char opt{};
	uart >> opt;

	switch (opt){
	    break; case '0': print_registers();
	    break; case '1': write();
	    break; default: unkwon_option();
	}
    }
}


