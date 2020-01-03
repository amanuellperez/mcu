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

#include "../../dev_system_clock.h"
#include <avr_interrupt.h>
#include <avr_USART.h>
#include <avr_time.h>
#include <avr_timer1_tr.h>
#include <atd_time.h>

using System_clock = dev::System_clock<avr::Timer1_CTC_mode>;

// Elegimos OCR1A para generar una señal de 1 Hz.
constexpr static uint16_t system_clock_period_in_us = 64u;
constexpr static uint16_t system_clock_ocr1a = 15625u; // 15625 miniticks*64 us/minitick = 1 seg


// Definimos el reloj del sistema
ISR_TIMER1_COMPA
{
    System_clock::tick();
}



std::ostream& operator<<(std::ostream& out, const System_clock::time_point& t0)
{
    std::time_t sec = System_clock::to_time_t(t0);
    std::tm t;
    ::gmtime_r(&sec, &t);

    return out << atd::only_date(t) << ' ' << atd::only_time(t);
}


void test_system_clock()
{
    avr::UART_iostream uart;

    while(1){
	uart << System_clock::now() << '\n';

	wait_ms(1000);
    }
}


void init_uart()
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();
}


void init_timer()
{
    System_clock::init<system_clock_period_in_us, system_clock_ocr1a>();
}


int main()
{
    init_uart();
    init_timer();

    test_system_clock();
}



