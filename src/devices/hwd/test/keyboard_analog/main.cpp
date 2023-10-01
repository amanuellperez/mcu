// Copyright (C) 2021 Manuel Perez 
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

#include "../../dev_keyboard_analog.h"

#include <avr_ADC_basic.h>
#include <avr_UART.h>
#include <avr_time.h>

constexpr uint8_t npin = 28;
constexpr uint16_t aref0 = 382;
constexpr uint16_t nrow = 82;
constexpr uint16_t ncol = 10;

using Keyboard_npin = dev::Keyboard_ADC0_npin<npin>;
using Keyboard_voltages = dev::Keyboard_ADC0_voltages<aref0, nrow, ncol>;
using  Keyboard = dev::Keyboard_ADC0<Keyboard_npin, Keyboard_voltages>;

constexpr uint16_t AREF_en_mV = 5000;

// TODO: a ADC
void wait_until_conversion_is_complete()
{while(!avr::ADC::is_the_conversion_complete()) ;}



void print(std::ostream& out, const avr::Potential& v)
{
    using namespace avr::literals;

    if (v < 1_V){
	out << atd::to_integer<uint16_t>(v.internal_value()) << ' ';
	atd::print_unit(out, v);
    }
    else
	out << v;
}


template <uint8_t npin>
void test_basic()
{
    avr::UART_iostream uart;

    uart << "\nBasic test\n"
	    "----------\n"
	    "Pulsa teclas del analog keyboard\n";

// ADC_init();
    avr::ADC::AREF_internal_to_AVCC();
    avr::ADC::clock_speed_in_kHz<125>();
    avr::ADC::enable();


    Keyboard keyboard;
//    keyboard.init();

    while (1) {
	if (keyboard.scan()){
            uart << "press (" << (int) keyboard.row() << ", " << (int) keyboard.col()
                 << ")\n";
        }

	wait_ms(100);
    }                                      
}


template <uint8_t npin>
void test_debug()
{
    avr::UART_iostream uart;

    uart << "\nDebug mode\n"
	    "----------\n";

    // encendemos el ADC en internal a 125kHz
    avr::ADC::AREF_internal_to_AVCC();
    avr::ADC::clock_speed_in_kHz<125>();
    avr::ADC::enable();


// seleccionamos pin a leer
    avr::ADC::select_pin<npin>();

    uint16_t old_arefs = 0;


    while (1) {
	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = avr::ADC::ADC_in_arefs();
	if (old_arefs == arefs)
	    continue;


	uart << "arefs = " << arefs  << "; incr = ";
	if (arefs > old_arefs) 
	    uart << (arefs - old_arefs);
	else
	    uart << '-' << (old_arefs - arefs);
	old_arefs = arefs;

	uart << "; V = ";
	avr::Potential V = avr::ADC::ADC_in_volts<AREF_en_mV>();
	print(uart, V);
	uart << "; ";



	dev::__Bisequence s{aref0, ncol, nrow};

	if (arefs < aref0 - ncol / 2)
	    uart << "no press\n";

	else{
	    auto [row, col] = s.index(arefs);
	    uart << "\n\t" << arefs << " = (" << row << ", " << col << ")\n";
	}


	wait_ms(1000);
    }                                      
}


template <uint8_t npin>
void calibrate()
{
    avr::UART_iostream uart;

    uart << "\nCalibration mode\n"
	      "----------------\n";

    // encendemos el ADC en internal a 125kHz
    avr::ADC::AREF_internal_to_AVCC();
    avr::ADC::clock_speed_in_kHz<125>();
    avr::ADC::enable();


// seleccionamos pin a leer
    avr::ADC::select_pin<npin>();

    uart << "No pulses ninguna tecla del analog keyboard\n"
	    "Pulsa una tecla del teclado del ordenador para empezar.\n";

    char res{};
    uart >> res;


	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs0 = avr::ADC::ADC_in_arefs();
	uart << "aref0 = " << arefs0 << "\tV0 = ";
	avr::Potential V = avr::ADC::aref2potential<AREF_en_mV>(arefs0);
	print(uart, V);

	uart << "\nPulsa la tecla (0,0). Pulsa tecla ordenador.\n";
	uart >> res;
	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	auto arefs = avr::ADC::ADC_in_arefs();
	uart << "aref = " << arefs << "\tV0 = ";
	V = avr::ADC::aref2potential<AREF_en_mV>(arefs);
	print(uart, V);
	uart << "; ncol = " << arefs - arefs0;
	arefs0 = arefs;


	uart << "\nPulsa la tecla (0,7). Pulsa tecla ordenador.\n";
	uart >> res;
	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	arefs = avr::ADC::ADC_in_arefs();
	uart << "aref = " << arefs << "\tV0 = ";
	V = avr::ADC::aref2potential<AREF_en_mV>(arefs);
	print(uart, V);
        uart << "; <ncol> = " << (arefs - arefs0) / 7;

	uart << "\nPulsa la tecla (1,0). Pulsa tecla ordenador.\n";
	uart >> res;
	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	arefs = avr::ADC::ADC_in_arefs();
	uart << "aref = " << arefs << "\tV0 = ";
	V = avr::ADC::aref2potential<AREF_en_mV>(arefs);
	print(uart, V);
        uart << "; nrow = " << (arefs - arefs0);



	uart << "\nPulsa la tecla (7,0). Pulsa tecla ordenador.\n";
	uart >> res;
	avr::ADC::start_conversion();
	wait_until_conversion_is_complete();
	
	arefs = avr::ADC::ADC_in_arefs();
	uart << "aref = " << arefs << "\tV0 = ";
	V = avr::ADC::aref2potential<AREF_en_mV>(arefs);
	print(uart, V);
        uart << "; <nrow> = " << (arefs - arefs0) / 7;

        uart << "\nPulsa una tecla del ordenador para volver a empezar\n";
	uart >> res;
}


int main() 
{
// init_UART();
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

// main.run();
    while (1) {
        uart << "\n\nKeyboard_ADC\n"
                    "------------\n"
		"Recordar conectar AVCC a alimentación.\n"
		"La alimentación del ADC es independiente de la del micro.\n"
		"Conectamos internamente el ADC a AVCC\n"
		"Conectar el pin " << (int) npin << " al keyboard.\n"
		"\nMenu:\n"
		"\t0. Debug\n"
		"\t1. Basic test\n"
		"\t2. Calibration\n"
		"\tPulsa una tecla para empezar\n";

        char res{};
        uart >> res;

	switch (res){
	    case '0':
		test_debug<npin>();
		break;

	    case '1':
		test_basic<npin>();
		break;

	    case '2':
		calibrate<npin>();
		break;

	    default:
		test_debug<npin>();
		break;
	}
    }
}

