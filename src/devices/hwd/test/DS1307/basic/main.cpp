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

#include "../../../dev_DS1307_basic.h"	

#include <cstddef>
#include <numeric>
#include <atd_cstddef.h>
#include <atd_ostream.h>

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
                                           myu::TWI_basic,
					   TWI_buffer_size>;

using TWI = mcu::TWI_master<TWI_master_cfg>;


// Devices
// -------
using RTC = dev::DS1307_basic<TWI>;


void twi_print_state()
{
    UART_iostream uart;

    if (TWI::error())
	uart << "state == error()\n";

    else if (TWI::no_response())
	uart << "state == no_response()\n";

    else if (TWI::prog_error())
	uart << "state == prog_error()\n";

    else if (TWI::eow())
	uart << "state == eow()\n";

    else if (TWI::eow_data_nack())
	uart << "state == eow_data_nack()\n";

    else if (TWI::eor())
	uart << "state == eor()\n";

    else if (TWI::eor_bf())
	uart << "state == eor_bf()\n";

    else if (TWI::ok())
	uart << "state == ok()\n";

    else
	uart << "state DESCONOCIDO!!!\n";
    
}

void twi_print_error()
{
    UART_iostream uart;

    if (TWI::no_response())
	uart << "Slave no responde.\n";

    else if (TWI::eow_data_nack())
	uart << "Error: eow_data_nack\n";

    else if (TWI::prog_error())
	uart << "Error de programación\n";

    else if (TWI::error_buffer_size())
	uart << "Error, buffer pequeño\n";

    else if (TWI::state() == TWI::iostate::bus_error)
	uart << "bus_error!!!\n";

    else if (TWI::state() == TWI::iostate::unknown_error)
	uart << "unknown_error\n";

    else
	uart << "Error desconocido\n";
    
}


// TODO: si lo defino con std::ostream no coge esta función sino la template
// del DS1307.h!!! ¿por qué? Debería de coger la función más especializada.
// Con todo con concepts este problema debería de desaparecer ya que la
// template quedaría sobrecargada solo para IOxtreams.
UART_iostream& operator<<(UART_iostream& out,
                               const dev::__DS1307_timekeeper& t)
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


void test_clock()
{
    UART_iostream uart;

    uart << "\nProbando clock\n"
	 <<   "==============\n";


// init_rtc();
    RTC rtc;

    RTC::Time_point t;
    t.seconds = 55;
    t.minutes = 59;
//    t.AMPM_format = true;
//    t.is_PM = true;
//    t.hours   = 11;

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

	Micro::wait_ms(1000);
	++n;
	if (!(n % 10)){
	    t.clock_on = !t.clock_on;
	    rtc.write(t);
	    uart << "APAGANDO/ENCENDIENDO EL RELOJ\n";
	}

    }
}

void test_bateria()
{
    UART_iostream uart;

    uart << "\nProbando la batería auxiliar\n"
	 << "Prueba a desconectar el reloj de la batería principal y "
	 << "volver a reconectarlo.\n";


// init_rtc();
    RTC rtc;

    RTC::Time_point t;
    t.seconds = 55;
    t.minutes = 59;
//    t.AMPM_format = true;
//    t.is_PM = true;
//    t.hours   = 11;

    t.hours   = 23;
    t.day    = 0;
    t.date    = 28;
    t.month   = 2;
    t.year    = 21;

    t.clock_on = true;

    rtc.write(t);
    if (rtc.error())
	uart << "Error al intentar escribir la hora\n";

    while (1){
	rtc.read(t);
	uart << t << '\n';

	// Probar a desconectar el sensor mientras está funcionando. Tiene que
	// generar el error correspondiente.
	if (rtc.no_response())
	    uart << "Error: no response\n";

	if (rtc.error())
	    uart << "Error: error en el rtc!!!\n";

	Micro::wait_ms(1000);

    }
}


void test_ram()
{
    UART_iostream uart;

    uart << "\n\nRAM test\n"
	 << "--------\n";

// init_rtc();
    RTC rtc;

    uart << "Writing RAM ... ";
    constexpr int N = 56;
    uint8_t buf[N];
    std::iota((uint8_t*) buf, (uint8_t*) &buf[N], 0);

    uint8_t nw = rtc.ram_write(buf, N, 0);
    if (nw != N){
	uart << "Error: can't write buffer\n";
	uart << "Write " << (int) nw << " bytes out of " << (int) N << '\n';
	twi_print_state();
	twi_print_error();
    }

    if (rtc.error())
	uart << "Error: can't write RAM\n";

    uart << "OK\n";

    uart << "Reading RAM:\n";
    uint8_t res[N];
    std::fill(res, &res[N], uint8_t{0});

    if (rtc.ram_read(res, N, 0) != N)
	uart << "ERROR: can't read" << (int) N << " bytes\n";

    if (rtc.no_response())
	uart << "Error: no response\n";

    if (rtc.error())
	uart << "Error: can't read RAM\n";

    bool ok = true;
    for (uint8_t i = 0; i < N; ++i){
	if (res[i] != buf[i]){
	    uart << "wrong, ";
	    ok = false;
	}
	else
	    uart << (int)(res[i]) << ", ";
    }

    uart << '\n';
    if (ok) uart << "OK\n";
    else    uart << "FAIL\n";



    std::fill(res, &res[N], uint8_t{0});
    constexpr uint8_t nsecond = 10;
    constexpr uint8_t i0_second = 10;
    if (rtc.ram_read(res, nsecond, i0_second) != nsecond)
	uart << "ERROR: can't write " << (int) nsecond << " bytes\n";

    uart << "\nSecond lecture (partial reading):\n";
    ok = true;
    for (uint8_t i = 0; i < nsecond; ++i){
	if (res[i] != buf[i0_second + i]){
	    ok = false;
	    uart << "wrong, ";
	}
	else
	    uart << (int)(res[i]) << ", ";
    }
    uart << '\n';

    if (ok) uart << "OK\n";
    else    uart << "FAIL\n";

}



void test_output()
{
    UART_iostream uart;

    uart <<  "Probando output\n"
	 << "Conectar el osciloscopio al pin 7 y recordar haber puesto\n"
	 << "la pull-up resistor en el pin 7.\n";

    RTC rtc;

    RTC::Time_point t;
    t.seconds = 55;
    t.minutes = 59;
    t.hours   = 23;
    t.day    = 0;
    t.date    = 28;
    t.month   = 2;
    t.year    = 21;

    t.clock_on = true; // FUNDAMENTAL: RECORDAR ENCENDER EL RELOJ!!!

    rtc.write(t);
    if (rtc.error())
	uart << "Error al intentar escribir la hora\n";

    while (1){
	uart << "\n\n[0] - low\n"
	     << "[1] - high\n"
	     << "[h] - 1 Hz\n"
	     << "[4] - 4.096 kHz\n"
	     << "[8] - 8.192 kHz\n"
	     << "[3] - 32.768 kHz\n"
	     << "[r] - return\n";

	char res{};
	uart >> res;

	switch(res){
	    case '0': rtc.output_low(); break;
	    case '1': rtc.output_high(); break;
	    case 'h': rtc.output_square_wave_1Hz(); break;
	    case '4': rtc.output_square_wave_4kHz(); break;
	    case '8': rtc.output_square_wave_8kHz(); break;
	    case '3': rtc.output_square_wave_32kHz(); break;
	    case 'r':
	    case 'R': return;
	}

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

    uart << "\n\nDS1307 test\n"
	        "-----------\n";

    while(1){
	uart << "\n\nMenu\n"
		 "----\n"
		 "[b]atería auxiliar\n"
		 "[c]lock\n"
		 "[o]utput\n"
		 "[r]am\n";

	char res{};
	uart >> res;

	switch(res){
	    case 'r': case 'R': test_ram(); break;
	    case 'b': case 'B': test_bateria(); break;
	    case 'o': case 'O': test_output(); break;
	    default:
		    test_clock();
	}
    }
}




ISR(TWI_vect)
{
    TWI::handle_interrupt();
}


