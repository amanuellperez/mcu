// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_UART.h"
#include "../../../avr_timer0_generic.h"
#include "../../../avr_timer1_generic.h"
#include "../../../avr_time.h"

#include <atd_test.h>
#include <atd_ostream.h>

#include <stdlib.h>
#include <type_traits>

//#define ISR_TIMER_INTERRUPT ISR_TIMER0_COMPA
//using Timer = avr_::Timer0_generic;
#define ISR_TIMER_INTERRUPT ISR_TIMER1_COMPA
using Timer = avr_::Timer1_generic;
using time_t = uint32_t;

constexpr uint16_t period_in_us = 1024;
// constexpr uint16_t period_in_us = 256;
// DONT_COMPILE(constexpr uint16_t period_in_us = 10);
constexpr Timer::counter_type top_counter = 99;

volatile time_t contador = 0;

// TODO: quedaría mejor algo del tipo
// ISR_TIMER_INTERRUPT(avr_::Timer0) or
// ISR_TIMER_INTERRUPT(avr_::Timer1) 
// Pasándole como parámetro el Timer que usamos se elige la interrupción.
// Pero ¿cómo hacerlo sin usar macros?
//ISR_TIMER0_COMPA
ISR_TIMER_INTERRUPT
{
    // ++contador;
    contador = contador + 1;
}

 
struct time_in_days{
    uint16_t us; // microseconds: [0, 999]
    uint16_t ms; // miliseconds : [0, 999]
    uint8_t sec; // seconds     : [0, 59]
    uint8_t min; // minutes     : [0, 59]
    uint8_t hour;// hours	   : [0, 23]
    uint16_t day;// days	   : [0, 65535]
};

constexpr uint64_t num_microsegundos_tiene_1dia = 86400000000UL;

time_in_days us_to_time_in_days(uint64_t time_in_us)
{
    time_in_days t;
    t.day = time_in_us / num_microsegundos_tiene_1dia;

    uint32_t us = time_in_us % num_microsegundos_tiene_1dia;

    ldiv_t d = ldiv(us, 1000UL);

    t.us = d.rem;
    d = ldiv(d.quot, 1000UL);
    t.ms = d.rem;
    d = ldiv(d.quot, 60UL);	// Aqui quot ya entra en un uint32_t.
    t.sec = d.rem;
    d = ldiv(d.quot, 60UL);
    t.min = d.rem;
    d = ldiv(d.quot, 24UL);
    t.hour = d.rem;

    return t;
}

std::ostream& operator<<(std::ostream& out, const time_in_days& t)
{
    return out << t.day << " d "
	        << atd::write_as_uint8_t(t.hour) << " h "
		<< atd::write_as_uint8_t(t.min) << " min "
		<< atd::write_as_uint8_t(t.sec) << " s "
		<< t.ms << " ms "
		<< t.us << " us";

}

void print(uint64_t time_en_us)
{
    avr_::UART_iostream uart;
    uart << us_to_time_in_days(time_en_us) << "\n\r";
}


int main()
{
    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.on();
    test::init(uart);


    avr_::enable_interrupts();

    Timer::mode_timer_counter(top_counter);
    Timer::on<period_in_us>();


    uart << "\n\nGeneric timer test\n"
	        "------------------\n";

    CHECK_TRUE(Timer::timer_counter_top() == top_counter,
               "timer_counter_top");

    while(1){
	time_t c;
	Timer::counter_type v;
	{
	    avr_::Interrupts_lock l;
	    v = Timer::timer_counter();
	    c = contador;
	}

        uint64_t t_us =
            (c * (Timer::timer_counter_top() + 1) + v) * period_in_us;

        uart << '(' << c << "*" << (Timer::timer_counter_top() + 1) << "+"
             << atd::write_as_uint8_t(v) << ")*" << period_in_us << " = ";

	print (t_us);
//	uart << t_us << '\n';

	avr_::wait_ms(1000);
    }
}



