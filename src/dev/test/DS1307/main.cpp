// Copyright (C) 2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#include "../../dev_DS1307_basic.h"
#include <avr_time.h>
#include <avr_UART.h>
#include <atd_ostream.h>
#include <cstddef>
#include <atd_cstddef.h>

// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivo TWI al que conectamos
static constexpr uint8_t TWI_buffer_size = 70; 
using TWI = avr::TWI_master<avr::TWI_basic, TWI_buffer_size>;

using RTC = dev::DS1307<TWI>;

// TODO: si lo defino con std::ostream no coge esta función sino la template
// del DS1307.h!!! ¿por qué? Debería de coger la función más especializada.
// Con todo con concepts este problema debería de desaparecer ya que la
// template quedaría sobrecargada solo para IOxtreams.
avr::UART_iostream& operator<<(avr::UART_iostream& out, const dev::__DS1307_clock& t)
{
    if (t.clock_on){
	out << "Encendido: "
	    << atd::write_as_uint8_t(t.hours) << ':'
	     << atd::write_as_uint8_t(t.minutes) << ':' 
	     << atd::write_as_uint8_t(t.seconds) << ' ';

	if (t.AMPM_format){
	    if (t.is_PM)
		out << "PM ";
	    else
		out << "AM ";
	}

	out    << atd::write_as_uint8_t(t.date) << '/'
	     << atd::write_as_uint8_t(t.month) << '/'
	     << atd::write_as_uint8_t(t.year);
    }
    else
	out << "Apagado!!!";

    return out;
}


void test_ds1307()
{
// init_UART();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    uart << "----------------------------------------\n"
	 << "DS1307\n"
	 << "----------------------------------------\n\n";

// init_TWI();
    // 50 kHz es la unica frecuencia de TWI que va a 1MHz.
    // 100 kHz a 8 MHz
    TWI::on<50>();

// init_rtc();
    RTC rtc;

    RTC::Clock t;
    t.seconds = 55;
    t.minutes = 59;
//    t.AMPM_format = true;
//    t.is_PM = true;

    t.hours   = 23;
    t.day    = 0;
    t.date    = 28;
    t.month   = 2;
    t.year    = 21;

    t.clock_on = true;

    rtc.write(t);
    if (rtc.error())
	uart << "Error al intentar escribir la hora\n";

    uint8_t n = 0;
    while (1){
	rtc.read(t);
	uart << t << '\n';

	// Probar a desconectar el sensor mientras está funcionando. Tiene que
	// generar el error correspondiente.
	if (rtc.no_response())
	    uart << "Error: no response\n";

	if (rtc.error())
	    uart << "Error: error en el rtc!!!\n";

        wait_ms(1000);
	++n;
	if (!(n % 10)){
	    t.clock_on = !t.clock_on;
	    rtc.write(t);
	    uart << "APAGANDO/ENCENDIENDO EL RELOJ\n";
	}
    }
}



int main()
{
    test_ds1307();
}




ISR(TWI_vect)
{
    TWI::handle_interrupt();
}


