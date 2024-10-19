// Copyright (C) 2019-2023 Manuel Perez 
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

#include "../../../avr_atmega328p_cfg.h"
#include "../../../avr_interrupt.h"
#include "../../../avr_pin.h"
#include <avr_time.h>
#include "../../../avr_UART_iostream.h"


// Micro
// -----
namespace my_mcu = avr_;
//using Micro = my_mcu::Micro;

// Pins
// ----
template <uint8_t n>
using Pin = my_mcu::Pin<n>;

// Hwd Devices
// -----------


// Global variables (all volatile)
//volatile bool int_pin1 = false; 
//pin 2: UART --> no usar volatile bool int_pin2 = false;
//pin 3: UART --> no usar volatile bool int_pin3 = false;
volatile bool int_pin4 = false;
volatile bool int_pin5 = false;
volatile bool int_pin6 = false;
// pin 7 = VCC
// pin 8 = GND
volatile bool int_pin9 = false;
volatile bool int_pin10 = false;
volatile bool int_pin11 = false;
volatile bool int_pin12 = false;
volatile bool int_pin13 = false;
volatile bool int_pin14 = false;
volatile bool int_pin15 = false;
volatile bool int_pin16 = false;
volatile bool int_pin17 = false;
volatile bool int_pin18 = false;
volatile bool int_pin19 = false;
// pin 20 = AVCC
// pin 21 = AREF
// pin 22 = GND
volatile bool int_pin23 = false;
volatile bool int_pin24 = false;
volatile bool int_pin25 = false;
volatile bool int_pin26 = false;
volatile bool int_pin27 = false;
volatile bool int_pin28 = false;

// Como conectamos al pull-up resistor los pines
// el valor inicial será de 1
volatile bool old_value_pin4 = true;
volatile bool old_value_pin5 = true;
volatile bool old_value_pin6 = true;
volatile bool old_value_pin9 = true;
volatile bool old_value_pin10 = true;
volatile bool old_value_pin11 = true;
volatile bool old_value_pin12 = true;
volatile bool old_value_pin13 = true;
volatile bool old_value_pin14 = true;
volatile bool old_value_pin15 = true;
volatile bool old_value_pin16 = true;
volatile bool old_value_pin17 = true;
volatile bool old_value_pin18 = true;
volatile bool old_value_pin19 = true;
volatile bool old_value_pin23 = true;
volatile bool old_value_pin24 = true;
volatile bool old_value_pin25 = true;
volatile bool old_value_pin26 = true;
volatile bool old_value_pin27 = true;
volatile bool old_value_pin28 = true;


/***************************************************************************
 *				FUNCTIONS
 ***************************************************************************/
void enable_all_change_level_interrupts()
{
    Pin<4>::enable_change_level_interrupt();
    Pin<5>::enable_change_level_interrupt();
    Pin<6>::enable_change_level_interrupt();

    Pin<9>::enable_change_level_interrupt();
    Pin<10>::enable_change_level_interrupt();
    Pin<11>::enable_change_level_interrupt();
    Pin<12>::enable_change_level_interrupt();
    Pin<13>::enable_change_level_interrupt();
    Pin<14>::enable_change_level_interrupt();
    Pin<15>::enable_change_level_interrupt();
    Pin<16>::enable_change_level_interrupt();
    Pin<17>::enable_change_level_interrupt();
    Pin<18>::enable_change_level_interrupt();
    Pin<19>::enable_change_level_interrupt();

    Pin<23>::enable_change_level_interrupt();
    Pin<24>::enable_change_level_interrupt();
    Pin<25>::enable_change_level_interrupt();
    Pin<26>::enable_change_level_interrupt();
    Pin<27>::enable_change_level_interrupt();
    Pin<28>::enable_change_level_interrupt();

}

void disable_all_change_level_interrupts()
{
    Pin<4>::disable_change_level_interrupt();
    Pin<5>::disable_change_level_interrupt();
    Pin<6>::disable_change_level_interrupt();

    Pin<9>::disable_change_level_interrupt();
    Pin<10>::disable_change_level_interrupt();
    Pin<11>::disable_change_level_interrupt();
    Pin<12>::disable_change_level_interrupt();
    Pin<13>::disable_change_level_interrupt();
    Pin<14>::disable_change_level_interrupt();
    Pin<15>::disable_change_level_interrupt();
    Pin<16>::disable_change_level_interrupt();
    Pin<17>::disable_change_level_interrupt();
    Pin<18>::disable_change_level_interrupt();
    Pin<19>::disable_change_level_interrupt();

    Pin<23>::disable_change_level_interrupt();
    Pin<24>::disable_change_level_interrupt();
    Pin<25>::disable_change_level_interrupt();
    Pin<26>::disable_change_level_interrupt();
    Pin<27>::disable_change_level_interrupt();
    Pin<28>::disable_change_level_interrupt();

}


void enable_pcint(uint16_t pin)
{
    if (pin == 4)
	Pin<4>::enable_change_level_interrupt();
    else if (pin == 5)
	Pin<5>::enable_change_level_interrupt();
    else if (pin == 6)
	Pin<6>::enable_change_level_interrupt();
    else if (pin == 9)
	Pin<9>::enable_change_level_interrupt();
    else if (pin == 10)
	Pin<10>::enable_change_level_interrupt();
    else if (pin == 11)
	Pin<11>::enable_change_level_interrupt();
    else if (pin == 12)
	Pin<12>::enable_change_level_interrupt();
    else if (pin == 13)
	Pin<13>::enable_change_level_interrupt();
    else if (pin == 14)
	Pin<14>::enable_change_level_interrupt();
    else if (pin == 15)
	Pin<15>::enable_change_level_interrupt();
    else if (pin == 16)
	Pin<16>::enable_change_level_interrupt();
    else if (pin == 17)
	Pin<17>::enable_change_level_interrupt();
    else if (pin == 18)
	Pin<18>::enable_change_level_interrupt();
    else if (pin == 19)
	Pin<19>::enable_change_level_interrupt();
    else if (pin == 23)
	Pin<23>::enable_change_level_interrupt();
    else if (pin == 24)
	Pin<24>::enable_change_level_interrupt();
    else if (pin == 25)
	Pin<25>::enable_change_level_interrupt();
    else if (pin == 26)
	Pin<26>::enable_change_level_interrupt();
    else if (pin == 27)
	Pin<27>::enable_change_level_interrupt();
    else if (pin == 28)
	Pin<28>::enable_change_level_interrupt();
}

void disable_pcint(uint16_t pin)
{
    if (pin == 4)
	Pin<4>::disable_change_level_interrupt();
    else if (pin == 5)
	Pin<5>::disable_change_level_interrupt();
    else if (pin == 6)
	Pin<6>::disable_change_level_interrupt();
    else if (pin == 9)
	Pin<9>::disable_change_level_interrupt();
    else if (pin == 10)
	Pin<10>::disable_change_level_interrupt();
    else if (pin == 11)
	Pin<11>::disable_change_level_interrupt();
    else if (pin == 12)
	Pin<12>::disable_change_level_interrupt();
    else if (pin == 13)
	Pin<13>::disable_change_level_interrupt();
    else if (pin == 14)
	Pin<14>::disable_change_level_interrupt();
    else if (pin == 15)
	Pin<15>::disable_change_level_interrupt();
    else if (pin == 16)
	Pin<16>::disable_change_level_interrupt();
    else if (pin == 17)
	Pin<17>::disable_change_level_interrupt();
    else if (pin == 18)
	Pin<18>::disable_change_level_interrupt();
    else if (pin == 19)
	Pin<19>::disable_change_level_interrupt();
    else if (pin == 23)
	Pin<23>::disable_change_level_interrupt();
    else if (pin == 24)
	Pin<24>::disable_change_level_interrupt();
    else if (pin == 25)
	Pin<25>::disable_change_level_interrupt();
    else if (pin == 26)
	Pin<26>::disable_change_level_interrupt();
    else if (pin == 27)
	Pin<27>::disable_change_level_interrupt();
    else if (pin == 28)
	Pin<28>::disable_change_level_interrupt();
}

void pins_as_input_with_pullup()
{
    Pin<4>::as_input_with_pullup();
    Pin<5>::as_input_with_pullup();
    Pin<6>::as_input_with_pullup();

    Pin<9>::as_input_with_pullup();
    Pin<10>::as_input_with_pullup();
    Pin<11>::as_input_with_pullup();
    Pin<12>::as_input_with_pullup();
    Pin<13>::as_input_with_pullup();
    Pin<14>::as_input_with_pullup();
    Pin<15>::as_input_with_pullup();
    Pin<16>::as_input_with_pullup();
    Pin<17>::as_input_with_pullup();
    Pin<18>::as_input_with_pullup();
    Pin<19>::as_input_with_pullup();

    Pin<23>::as_input_with_pullup();
    Pin<24>::as_input_with_pullup();
    Pin<25>::as_input_with_pullup();
    Pin<26>::as_input_with_pullup();
    Pin<27>::as_input_with_pullup();
    Pin<28>::as_input_with_pullup();

}


 
void init_uart()
{
    my_mcu::UART_iostream uart;
    basic_cfg(uart);
    uart.turn_on();
}

void init()
{
    init_uart();

    pins_as_input_with_pullup();
    my_mcu::enable_interrupts();
}


// Devuelve true si queremos acabar
bool menu_pcint()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nMenu\n"
	        "----\n"
	    "1. Enable interrupt\n"
	    "2. Disable interrupt\n"
	    "[r]eturn\n";

    char opt{};
    uart >> opt;
    if (opt == 'r' or opt == 'R')
	return true;

    if (opt != '1' and opt != '2'){
	uart << "Unknown option\n";
	return false;
    }

    uart << "Write pin to ";

    if (opt == '1') uart << "enable";
    else            uart << "disable";

    uart << " interrupt: ";
    uint16_t pin{};

    uart >> pin;
    uart << '\n';

    if (opt == '1'){
	enable_pcint(pin);
	uart << "Enable";
    }
    else{
	disable_pcint(pin);
	uart << "Disable";
    }
    
    uart << " PCINT " << pin << '\n';

    return false;
}


void check_pin(volatile bool& int_pin, uint8_t npin)
{
    my_mcu::UART_iostream uart;

    if (int_pin){
	uart << "pin " << (int) npin << " interrupt\n";
	int_pin = false;
    }
}

void check_pcints()
{
    check_pin(int_pin4, 4);
    check_pin(int_pin5, 5);
    check_pin(int_pin6, 6);
    check_pin(int_pin9, 9);
    check_pin(int_pin10, 10);
    check_pin(int_pin11, 11);
    check_pin(int_pin12, 12);
    check_pin(int_pin13, 13);
    check_pin(int_pin14, 14);
    check_pin(int_pin15, 15);
    check_pin(int_pin16, 16);
    check_pin(int_pin17, 17);
    check_pin(int_pin18, 18);
    check_pin(int_pin19, 19);
    check_pin(int_pin23, 23);
    check_pin(int_pin24, 24);
    check_pin(int_pin25, 25);
    check_pin(int_pin26, 26);
    check_pin(int_pin27, 27);
    check_pin(int_pin28, 28);
}


void test_pcint()
{
    my_mcu::UART_iostream uart;
    uart << "\nTest PCINT\n"
	      "----------\n"
	     "Connect a push button to a pin and test the interruption\n"
	     "All pins with PCINT enable\n"
	     "Press a key to enter menu\n";

    enable_all_change_level_interrupts();

    while(1){
	check_pcints();

	if (my_mcu::UART_basic::are_there_data_unread()){
	    char tmp{};
	    uart >> tmp; // borramos el caracter usado para entrar aqui
	    if (menu_pcint())
		break;
	}
    }

    disable_all_change_level_interrupts();
}

template <uint8_t nINT, uint8_t npin>
void test_int()
{
    my_mcu::UART_iostream uart;
    while(1){
	uart << "\nTest INT" << nINT << " (in pin " << (int) npin << ")\n"
		  "---------------\n"
		  "1. Enable chage level interrupt (from 0 to 1, or 1 to 0)\n"
		  "2. Enable when falling edge interrupt (from 1 to 0)\n"
		  "3. Enable when rising edge interrupt (from 0 to 1)\n"
		  "4. Enable when is zero interrupt\n"
		  "5. Disable INTx interrupt\n"
		  "r. Return\n";

	char opt{};
	uart >> opt;
	switch(opt){
	    break; case '1': Pin<npin>::INT::enable_change_level();
	    break; case '2': Pin<npin>::INT::enable_when_falling_edge();
	    break; case '3': Pin<npin>::INT::enable_when_rising_edge();
	    break; case '4': Pin<npin>::INT::enable_when_is_zero();
	    break; case '5': Pin<npin>::INT::disable();
	    break; case 'r':
		   case 'R': 
			    Pin<npin>::INT::disable();
			    return;

	    break; default : uart << "Unknown option\n";
	}

    }
}


void test_int0()
{
    test_int<0, 4>();
}

void test_int1()
{
    test_int<1, 5>();
}

void main_menu()
{
    my_mcu::UART_iostream uart;
    uart << "\n\nMenu\n"
		"----\n"
	    "0. Test INT0\n"
	    "1. Test INT1\n"
	    "2. Test PCINT\n";

    char option{};
    uart >> option;

    switch(option){
	break; case '0': test_int0();
	break; case '1': test_int1();
	break; case '2': test_pcint();
	break; default : uart << "Unknown option\n";
    }
}

int main()
{
    init();

    my_mcu::UART_iostream uart;
    uart << "\n\nExternal interrupts test\n"
	        "------------------------\n"
		"All external interrupts enables by default\n";
    


    while(1){
	main_menu();
    }
}


template <uint8_t n>
void update_pin_value(volatile bool& int_pin, volatile bool& old_value)
{
    if (static_cast<bool>(Pin<n>::read())  != old_value){
	old_value= !old_value;
	int_pin = true;
    }
}

ISR(PCINT0_vect){// pines 9, 10 y del 14 al 19
    update_pin_value<9>(int_pin9, old_value_pin9);
    update_pin_value<10>(int_pin10, old_value_pin10);
    update_pin_value<14>(int_pin14, old_value_pin14);
    update_pin_value<15>(int_pin15, old_value_pin15);
    update_pin_value<16>(int_pin16, old_value_pin16);
    update_pin_value<17>(int_pin17, old_value_pin17);
    update_pin_value<18>(int_pin18, old_value_pin18);
    update_pin_value<19>(int_pin19, old_value_pin19);
}


ISR(PCINT1_vect){// pines del 23 al 28
    update_pin_value<23>(int_pin23, old_value_pin23);
    update_pin_value<24>(int_pin24, old_value_pin24);
    update_pin_value<25>(int_pin25, old_value_pin25);
    update_pin_value<26>(int_pin26, old_value_pin26);
    update_pin_value<27>(int_pin27, old_value_pin27);
    update_pin_value<28>(int_pin28, old_value_pin28);
}

ISR(PCINT2_vect){// pines del 2 al 6, y del 11 al 13
    update_pin_value<4>(int_pin4, old_value_pin4);
    update_pin_value<5>(int_pin5, old_value_pin5);
    update_pin_value<6>(int_pin6, old_value_pin6);
    update_pin_value<11>(int_pin11, old_value_pin11);
    update_pin_value<12>(int_pin12, old_value_pin12);
    update_pin_value<13>(int_pin13, old_value_pin13);
}

ISR_INT0{
    my_mcu::UART_iostream uart;
    uart << "INT0 interrupt\n";

}


ISR_INT1{
    my_mcu::UART_iostream uart;
    uart << "INT1 interrupt\n";

}

