// Copyright (C) 2019-2020 A.Manuel L.Perez
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
#include "../../../avr_USART.h"
#include "../../../avr_timer1.h"
#include "../../../avr_time.h"

#include <time.h>


using namespace avr;

using Timer = Timer1_CTC_mode;

constexpr uint16_t period_in_us = 64;


ISR_TIMER1_COMPA
{
    system_tick();
    // reti(); ???
}

 
UART_ostream& operator<<(UART_ostream& uart, const tm& t)
{
    return uart << t.tm_mday << '/'
		<< t.tm_mon << '/'
		<< 1900 + t.tm_year << ' '
	        << t.tm_hour << ':'
		<< t.tm_min << ':'
		<< t.tm_sec;

}

void print_time()
{
    UART_ostream uart;
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
    Timer::on<period_in_us>();
    Timer::enable_on_compareA_match();

    // Elegimos OCR1A para generar una señal de 1 Hz
    // El osciloscopio la marca de 996ms. Hay que calibrar el número o usar un
    // cristal externo.
    uint16_t ocr1a = 15625; // 15625 miniticks * 64 us/minitick = 1 seg


    Timer::top_comparadorA(ocr1a);
//    Timer::comparadorA(ocr1a);

    init_time();
    Timer::on<period_in_us>();

    while(1){
	wait_ms(1000);
	print_time();
    }
}
