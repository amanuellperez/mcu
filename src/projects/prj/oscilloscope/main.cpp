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

#include <avr_timer1_generic.h>
#include <avr_time.h>
#include <avr_UART_iostream.h>
#include <avr_interrupt.h>

constexpr uint8_t pin_number = 15;
#define ISR_PCINT  ISR_PCINT_PIN15

using Timer = dev::Generic_timer_counter<avr::Timer1>;
using Pin = avr::Input_pin_without_pullup<pin_number>;


constexpr char help[] = 
"Un sensor IR recibe del mando de la TV una señal de pulsos (0-1-0-1...)\n"
"Si bien esa señal es muy fácil de ver con un osciloscopio, ¿cómo poder\n"
"detectarla con el microcontrolador?.\n"
"Este es un primer intento de medir esa señal: este programa da por supuesto\n"
"que la señal empieza en 1 (como TWI), y que es una sucesión de pulsos 0,1,0,1...\n"
"Lo que hacemos es medir el tiempo que duran esos pulsos (máximo N pulsos)\n\n";


// leemos como máximo 10 pulsos 
static constexpr int8_t imax_data = 100;
volatile Timer::counter_type data[imax_data];
volatile int8_t idata = -1;
volatile bool end_of_reading = false;

void reset_data()
{
    idata = -1;
    end_of_reading = false;
}

void print_data()
{
    avr::UART_iostream uart;
    for (int8_t i = 0; i < idata; ++i){
	if (i % 2)
	    uart <<  data[i] << "); ";
	else {
	    uart << "(" << data[i] << ", ";
	}

	if (i != 0 and (i % 7) == 0)
	    uart << '\n';
    }
    uart << '\n';
    uart << "Total number of pulses: " << idata / 2 << '\n';
}

int main()
{
    reset_data();

// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

// init_pins
    Pin::init(); // <-- al usar su interfaz static, hay
		// que inicializarlo, ya que no llamamos al constructor.

    avr::enable_all_interrupts();
    Timer::enable_overflow_interrupt();

    uart << "\n\nOscilloscope\n"
	        "------------\n"
            "Connect oscilloscope to pin "
         << uint16_t{pin_number} << '\n'
	 << "and generate a signal in that pin.\n"
	 << "Connect that pin with an external pull-up resistor\n"
	 << "Example: connect an IR receptor KY-022.\n";

    while(1){
	uart << "Waiting for value of pin " 
	     << uint16_t{pin_number} << " to be equal 1 ";

	while (Pin::is_zero()) 
	     uart << '.'; 

	uart << "OK\n"
		"Waiting pulses\n";

	Timer::on<1>();	    // leemos microsegundos
	avr::Interrupt::enable_pin<pin_number>();
	
	while (end_of_reading == false and idata < imax_data)
	{ ; }

	avr::disable_all_interrupts();
	print_data();
	
	reset_data();
	avr::enable_all_interrupts();
    }
}


ISR_PCINT {
    if (idata >= 0)
	data[idata] = Timer::value();

    Timer::reset();
    idata = idata + 1;
}



ISR_TIMER1_OVF {
    if (idata != -1)
	end_of_reading = true;
}

