// Copyright (C) 2019-2021 Manuel Perez 
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

// Conectar dos potenciometros. De esta forma probamos SPI también.
#include "../../dev_pot_MCP4231.h"
#include <avr_UART.h>
#include <avr_time.h>
#include <atd_cast.h>


// ¿Cómo seleccionamos el dispositivo SPI?
class Select{
public:
    using no_CS = avr::Pin<16>; // es el pin SS
		// no es necesario definirlo de salida, ya que lo hace SPI

    explicit Select() { no_CS::write_zero(); }

    ~Select() { no_CS::write_one(); }

    Select(const Select&) = delete;
    Select& operator= (const Select&) = delete;
};


using SPI = avr::SPI_master;
using Pot = dev::Pot_MCP4231<Select>;


// Funciona hasta con 10 MHz
constexpr uint8_t period_in_us = 2;	
//constexpr uint8_t period_in_us = 16;

void status_register_menu()
{
    avr::UART_iostream uart;

    uart << "\nStatus register = " <<
	std::to_integer<uint16_t>(Pot::status_register()) << "\n\n";

}

void bool2yesno(bool x)
{
    avr::UART_iostream uart;
    if (x)
	uart << "YES";
    else 
	uart << "NO";
}

void tcon_read()
{
    avr::UART_iostream uart;
    std::byte tcon = Pot::TCON_register();

    uart << "TCON = " << std::to_integer<uint16_t>(tcon) << "\n";

// resistor 0
    uart << "Resistor0:\n"
	    "   shutdown = ";
    bool2yesno(Pot::TCON::resistor0_shutdown(tcon));

    uart << "\n   terminal A CONNECT = ";
    bool2yesno(Pot::TCON::resistor0_terminalA_connect(tcon));

    uart << "\n   terminal W CONNECT = ";
    bool2yesno(Pot::TCON::resistor0_terminalW_connect(tcon));

    uart << "\n   terminal B CONNECT = ";
    bool2yesno(Pot::TCON::resistor0_terminalB_connect(tcon));


// resistor 1
    uart << "\n\nResistor1:\n"
	    "   shutdown = ";
    bool2yesno(Pot::TCON::resistor1_shutdown(tcon));

    uart << "\n   terminal A = ";
    bool2yesno(Pot::TCON::resistor1_terminalA_connect(tcon));

    uart << "\n   terminal W = ";
    bool2yesno(Pot::TCON::resistor1_terminalW_connect(tcon));

    uart << "\n   terminal B = ";
    bool2yesno(Pot::TCON::resistor1_terminalB_connect(tcon));

    uart << "\n\n";


}

void tcon_write()
{
    avr::UART_iostream uart;
    uart << "\n\nTCON write:\n"
	    "1. New TCON\n"
	    "2. Options\n";

    char c{};
    uart >> c;
    if (c == '1'){
	uint16_t tcon{};
	uart << "New TCON = ";
	uart >> tcon;
	Pot::TCON_register(std::byte{static_cast<uint8_t>(tcon)});
	return;
    }

// options
    std::byte tcon{0};
    uart << "\nResistor 0 shutdown: y/n?";
    uart >> c;
    Pot::TCON::resistor0_shutdown(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 0 terminal A connect: y/n?";
    uart >> c;
    Pot::TCON::resistor0_terminalA_connect(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 0 terminal W connect: y/n?";
    uart >> c;
    Pot::TCON::resistor0_terminalW_connect(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 0 terminal B connect: y/n?";
    uart >> c;
    Pot::TCON::resistor0_terminalB_connect(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 1 shutdown: y/n?";
    uart >> c;
    Pot::TCON::resistor1_shutdown(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 1 terminal A connect: y/n?";
    uart >> c;
    Pot::TCON::resistor1_terminalA_connect(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 1 terminal W connect: y/n?";
    uart >> c;
    Pot::TCON::resistor1_terminalW_connect(tcon, c == 'y' or c == 'Y');

    uart << "\nResistor 1 terminal B connect: y/n?";
    uart >> c;
    Pot::TCON::resistor1_terminalB_connect(tcon, c == 'y' or c == 'Y');

    Pot::TCON_register(tcon);
    
}

void tcon_register_menu()
{
    avr::UART_iostream uart;
    uart << "\nTCON register:\n"
	      "1. read\n"
	      "2. write\n";
    char c{};
    uart >> c;
    switch(c){
	case '1':
	    tcon_read();
            break;

	case '2':
	    tcon_write();
	    break;
    }

}

template <uint8_t npot>
void wiper_register_menu()
{
    avr::UART_iostream uart;
    uart << "Write wiper" << (uint16_t) npot << " value: ";
    uint16_t x{};
    uart >> x;
    uart << x << '\n';

    uart << "Writing value ... ";
    Pot::wiper_write_data<npot>(x);
    

    uint16_t res = Pot::wiper_read_data<npot>();
    if (res != x)
	uart << "ERROR: wiper value = " << res << '\n';
    else
	uart << "OK\n";
}



void test_basic()
{
    avr::UART_iostream uart;
    uart << "\n\nBasic test:\n";

    while (1) {
	uart << "\n\nMenu:\n"
		"1. Status register\n"
		"2. TCON register\n"
		"3. Wiper 0 register\n"
		"4. Wiper 1 register\n";
		

	char c{};
	uart >> c;

	switch(c){
	    case '1':
		status_register_menu();
                break;
            case '2':
                tcon_register_menu();
                break;
            case '3':
                wiper_register_menu<0>();
                break;
            case '4':
                wiper_register_menu<1>();
                break;

	    default:
		return;
	}

    }
}



int main() 
{
// init_uart()
    avr::UART_iostream uart;
    avr::basic_cfg(uart);
    uart.turn_on();

// init_SPI()
    SPI::turn_on<period_in_us>();

    uart << "\n\nPotentiometer MCP4231\n"
	        "---------------------\n";
    while(1){
	test_basic();
    }// while
}



