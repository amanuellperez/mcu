// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// Ejemplo básico de uso del Timer como contador
#include "../../../avr_timer1_basic.h"
#include "../../../avr_time.h"

#include <stdlib.h>
#include <std_type_traits.h>

using namespace avr;

using Timer = Timer1_fast_PWM;

//constexpr uint16_t period_in_us = 1024;
constexpr uint16_t period_in_us = 64;


int main()
{
//    UART_ostream uart;
    uint16_t ocr1a = 300;
    uint16_t ocr1b = 300;

    Timer::top_ICR(1000);
    Timer::output_compare_register_A(ocr1a);
    Timer::output_compare_register_B(ocr1b);
    Timer::pin_A_non_inverting_mode();
    Timer::pin_B_inverting_mode();
    // Si cambias non_inverting_mode por inverting, la señal se invierte.
//    Timer::inverting_mode();

    Timer::on<period_in_us>();

    while(1){
	wait_ms(5000);
    }
}



