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

#include "../../mcu_train_of_pulses.h"
#include "../../mcu_miniclock.h"

#include <atd_ostream.h>
#include <avr_atmega.h>	

// Cfg
// ---
static constexpr int16_t nmax_pulses = 64;

// Microcontroller
// ----------------
namespace myu = atmega; 
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// Pin connections
// ---------------
constexpr uint8_t test_pin = 15;
#define ISR_RECEIVER_PIN ISR_PCINT_PIN15


// Devices
// -------
using Miniclock_us = mcu::Miniclock_us<myu::Micro, myu::Time_counter1>;
using Pin = Micro::Pin<test_pin>;


// Train of pulses receiver
// ------------------------
using Train_isr_cfg =
    mcu::Train_of_pulses_isr_receiver_cfg<Micro,
				      Miniclock_us,
				      test_pin, 
				      nmax_pulses>;

using Train_of_pulses_isr_receiver = mcu::Train_of_pulses_isr_receiver<Train_isr_cfg>;

using Cfg_polling =
    mcu::Train_of_pulses_poll_receiver_cfg<Micro,
				      test_pin, 
				      nmax_pulses>;

using Train_of_pulses_poll_receiver = 
	    mcu::Train_of_pulses_poll_receiver<Cfg_polling>;

using Train_of_pulses = mcu::Train_of_pulses<nmax_pulses>;


// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void print_received(const Train_of_pulses& pulse)
{
    UART_iostream uart;
    uart << "\n\nReceived " << pulse.size() << " pulses:\n"
	        "---------"    "-----"    "--------\n";

    uart << "Polarity = ";
    if (pulse.polarity())
	uart << "1\n"
	        "\tlow\thigh";
    else
	uart << "0"
	        "\thigh\tlow";

    uart << '\n';


    for (uint8_t i = 0; i < pulse.size(); ++i){
	uart << (int) i << '\t';

	if (pulse.polarity())
	    uart << pulse[i].time_low << '\t' << pulse[i].time_high;
	else
	    uart << pulse[i].time_high << '\t' << pulse[i].time_low;

	uart << '\n';
    }
}


void receive_with_isr()
{
    UART_iostream uart;


    uart << "\n\nReceiving pulses with interrupt\n"
	    "-------------------------------\n"
	    "Only works with DHT11 if the atmega's frequency is 8MHz.\n"
	    "With 1MHz doesn't work because DHT11 send semipulses less than 25us\n"
	    "that can be read with the interrupt (right now uses 50 asm instructions)\n";


    mcu::Train_of_pulses<nmax_pulses> pulse;

    volatile bool abort = false; // no la uso
				 
    // Enviamos al DHT11 la señal para que empiece
    Pin::write_zero();
    Pin::as_output();
    Micro::wait_ms(20);
    Pin::as_input_without_pullup(); // release the bus <-- ya lo tiene
    Micro::wait_us(2); // tiempo para que pase de 0 -> 1 (tarda 0.7 us)
				    
    Train_of_pulses_isr_receiver::receive(pulse, abort);

    print_received(pulse);
}

void receive_polling()
{
    UART_iostream uart;
    uart << "\n\nReceiving polling the pin\n"
	    "-------------------------\n";

    mcu::Train_of_pulses<nmax_pulses> pulse;

    // Enviamos al DHT11 la señal para que empiece
    Pin::write_zero();
    Pin::as_output();
    Micro::wait_ms(20);
    Pin::as_input_without_pullup(); // release the bus
    Micro::wait_us(2); // tiempo para que pase de 0 -> 1 (tarda 0.7 us)

    // CUIDADO: llamar a as_input_without_pullup antes  de llamar a receive
    Train_of_pulses_poll_receiver::receive(pulse);

    print_received(pulse);
}

	      

int main()
{
    init_uart();
    Miniclock_us::init();

    UART_iostream uart;

    uart << "\n\nTrain of pulses test\n"
	        "--------------------\n"
		"Connect DHT11 to pin" << (int) test_pin 
		<< " with a pull-up resistor of 4.7k\n"
		"\nTry compiling using F_CPU=1MHz and F_CPU=8MHz\n"
		"and compare the polling method with this 2 different frequencies.\n"
		"(Don't forget to change the fuses: `make set_fast_fuse` for 8MHz\n"
		"and `make set_default_fuses` for 1MHz)\n\n";




    uart << "Waiting 1 seconds for DHT11 to start ... ";
    Micro::wait_ms(1000);
    uart << "OK\n";
    while(1){
	uart << "\nMenu\n"
	          "----\n"
		  "1. Receive using interruptions\n"
		  "2. Receive using polling\n";

	char option{};
	uart >> option;

	switch(option){
	    break; case '1': receive_with_isr();
	    break; case '2': receive_polling();
	    break; default : uart << "Unknown option\n";
	}

	}

}




ISR_RECEIVER_PIN {
    Train_of_pulses_isr_receiver::interrupt_callback();
}
