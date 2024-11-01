// Copyright (C) 2019-2023 Manuel Perez 
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

// Hacemos que parpadee un led. Cuando se pulsa un pulsador se duerme el avr,
// con lo que el led deja de iluminarse. Al soltar el pulsador, despierta,
// volviendo a iluminarse.
// Para ver que cualquier pin puede despertar al avr, defino 2 interrupciones.
// Funciona con los 2 pines correctamente. Probar a descomentar el pin2.
#include "../../mega_cfg.h"
#include "../../mega_sleep.h"
#include "../../mega_interrupt.h"
#include "../../mega_pin.h"
#include <avr_time.h>
#include "../../mega_UART.h"
#include <mcu_UART_iostream.h>


// Micro
// -----
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART>;
//using Micro = myu::Micro;

// Pins
// ----
constexpr uint8_t npin    = 14;

// Interrupts
// ----------
#define ISR_PIN ISR_PCINT_PIN14

// Hwd Devices
// -----------
using Pin     = myu::Pin<npin>;


void init_uart()
{
    UART_iostream uart;
    myu::UART_basic_cfg();
    uart.turn_on();

}

void init()
{
    init_uart();

    myu::enable_interrupts();

    Pin::as_input_with_pullup();
}


void select_mode()
{
    UART_iostream uart;

    uart << "\nSelect mode\n"
	      "-----------\n"
	    "1. idle mode\n"
	    "2. ADC noise reduction mode\n"
	    "3. power down mode\n"
	    "4. power save mode (to use with Timer2)\n";
    // los standby mode son para usarlos con cristal externo. 
    // TODO: al probarlos escribir las opciones de menu.

    char ans{};
    uart >> ans;

    switch(ans){
	break; case '1': myu::Sleep::idle_mode();
	break; case '2': myu::Sleep::ADC_noise_reduction_mode();
	break; case '3': myu::Sleep::power_down_mode();
	break; case '4': myu::Sleep::power_save_mode();
	break; default : uart << "Unknown option\n";
    }
}

void test_sleep()
{
    UART_iostream uart;

    uart << "\nSleeping ... \n"
	    "To wake me up:\n"
	    "\t1. press a key (only sleep idle mode)\n"
	    "\t2. change level in pin number " << (int) Pin::number << "\n";

    myu::UART_basic::enable_interrupt_unread_data();
    Pin::enable_change_level_interrupt();

// >>> sleep()
    myu::Sleep::enable();
    myu::Sleep::instruction();
    myu::Sleep::disable();
// <<< sleep()

    myu::UART_basic::disable_interrupt_unread_data();
    Pin::disable_change_level_interrupt();

    // vaciamos el buffer para que no vuelva a saltar la interrupcion
    while (myu::UART_basic::are_there_unread_data()){
	char ans{};
	uart >> ans; 
    }

    uart << "AWAKE again\n";
}


int main()
{
    init();

    UART_iostream uart;
    uart << "\nSleep teset\n"
	      "-----------\n";

    while(1){
	uart <<  "\n\nMenu\n"
		     "----\n"
		 "1. Select sleep mode\n"
		 "2. Sleep\n";

	char ans{};
	uart >> ans;

	switch(ans){
	    break; case '1': select_mode();
	    break; case '2': test_sleep();
	    break; default: uart << "Unknown option\n";
	}


    }
}

ISR_PIN{
    UART_iostream uart;
    uart << "Inside ISR_PIN!\n";

}

ISR_USART_RX {
}
