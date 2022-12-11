// Copyright (C) 2019-2020 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
//           https://github.com/amanuellperez/avr_
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

#include "../../avr_atmega328p_cfg.h"
#include "../../avr_ADC_basic.h"
#include "../../avr_UART.h"
#include "../../avr_time.h"


// Conectar un divisor de potencial, la salida al pin 28.
// Con un multimetro ir comparando la salida del multimetro con la que da el
// ADC.

// Conclusiones después de hacer varias pruebas:
//  1.- Si se conecta AREF a AVCC es más impreciso (el resultado es correcto
//	hasta la décima de milivoltio, pero ya no es de fiar la centésima).
//
//  2.- Si se conecta AREF a una fuente externa el resultado es más correcto
//	(hasta la centésima de voltio). La milésima de voltio no va a ser de
//	fiar ya que la resolución del ADC es 5/1024, que es más-menos 0.005 V.
//	Todas las medidas el ADC las dará de 5 mV en 5 mV!!! Esto va a ser
//	importante si se quiere medir la temperatura con un TMP36, ya que este
//	termómetro las mílesimas de voltio significan las décimas de grado.
//
//	De hecho si conectamos AREF a 5V cometemos los siguientes errores:
//	a) para potenciales pequeños: el error cometido puede llegar 
//				      a ser 40 mV.
//	b) para potenciales > 2500 mV: el error cometido anda en torno a 
//				       los 10 mV.
//
//	Para conseguir un error de +-1mV hay que calibrar el ADC.
//
//  3.- Si conecto al potencial de referencia interno 1.1V observo que este
//  potencial va variando dependiendo del potencial que queramos medir:
//  Para que funcione es fundamental conectar a un voltage regulator (por 
//  ejemplo el 7805) la fuente de alimentación. Si no oscila mucho AREF no
//  sirviendo para nada.

void wait_until_conversion_is_complete()
{while(!avr_::ADC::is_the_conversion_complete()) ;}


void print(std::ostream& out, const avr_::Potential& v)
{
    using namespace avr_::literals;

    if (v < 1_V){
	out << atd::to_integer<uint16_t>(v.internal_value()) << ' ';
	atd::print_unit(out, v);
    }
    else
	out << v;
}


template <uint8_t npin>
void test_internal_1_1V()
{
    avr_::UART_iostream uart;

    uart << "\n\nSelection internal 1.1 V\n"
	    "------------------------\n"
	    "IMPORTANTE: desconectar el pin AREF de VCC\n"
	    "si no se puede producir un cortocircuito dentro del avr!!!\n";

    uart << "\nPulsa una tecla DESPUÉS de desconectar AREF de VCC\n";

    char c{};
    uart >> c;

    uart << "El potencial máximo a leer es de 1.1 V\n";

    // encendemos el ADC en internal 1.1V a 125kHz
    avr_::ADC::AREF_internal_to_1_1V();
    avr_::ADC::clock_speed_in_kHz<125>();
    avr_::ADC::enable();

    constexpr uint16_t AREF_en_mV = 1100;

// seleccionamos pin a leer
    avr_::ADC::select_pin<npin>();

    while (1) {
	avr_::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = avr_::ADC::ADC_in_arefs();

	uart << "arefs = " << arefs  << "; V = ";
	print(uart, avr_::ADC::ADC_in_volts<AREF_en_mV>());
	uart << '\n';

	avr_::wait_ms(1000);
    }                                      
}

template <uint8_t npin>
void test_internal_AVCC()
{
    avr_::UART_iostream uart;

    uart << "\n\nSelection internal AVCC\n"
	    "------------------------\n"
	    "IMPORTANTE: desconectar el pin AREF de VCC\n"
	    "si no se puede producir un cortocircuito dentro del avr!!!\n"
	    "(TODO: revisar este comentario, en este caso no lo tengo claro\n"
	    "que sea necesario aunque no tiene mucho sentido tenerlo conectado\n"
	    "si se va a conectar internamente)\n";

    uart << "\nPulsa una tecla DESPUÉS de desconectar AREF de VCC\n";

    char c{};
    uart >> c;

    constexpr uint16_t AREF_en_mV = 5000;
    uart << "El potencial máximo a leer es de 5 V.\n";

    // encendemos el ADC en internal 1.1V a 125kHz
    avr_::ADC::AREF_internal_to_AVCC();
    avr_::ADC::clock_speed_in_kHz<125>();
    avr_::ADC::enable();


// seleccionamos pin a leer
    avr_::ADC::select_pin<npin>();

    while (1) {
	avr_::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = avr_::ADC::ADC_in_arefs();

	uart << "arefs = " << arefs  << "; V = ";
	print(uart, avr_::ADC::ADC_in_volts<AREF_en_mV>());
	uart << '\n';

	avr_::wait_ms(1000);
    }                                      
}



template <uint8_t npin>
void test_AREF_external()
{
    avr_::UART_iostream uart;

    uart << "\n\nSelection AREF external\n"
	        "-----------------------\n"
		"Conectar el pin AREF a VCC\n";

    uart << "\nPulsa una tecla DESPUÉS de conectar AREF de VCC\n";

    char c{};
    uart >> c;

    constexpr uint16_t AREF_en_mV = 5000;
    uart << "El potencial máximo a leer es de 5 V.\n";

    // encendemos el ADC en internal 1.1V a 125kHz
    avr_::ADC::AREF_external();
    avr_::ADC::clock_speed_in_kHz<125>();
    avr_::ADC::enable();


// seleccionamos pin a leer
    avr_::ADC::select_pin<npin>();

    while (1) {
	avr_::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = avr_::ADC::ADC_in_arefs();

	uart << "arefs = " << arefs  << "; V = ";
	print(uart, avr_::ADC::ADC_in_volts<AREF_en_mV>());
	uart << '\n';

	avr_::wait_ms(1000);
    }                                      
}


int main() 
{
    constexpr uint8_t npin = 28;

    avr_::UART_iostream uart;
    avr_::basic_cfg(uart);
    uart.on();

    while (1) {
        uart << "\n\nADC test\n"
                "--------\n"
		"Recordar conectar AVCC a alimentación.\n"
		"La alimentación del ADC es independiente de la del micro.\n"
		"Conectar el pin " << (int) npin << " al potenciómetro.\n"
                "1. Internal 1.1 V\n"
                "2. Internal to AVCC\n"
		"3. External to AREF\n";

        char res{};
        uart >> res;

        switch (res) {
            case '1':
                test_internal_1_1V<npin>();
                break;

	    case '2':
                test_internal_AVCC<npin>();
                break;

	    case '3': 
		test_AREF_external<npin>();
		break;
        }
    }
}

