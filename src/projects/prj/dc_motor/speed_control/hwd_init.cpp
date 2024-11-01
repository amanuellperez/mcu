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

#include "prj_main.h"

void Main::init_hwd()
{
    init_uart();

    priv_::PWM_pinA::init(); // TODO: PWM_pinA pertenece al motor
			     //	¿no debería de inicializarse en el motor?
			     //	Pero cuidado: L298 es dual H-bridge!!!
			     //	Observar que aparezca priv_:: da pistas de que
			     //	hay algo raro

//    Speedmeter::init(); // lo inicializa Speed_control_motor
    Speed_control_motor::init();
    Micro::enable_interrupts();
}

void Main::init_uart()
{
    myu::UART_basic_cfg();
    uart.turn_on();
}


