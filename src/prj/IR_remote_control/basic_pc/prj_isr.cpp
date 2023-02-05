// Copyright (C) 2022 Manuel Perez 
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

#include "prj_dev.h"
#include "prj_cfg.h"
#include "prj_main.h"

ISR_RECEIVER_PIN {

    Train_of_pulses_receiver::interrupt_callback();
}


// De momento solo activo esta interrupción para cuando el usuario quiera
// abortar la ejecución de un receive.
ISR_USART_RX{

    UART::disable_interrupt_unread_data();
    Main::user_abort = true;

}




