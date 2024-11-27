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

#include "mega_debug_hwd.h"
#include <atd_ostream.h>    // print_int_as_hex
#include <avr/io.h> // registros: DDRB... PORT...

namespace mega_{
namespace hwd{

template <typename Int>
static 
void print_register(std::ostream& out, const char* name, Int value)
{
    out << name << " = ";
    atd::print_int_as_hex(out, value);
    out << '\n';
}

void print_registers_timer0(std::ostream& out)
{
    print_register(out, "TCCR0A", TCCR0A);
    print_register(out, "TCCR0B", TCCR0B);
    print_register(out, "TCNT0", TCNT0);

    print_register(out, "OCR0A", OCR0A);
    print_register(out, "OCR0B", OCR0B);

    print_register(out, "TIMSK0", TIMSK0);
    print_register(out, "TIFR0", TIFR0);

    print_register(out, "GTCCR", GTCCR);

}


void print_registers_timer1(std::ostream& out)
{
    print_register(out, "TCCR1A", TCCR1A);
    print_register(out, "TCCR1B", TCCR1B);
    print_register(out, "TCCR1C", TCCR1C);

    print_register(out, "TCNT1H", TCNT1H);
    print_register(out, "TCNT1L", TCNT1L);
	
    print_register(out, "ICR1H", ICR1H);
    print_register(out, "ICR1L", ICR1L);

    print_register(out, "OCR1AH", OCR1AH);
    print_register(out, "OCR1AL", OCR1AL);

    print_register(out, "OCR1BH", OCR1BH);
    print_register(out, "OCR1BL", OCR1BL);

    print_register(out, "TIMSK1", TIMSK1);
    print_register(out, "TIFR1", TIFR1);
    
    print_register(out, "GTCCR", GTCCR);
}

void print_registers_timer2(std::ostream& out)
{
    print_register(out, "TCCR2A", TCCR2A);
    print_register(out, "TCCR2B", TCCR2B);

    print_register(out, "TCNT2", TCNT2);

    print_register(out, "OCR2A", OCR2A);
    print_register(out, "OCR2B", OCR2B);

    print_register(out, "TIMSK2", TIMSK2);
    print_register(out, "TIFR2", TIFR2);

    print_register(out, "ASSR", ASSR);

    print_register(out, "GTCCR", GTCCR);

}

void print_registers_uart(std::ostream& out)
{
    print_register(out, "UDR0  ", UDR0);

    print_register(out, "UCSR0A", UCSR0A);
    print_register(out, "UCSR0B", UCSR0B);
    print_register(out, "UCSR0C", UCSR0C);

    print_register(out, "UBRR0H", UBRR0H);
    print_register(out, "UBRR0L", UBRR0L);
}


void print_registers_adc(std::ostream& out)
{
    print_register(out, "ADMUX ", ADMUX);
    print_register(out, "ADCSRA", ADCSRA);
    print_register(out, "ADCSRB", ADCSRB);
    print_register(out, "DIDR0", DIDR0);

    // La datasheet indica leer primero el ADCL ya que bloquea, 
    // antes que ADCH (pag. 306)
    print_register(out, "ADCL", ADCL);
    print_register(out, "ADCH", ADCH);

}


void print_registers_spi(std::ostream& out)
{
    print_register(out, "SPCR", SPCR);
    print_register(out, "SPSR", SPSR);
    print_register(out, "SPDR", SPDR);
}


}// namespace
}// namespace
 
