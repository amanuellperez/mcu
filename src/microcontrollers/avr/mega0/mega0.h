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

#pragma once

#ifndef __MEGA0_H__
#define __MEGA0_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Built-in devices que suministra cada micro de la familia mega0	
 *
 * HISTORIA
 *    Manuel Perez
 *    19/10/2024 Empezando...
 *
 ****************************************************************************/
#include "mega0_import_avr.h"

#include "mega0_pin_hwd.h"
#include "mega0_micro.h"
#include "mega0_clock_hwd.h"

#include "mega0_usart_hwd.h"
#include "mega0_uart.h"

#include "mega0_spi_hal.h"
#include "mega0_spi_hwd.h"

#include "mega0_registers.h"

#include <mcu_UART_iostream.h>  // Este lo incluyo por comodidad aquí

/***************************************************************************
 *			    ATMEGA4809 de 40 pins
 ***************************************************************************/
namespace atmega4809_40{
// cosas comunes a todos los avrs
    using namespace avr_; 
    using namespace mega0_::clocks; 

// Configuración de este chip con 40 pins
    namespace cfg = mega0_::cfg_40_pins;

    // Fundamental: llamar a esta función init!!!
    inline void init() { cfg::pins::init();}


// hwd layer
// ---------
namespace hwd{

    template <uint8_t n>
    using Pin = mega0_::hwd::Pin<n, cfg::pins>;

    using Clock_controller = mega0_::hwd::Clock_controller;

// USART
    using USART0 = mega0_::hwd::USART<cfg::USART0>;
    using USART1 = mega0_::hwd::USART<cfg::USART1>;
    using USART2 = mega0_::hwd::USART<cfg::USART2>;

// SPI
    using SPI = mega0_::hwd::SPI<cfg::SPI0>;

} // hwd

// Drivers
// -------
    using Micro = mega0_::Micro<cfg::pins>;

// UART_8bits
    using UART0_8bits = mega0_::UART_8bits<cfg::USART0>;
    using UART1_8bits = mega0_::UART_8bits<cfg::USART1>;
    using UART2_8bits = mega0_::UART_8bits<cfg::USART2>;

// SPI
    template <typename Cfg>
    using SPI_master = mega0_::hal::SPI_master<cfg::SPI0, Cfg>;

    using SPI_master_cfg = mega0_::hal::SPI_master_cfg;
}

// La única diferencia entre estos dos namespace son los pines y la función
// init del atmega4809_40.  El resto es igual (???)
namespace atmega4809 = atmega4809_40;

#endif


