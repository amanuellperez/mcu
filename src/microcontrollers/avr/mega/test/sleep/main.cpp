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
#include "../../mega_UART_iostream.h"


// Micro
// -----
namespace my_mcu = mega_;
//using Micro = my_mcu::Micro;

// Pins
// ----
constexpr uint8_t npin    = 14;

// Interrupts
// ----------
#define ISR_PIN ISR_PCINT_PIN14

// Hwd Devices
// -----------
using Pin     = my_mcu::Pin<npin>;


void init_uart()
{
    my_mcu::UART_iostream uart;
    basic_cfg(uart);
    uart.turn_on();

}

void init()
{
    init_uart();

    my_mcu::enable_interrupts();

    Pin::as_input_with_pullup();
}


void select_mode()
{
    my_mcu::UART_iostream uart;

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
	break; case '1': my_mcu::Sleep::idle_mode();
	break; case '2': my_mcu::Sleep::ADC_noise_reduction_mode();
	break; case '3': my_mcu::Sleep::power_down_mode();
	break; case '4': my_mcu::Sleep::power_save_mode();
	break; default : uart << "Unknown option\n";
    }
}

void test_sleep()
{
    my_mcu::UART_iostream uart;

    uart << "\nSleeping ... \n"
	    "To wake me up:\n"
	    "\t1. press a key (only sleep idle mode)\n"
	    "\t2. change level in pin number " << (int) Pin::number << "\n";

    my_mcu::UART_basic::enable_interrupt_unread_data();
    Pin::enable_change_level_interrupt();

// >>> sleep()
    my_mcu::Sleep::enable();
    my_mcu::Sleep::instruction();
    my_mcu::Sleep::disable();
// <<< sleep()

    my_mcu::UART_basic::disable_interrupt_unread_data();
    Pin::disable_change_level_interrupt();

    // vaciamos el buffer para que no vuelva a saltar la interrupcion
    while (my_mcu::UART_basic::are_there_data_unread()){
	char ans{};
	uart >> ans; 
    }

    uart << "AWAKE again\n";
}


int main()
{
    init();

    my_mcu::UART_iostream uart;
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
    my_mcu::UART_iostream uart;
    uart << "Inside ISR_PIN!\n";

}

ISR_USART_RX {
}
