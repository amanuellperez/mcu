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

#include "mega0_pin.h"
#include "mega0_micro.h"
#include "mega0_clock.h"

#include "mega0_usart_basic.h"
#include "mega0_uart.h"

#include "mega0_cfg.h"


/***************************************************************************
 *			    ATMEGA4809 de 40 pins
 ***************************************************************************/
namespace atmega4809_40{
    using namespace avr_;

    // Fundamental: llamar a esta función init!!!
    inline void init() { mega0_::cfg_40_pins::pins::init();}


    template <uint8_t n>
    using Pin = mega0_::Pin<n, mega0_::cfg_40_pins::pins>;

    using Micro = mega0_::Micro<mega0_::cfg_40_pins::pins>;

    using Clock_controller = mega0_::Clock_controller;

// USART
    using USART0 = mega0_::USART_basic<mega0_::cfg_40_pins::USART0>;
    using USART1 = mega0_::USART_basic<mega0_::cfg_40_pins::USART1>;
    using USART2 = mega0_::USART_basic<mega0_::cfg_40_pins::USART2>;

    using UART0_8bits = mega0_::UART_8bits<mega0_::cfg_40_pins::USART0>;
    using UART1_8bits = mega0_::UART_8bits<mega0_::cfg_40_pins::USART1>;
    using UART2_8bits = mega0_::UART_8bits<mega0_::cfg_40_pins::USART2>;

}

// La única diferencia entre estos dos namespace son los pines y la función
// init del atmega4809_40.  El resto es igual (???)
namespace atmega4809 = atmega4809_40;

#endif


