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

#include "../../avr_ADC.h"
#include "../../avr_USART.h"
#include "../../avr_time.h"

using namespace avr;
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


// pruebo las funciones de bajo nivel
void test_single_mode_bajo_nivel()
{
    avr::UART_iostream uart;

    // ADC::single_mode(ADC::AREF::selection_internal_to_AVCC);
//    ADC::single_mode(ADC::AREF::selection_external);
//    constexpr uint16_t AREF_en_mV = 5000;

//OJO: desconectar el pin AREF de Vcc, sino se produce un
//cortocircuito dentro del avr!!!
    ADC::single_mode(ADC::AREF::selection_internal_to_1_1V);
    constexpr uint16_t AREF_en_mV = 1100;

    ADC::select_pin<28>();

    uart << "single_mode (bajo nivel)\n\r"
	     "------------------------\n\r";


    while (1) {
	ADC::start_conversion();
	ADC::wait_until_conversion_is_complete();
	
	auto arefs = ADC::ADC_in_arefs();

	uart << "arefs = " << arefs << "; V(calibrado) = " 
		<< ADC::tomV<17321u, 348976u>(arefs) << " mV; "
		<< "V(sin cal.)= " << ADC::tomV<AREF_en_mV>(arefs)<< " mV\n\r";

	delay_ms(1000);
    }                                      
}

// Probamos las funciones de más alto nivel
void test_single_mode_alto_nivel()
{
    avr::UART_iostream uart;

    // ADC::single_mode(ADC::AREF::selection_internal_to_AVCC);
    ADC::single_mode(ADC::AREF::selection_external);
    ADC::select_pin<28>();

    uart << "single_mode (alto nivel)\n\r"
	     "------------------------\n\r";

    while (1) {
	auto v = ADC::read_in_arefs();
	uart << v << "; " << ADC::tomV<4990>(v)<< "\n\r";

	delay_ms(1000);
    }                                      
}


int main() 
{
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.on();

    test_single_mode_bajo_nivel();
//    test_single_mode_alto_nivel();
}


