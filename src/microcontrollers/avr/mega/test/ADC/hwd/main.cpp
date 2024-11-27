// Copyright (C) 2019-2024 Manuel Perez 
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
#include "../../../mega_cfg_hwd.h"
#include "../../../mega_ADC.h"
#include "../../../mega_UART.h"
#include "../../../mega_debug_hwd.h"
#include <avr_time.h>
#include <mcu_UART_iostream.h>


// Microcontroller
// ---------------
namespace myu = mega_;
namespace hwd = mega_::hwd;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;


// Pin connections
// ---------------
constexpr uint8_t ADC_pin = 28;

// Hwd devices
// -----------
using ADC = hwd::ADC_basic;

// Cfg
// ---
using namespace myu::literals;


// Global 
// ------
namespace global{
myu::Potential AREF= 5_V;
}


// FUNCTIONS
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}


void hello()
{
    UART_iostream uart;
    uart << "\n\nADC basic test\n"
	        "--------------\n"
		"* DON'T FORGET to connect AVCC to power.\n"
		"* Connect pin number " << (uint16_t) ADC_pin 
					<< " to potenciometer.\n";
}

void press_key_to_continue()
{
    UART_iostream uart;
    uart << "Press a key to continue\n";

    char c{};
    uart >> c;
}


// Lo que desea el usuario (que nos comunica via el teclado)
// DUDA: User_wish or User_want?
enum class User_wish{
    nothing,	// el usuario no quiere nada
    cancel	// el usuario quiere cancelar la operacion
};

// Damos la opción al usuario ha cancelar el wait
User_wish wait_ms(uint16_t time_ms)
{
    UART_iostream uart;

    for (uint8_t i = 1; i < time_ms / 100u; ++i){
	myu::wait_ms(100);
	if (uart.is_there_something_to_read()){
	    // uart.consume_char(): (???)
	    char ans{}; 
	    uart >> ans;
	    return User_wish::cancel;
	}
    }

    return User_wish::nothing;
}



// Ampliación de ADC
// -----------------
void wait_until_conversion_is_complete()
{while(!ADC::is_the_conversion_complete()) ;}


myu::Potential ADC_in_volts(const myu::Potential& AREF, uint16_t arefs)
{
    myu::Potential res = myu::Millivolt{AREF};
    res *= arefs;
    res /= uint32_t{1024};

    return res;
}


void ADC_clock_frequency_125kHz()
{
    if constexpr (hwd::clock_cpu() == 1'000'000)
	ADC::clock_frequency_divide_by_8();

    else {
	UART_iostream uart;
	uart << "Case not implemented!\n";
    }
}


void choose_aref()
{
    UART_iostream uart;
    uart << "\n\nAREF:\n"
	    "\t1. External\n"
	    "\t2. Internal to AVCC\n"
	    "\t3. Internal to 1.1V\n";

    char opt{};
    uart >> opt;


    switch(opt){
	    break; case '1':
		    uart << "Connect AREF pin to VCC\n";
		    press_key_to_continue();
		    ADC::AREF_external();
		    global::AREF = 5_V; // TODO: preguntarlo al usuario?

	    break; case '2':
		    // TODO: ¿se puede producir un cortocircuito dentro del
		    // avr si no se desconecta? 
		    uart << "\nIMPORTANT: disconnect AREF pin from VCC\n";
		    press_key_to_continue();
		    ADC::AREF_internal_to_AVCC();
		    global::AREF = 5_V; // TODO: preguntarlo al usuario?
					// Si se pregunta las medidas serán
					// mas exactas

	    break; case '3':
		    // TODO: ¿se puede producir un cortocircuito dentro del
		    // avr si no se desconecta? 
		    uart << "\nIMPORTANT: disconnect AREF pin from VCC\n";
		    press_key_to_continue();
		    ADC::AREF_internal_to_1_1V();
		    global::AREF = 1100_mV; 

	    break; default: uart << "What?\n";
    }
}


void choose_adjust()
{
    UART_iostream uart;
    uart << "\nLeft or right adjust? (l/r) ";

    char opt{};
    uart >> opt;

    switch(opt){
	    break; case 'r':
		   case 'R':
		    uart << "Right adjust\n";
		    ADC::right_adjust_result();

	    break; default: 
		    uart << "Left adjust\n";
		    ADC::left_adjust_result();
    }
}

bool is_valid_prescaler(uint16_t prescaler)
{
    switch(prescaler){
	case 2:
	case 4:
	case 8:
	case 16:
	case 32:
	case 64:
	case 128:
	    return true;

	default:
	    return false;
    }
}

void select_prescaler()
{
    UART_iostream uart;
    uart << "\nSelect prescaler: 2, 4, 8, 16, 32, 64 or 128\n";

    uint16_t prescaler{0};
    uart >> prescaler;
    
    if (!is_valid_prescaler(prescaler)){
	uart << "Wrong value " << prescaler << '\n';
	return;
    }

    myu::KiloHertz freq = hwd::clock_cpu() / prescaler;
    
    // datasheet: 28.4.
    // La frecuencia para 10 bits de resolución debe de ir de 50kHz a 200kHz
    if (freq < 50_kHz){
	uart << "ERROR: prescaler too high\n"
	        "Trying to configure ADC with frequency " << freq << "\n"
		"Its too low.\n";
	return;
    }

    if (freq > 200_kHz){
	uart << "You are trying to configure ADC frequency too high, "
	        "to "  << freq << ".\nAre you sure? (y/n) ";
	char ans{};
	uart >> ans;
	if (ans != 'y' and ans != 'Y')
	    return;
    }

    uart << "ADC clock frequency = " << freq << '\n';
    switch(prescaler){
	break; case 2	: ADC::clock_frequency_divide_by_2();
	break; case 4	: ADC::clock_frequency_divide_by_4();
	break; case 8	: ADC::clock_frequency_divide_by_8();
	break; case 16	: ADC::clock_frequency_divide_by_16();
	break; case 32	: ADC::clock_frequency_divide_by_32();
	break; case 64	: ADC::clock_frequency_divide_by_64();
	break; case 128	: ADC::clock_frequency_divide_by_128();
    }
}

void test_single_conversion_mode()
{
    UART_iostream uart;

    uart << "\n\nSingle conversion mode automatic test\n"
	        "-------------------------------------\n"
		"IMPORTANT: disconnect AREF pin from VCC\n";
    press_key_to_continue();

    // Para ver el comportamiento real del ADC es fundamental conocer
    // el valor real de Vcc.
    uart << "Measure VCC in millivolts (write 0 tu use 5V by default): ";
    uint16_t AREF_mV;
    uart >> AREF_mV;
    if (AREF_mV == 0)
	AREF_mV = 5000;

    myu::Millivolt AREF = AREF_mV;


    // Configuración automática: dejar esto asi como ejemplo
    ADC::AREF_internal_to_AVCC();
    ADC_clock_frequency_125kHz();
    ADC::single_conversion_mode();
    ADC::enable(); 
    ADC::select_pin<ADC_pin>();

    while (1) {
	ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = ADC::ADC_in_arefs();

	uart << "arefs = " << arefs  << "; V = " 
	    << ADC_in_volts(AREF, arefs) << '\n';

	if (wait_ms(1000) == User_wish::cancel)
	    return;

    }                                      
}


// TODO: elaborar más este test y probarlo
void manual_test() 
{
    UART_iostream uart;

    while (1) {
        uart << "\n\nManual test\n"
		"--------\n"
		"r. return main menu\n"    
		"p. print registers\n"
		"0. Disable\n"
		"1. Enable\n"
		"2. Choose AREF\n"
		"3. Left/right adjust\n"
		"4. Select prescaler\n";
//		"4. Select pin\n"

        char res{};
        uart >> res;

        switch (res) {
	    break; case 'r': case 'R': return;
	    break; case 'p': case 'P': 
			    uart << '\n';
			    hwd::print_registers_adc(uart);

	    break; case '0': ADC::disable();
	    break; case '1': ADC::enable();
	    break; case '2': choose_aref();
	    break; case '3': choose_adjust();
	    break; case '4': select_prescaler();
//	    break; case '5': select_pin();

	    break; default: uart << "What?\n";
        }
    }
}
int main() 
{
    init_uart();
    hello();

    UART_iostream uart;

    while (1) {
        uart << "\n\nMenu\n"
                    "----\n"
		"1. Manual test\n"
		"2. Automatic test: single conversion mode\n";

        char res{};
        uart >> res;

        switch (res) {
	    break; case '1': manual_test();
	    break; case '2': test_single_conversion_mode();
//	    break; case '3': test_free_running_mode();

	    break; default: uart << "What?\n";
        }
    }
}

