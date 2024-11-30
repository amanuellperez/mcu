// Copyright (C) 2019-2020 Manuel Perez 
//           mail: <manuel2perez@proton.me>
//           https://github.com/amanuellperez/mcuG
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

// Reloj de sistema básico. Me baso en time.h
#include "../../../mega_cfg_hwd.h"
#include "../../../mega_UART_hal.h"
#include <mcu_UART_iostream.h>
#include "../../../mega_timer1_hwd.h"
#include <avr_time.h>
#include "../../../mega_interrupt.h"
#include "../../../mega_micro.h"

#include <time.h>

namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::hal::UART_8bits>;

using Timer = hwd::Timer1;


ISR_TIMER1_COMPA
{
    system_tick();
}

 
std::ostream& operator<<(std::ostream& out, const tm& t)
{
    return out << t.tm_mday << '/'
		<< t.tm_mon << '/'
		<< 1900 + t.tm_year << ' '
	        << t.tm_hour << ':'
		<< t.tm_min << ':'
		<< t.tm_sec;

}

void print_time()
{
    UART_iostream uart;
    time_t sec = time(nullptr);
    tm* t = gmtime(&sec);

    uart << *t << "\n\r";
}

void init_time()
{
    struct tm t;
    t.tm_sec = 30;
    t.tm_min = 3;
    t.tm_hour = 15;
    t.tm_mday = 2;
    t.tm_mon = 8;
    t.tm_year = 2019 - 1900;
    t.tm_isdst = 0;

    set_system_time(mktime(&t));
}


int main()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

    Timer::clock_frequency_divide_by_64();
    Timer::enable_output_compare_A_match_interrupt();
    myu::enable_interrupts();

    // Elegimos OCR1A para generar una señal de 1 Hz
    // El osciloscopio la marca de 996ms. Hay que calibrar el número o usar un
    // cristal externo.
    uint16_t ocr1a = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


    Timer::CTC_mode_top_OCRA();
    {
	myu::Disable_interrupts l;
	Timer::unsafe_output_compare_register_A(ocr1a);
    }

    init_time();
    Timer::clock_frequency_divide_by_64();

    while(1){
	myu::wait_ms(1000);
	print_time();
    }
}
