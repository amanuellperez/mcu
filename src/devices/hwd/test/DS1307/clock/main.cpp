// Copyright (C) 2020 Manuel Perez 
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

#include "../../../dev_DS1307_clock.h"
#include <atd_ostream.h>
#include <cstddef>
#include <numeric>
#include <atd_cstddef.h>
#include <atd_time.h>

#include <mega.h>

#include <mcu_TWI_master.h>


// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;


// TWI Protocol
// ------------
static constexpr uint8_t TWI_buffer_size = 70; 
using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           myu::hwd::TWI,
					   TWI_buffer_size>;

using TWI = mcu::TWI_master<TWI_master_cfg>;


// Devices
// -------
using RTC = dev::DS1307_clock<TWI>;



// TODO: si lo defino con std::ostream no coge esta función sino la template
// del DS1307.h!!! ¿por qué? Debería de coger la función más especializada.
// Con todo con concepts este problema debería de desaparecer ya que la
// template quedaría sobrecargada solo para IOxtreams.
UART_iostream& print(UART_iostream& out, const RTC::Time_point& t)
{
    if (t.clock_on){
	out << "Encendido: "
	    << atd::write_as_int(t.hours) << ':'
	     << atd::write_as_int(t.minutes) << ':' 
	     << atd::write_as_int(t.seconds) << ' ';

	if (t.AMPM_format){
	    if (t.is_PM)
		out << "PM ";
	    else
		out << "AM ";
	}

	out    << atd::write_as_int(t.date) << '/'
	     << atd::write_as_int(t.month) << '/'
	     << atd::write_as_int(t.year) << "; day = " 
	     << atd::write_as_int(t.day);
    }
    else
	out << "Apagado!!!";

    return out;
}



std::ostream& operator<<(std::ostream& out, const RTC::time_point& t0)
{
    std::time_t sec = RTC::to_time_t(t0);
    std::tm t;
    ::gmtime_r(&sec, &t);

    return out << atd::only_date(t) << ' ' << atd::only_time(t);
}


void test_clock()
{
    UART_iostream uart;

    uart << "\nProbando clock\n"
	 <<   "==============\n";


// init_rtc();
    RTC rtc;

    RTC::Time_point t;

    t.hours = 16;
    t.minutes = 44;
    t.seconds = 0;

    t.date = 27;
    t.day = 0;
    t.month = 12;
    t.year= 20;


    rtc.turn_on(t);
    if (rtc.error())
	uart << "Error al intentar escribir la hora\n";


    while (1){
	atd::Date_time_view<RTC::Time_point> gt{t};

	rtc.read(t);
	print(uart, t)  << '\n';
	uart << rtc.now() << '\n';
	uart << "print_date: ";
	atd::print_date(uart, gt);
	uart << "\nprint_time: ";
	atd::print_time(uart, gt);
	uart << '\n';

	// Probar a desconectar el sensor mientras está funcionando. Tiene que
	// generar el error correspondiente.
	if (rtc.no_response())
	    uart << "Error: no response\n";

	if (rtc.error())
	    uart << "Error: error en el rtc!!!\n";

	myu::Micro::wait_ms(1000);

    }
}

int main()
{
// init_UART();
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();

// init_TWI();
    // 50 kHz es la unica frecuencia de TWI que va a 1MHz.
    // 100 kHz a 8 MHz
    TWI::turn_on<50>();

    uart << "----------------------------------------\n"
	 << "DS1307 as a system clock\n"
	 << "----------------------------------------\n\n";

    test_clock();
}




ISR(TWI_vect)
{
    TWI::handle_interrupt();
}


