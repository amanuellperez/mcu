// Copyright (C) 2020 A.Manuel L.Perez 
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

#include "../../../dev_DS1307_basic.h"
#include <avr_time.h>
#include <avr_UART.h>
#include <atd_ostream.h>
#include <cstddef>
#include <numeric>
#include <atd_cstddef.h>

// pines que usamos
// ----------------


// dispositivos que conectamos
// ---------------------------
// Dispositivo TWI al que conectamos
static constexpr uint8_t TWI_buffer_size = 70; 
using TWI = avr::TWI_master<avr::TWI_basic, TWI_buffer_size>;

using RTC = dev::DS1307_basic<TWI>;


void twi_print_state()
{
    avr::UART_iostream uart;

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
    avr::UART_iostream uart;

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
avr::UART_iostream& operator<<(avr::UART_iostream& out,
                               const dev::__DS1307_timekeeper& t)
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
	     << atd::write_as_uint8_t(t.year) << "; day = " 
	     << atd::write_as_uint8_t(t.day);
    }
    else
	out << "Apagado!!!";

    return out;
}


void test_clock()
{
    avr::UART_iostream uart;

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

        wait_ms(1000);
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
    avr::UART_iostream uart;

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

        wait_ms(1000);

    }
}


void test_ram()
{
    avr::UART_iostream uart;

    uart << "----------------------------------------\n"
	 << "Probando la RAM\n"
	 << "----------------------------------------\n\n";

// init_rtc();
    RTC rtc;

    while (1){
	constexpr int N = 56;
	std::byte buf[N];
	std::iota((uint8_t*) buf, (uint8_t*) &buf[N], 0);

	uint8_t nw = rtc.ram_write(buf, N, 0);
	if (nw != N){
	    uart << "ERROR: no se han escrito todos los bytes\n";
	    uart << "Escritos " << atd::write_as_uint8_t(nw) << '\n';
	    twi_print_state();
	    twi_print_error();
	}

	if (rtc.error())
	    uart << "Error al intentar escribir en la RAM\n";

	std::byte res[N];
	std::fill(res, &res[N], std::byte{0});

	if (rtc.ram_read(res, N, 0) != N)
	    uart << "ERROR: no se han leído todos bytes\n";

	if (rtc.error())
	    uart << "Error al intentar leer en la RAM\n";
    
	for (uint8_t i = 0; i < N; ++i)
	    uart << std::to_integer<int>(res[i]) << ", ";

	uart << '\n';

	// Probar a desconectar el sensor mientras está funcionando. Tiene que
	// generar el error correspondiente.
	if (rtc.no_response())
	    uart << "Error: no response\n";

	if (rtc.error())
	    uart << "Error: error en el rtc!!!\n";


	std::fill(res, &res[N], std::byte{0});
	if (rtc.ram_read(res, 10, 10) != 10)
	    uart << "ERROR: no se han leído todos bytes\n";

	uart << "\nSegunda lectura:\n";
	for (uint8_t i = 0; i < N; ++i)
	    uart << std::to_integer<int>(res[i]) << ", ";

        wait_ms(5000);
    }
}



void test_output()
{
    avr::UART_iostream uart;

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
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

// init_TWI();
    // 50 kHz es la unica frecuencia de TWI que va a 1MHz.
    // 100 kHz a 8 MHz
    TWI::on<50>();

    uart << "----------------------------------------\n"
	 << "DS1307\n"
	 << "----------------------------------------\n\n";

    while(1){
	uart << "\n\nMenu:\n"
	     << "[b]atería auxiliar\n"
	     << "[c]lock\n"
	     << "[o]utput\n"
	     << "[r]am\n";

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


