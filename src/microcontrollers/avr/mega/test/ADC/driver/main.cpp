// Copyright (C) 2024 Manuel Perez 
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
#include "../../../mega_cfg_hwd.h"
#include "../../../mega_ADC.h"
#include "../../../mega_UART.h"
#include <mcu_UART_iostream.h>
#include <avr_time.h>


// Microcontroller
// ---------------
namespace myu = mega_;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;
		
// Pin connections
// ---------------
constexpr uint8_t ADC_npin = 28;

// Hwd devices
// -----------
using ADC = myu::ADC;
using ADC_pin = myu::ADC_pin_single_mode<ADC_npin, 5'000>;

// Cfg
// ---
using namespace myu::literals;


myu::Potential ADC_in_volts(const myu::Potential& AREF, uint16_t arefs)
{
    myu::Millivolt res{AREF};
    res *= arefs;
    res /= uint32_t{1024};

    return res;
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
    uart << "\n\nADC driver test\n"
	        "---------------\n"
		"* DON'T FORGET to connect AVCC to power.\n"
		"* Connect pin number " << (uint16_t) ADC_npin 
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


    ADC::single_mode<ADC::AREF_connect_to_internal_AVCC, 125>();
    ADC::select_pin<ADC_npin>();

    while (1) {
	auto arefs = ADC::read_in_arefs();

	uart << "ADC: arefs = " << arefs  << ";          V = " 
	    << ADC_in_volts(AREF, arefs)
	    << "; (si AREF = 5V ==> " << ADC::aref2potential<5000>(arefs)
	    << ")\n"
	    << "pin: arefs = " << ADC_pin::read_percentage()
	    << " = " << ADC_pin::read_potential()
	    << "\n\n";

	if (wait_ms(1000) == User_wish::cancel)
	    return;

    }                                      
}


int main() 
{
    init_uart();
    hello();

    while (1) {
	test_single_conversion_mode();
    }
}

