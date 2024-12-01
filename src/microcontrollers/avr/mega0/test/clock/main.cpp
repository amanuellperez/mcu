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

// (???) Este programa es genérico salvo los archivos de cabecera y el
// namespace myu. Se podría usar para probar cualquier micro???
#include "../../mega0_clock_hwd.h"
#include "../../mega0_pin_hwd.h"
#include "../../mega0.h"

#include <avr_time.h>

// HELP: conectar el osciloscopio al pin 40 del atmega4809 de 40 pins.
// y un push button en el pin push_button_pin. Al pulsar este pulsador se ira
// cambiando el divisor de frecuencia.

// Microcontroller
// ---------------
namespace myu = atmega4809_40;

// pines
constexpr uint8_t push_button_pin = 3;
// pin 40 = output pin

// Devices
// -------
using Clock = mega0_::hwd::Clock_controller;
using Push = myu::hwd::Pin<push_button_pin>;


int main()
{
    myu::init();
    Push::as_input_with_pullup();

    Clock::enable_system_clock_out();

    while (1)
    {
	// Primero se muestra la salida por defecto (con el prescaler por
	// defecto que trae, que es 6)
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	// Observar que los wait_ms no van a funcionar al cambiar la
	// frecuencia del reloj, ya que wait_ms se basa en F_CPU!!! y esa la
	// definimos en tiempo de compilación. El wait_ms lo uso para
	// debouncing.
	Clock::clk_main_divided_by_1();
	myu::wait_ms(1500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_2();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_4();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_6();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_8();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_10();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_12();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_16();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_24();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_32();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_48();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;

	Clock::clk_main_divided_by_64();
	myu::wait_ms(500); // debouncing
	while (Push::is_one()) ;



    }

}


