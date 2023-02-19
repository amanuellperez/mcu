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


#include <avr_atmega.h>
#include <avr_SPI_generic.h>
namespace mcu = atmega; // TODO: cambiar orden. Temporal.
			
#include "../../dev_sdcard.h"


using Micro   = mcu::Micro;

// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivos SPI
using Selector_SPI = dev::SDCard_select<mcu::Output_pin<mcu::SPI::CS_pin_number>>;
using SDCard = dev::SDCard;


int main()
{
// init_UART();
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.on();

    uart << "----------------------------------------\n"
	 << "SDCard\n"
	 << "----------------------------------------\n\n";


    Selector_SPI::init();
    uint8_t res = SDCard::go_idle_state();

    uart << "res = [" << (uint16_t)(res) << "]\n";
    while(1){
    }
}





