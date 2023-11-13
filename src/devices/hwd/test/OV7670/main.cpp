// Copyright (C) 2023 Manuel Perez 
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


#include <atd_ostream.h>

#include <avr_atmega.h>
#include <dev_TWI_master.h>
#include <dev_TWI_master_ioxtream.h>

// Micro
// -----
namespace mcu = atmega; 
using Micro   = mcu::Micro;

// Pin connections
// ---------------
// using UART: pins 2 and 3
// available: 4
constexpr uint8_t d7_pin = 5;
constexpr uint8_t d6_pin = 6;

// VCC and GND: 7, 8

constexpr uint8_t d5_pin = 9;
constexpr uint8_t d4_pin = 10;
constexpr uint8_t d3_pin = 11;
constexpr uint8_t d2_pin = 12;
constexpr uint8_t d1_pin = 13;
constexpr uint8_t d0_pin = 15;

constexpr uint8_t xclk_pin  = 14;   // = CLKO
constexpr uint8_t pclk_pin  = 23;
constexpr uint8_t href_pin  = 24;
constexpr uint8_t vsync_pin = 25;

// Not using SPI: available pins 16, 17, 18, 19
// CUIDADO: al principio conecté la CAM a los pins de SPI para aprovecharlos
// pero luego daba errores el programador (posiblemente conflicto de que la
// CAM interaccionaba con el SPI (???))

// Alimentación y AREF: 20, 21, 22

// using TWI: available pins 27 and 28 (SIOC/SIOD)

// Hwd Devices
// -----------

// TWI
// ---
constexpr uint8_t TWI_buffer_size = 100; // TODO: determinar cantidad

using TWI_master_cfg = dev::TWI_master_cfg<Micro, 
                                           mcu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master = dev::TWI_master<TWI_master_cfg>;
using TWI        = dev::TWI_master_ioxtream<TWI_master>;


constexpr uint8_t TWI_frequency = 100; // 100 kHz

// pag. 11 indica el address pero no dice si es decimal o hexadecimal (???)
constexpr uint8_t cam_address = 0x21;


// Como el slave_address es único, solo es posible tener una cámara de este
// estilo conectada. Por eso todas las funciones son static.
class OV7670{
public:
    using Address = typename TWI_master::Address;
    static constexpr Address slave_address = 0x21; // es única
	
    // Miramos a ver si hay comunicación con el twi-device
    // Return: true (found device); false (no response)
    static bool probe();

// Commands
    static bool reset();

// Funciones de bajo nivel para acceder a los registros
    // register[i] = x;
    static bool write_register(uint8_t i, uint8_t x);

    // x = register[i];
    static uint8_t read_register(uint8_t i);
private:
};


bool OV7670::write_register(uint8_t i, uint8_t x)
{
    TWI twi{slave_address};

    twi << i;
    if (twi.error()) return false;

    twi << x;
    if (twi.error()) return false;

    twi.close();

    Micro::wait_ms(1); // según la datasheet máximo 300 ms en cambiar un registro!!!

    return true;
}


// TODO: crear state y que tenga state wrong si va mal la lectura
uint8_t OV7670::read_register(uint8_t i)
{
    TWI twi{slave_address};
    twi << i;
    // if (twi.error()) ...
    twi.close();
    Micro::wait_ms(1);  // FUNDAMENTAL!!!

    uint8_t res{};
    twi.open(slave_address);
    twi >> res;
    // if (twi.error()) ...
    twi.close();
    Micro::wait_ms(1); 
    
    return res;
}



inline bool OV7670::probe()
{ return TWI_master::probe(slave_address); }

inline bool OV7670::reset()
{
    if (write_register(0x12, 0x80) == false)
	return false;

    Micro::wait_ms(100);    // segun la datasheet 1 ms máximo
			  
    return true;
    
}


// Functions
// ---------

void init_uart()
{
    mcu::UART_iostream uart;
    mcu::basic_cfg(uart);
    uart.turn_on();
}


void help()
{
    mcu::UART_iostream uart;
    uart << "*** IMPORTANT ***\n"
	    "Remember to run the command `make fuses` "
	    "to change the value of the LFUSE\n"
	    "to generate the XCLK signal in pin " << (int) xclk_pin <<
	    ".\nAnd don't forget to use the programmer and the "
	    "FTDI cable with 3.3V\n\n";
}

void hello()
{
    mcu::UART_iostream uart;
    uart << "\n\nCamera OV7670 test\n"
	        "------------------\n";

    help();
}


void init_TWI()
{
    TWI_master::turn_on<TWI_frequency>();
}

void test_cam_connected()
{
    mcu::UART_iostream uart;

    while(1){
	uart << "Scanning for cam ... ";
	if (OV7670::probe()){
	    uart << "FOUND\n";
	    return;
	}
	else
	    uart << "can't find!!!\n";

	help();

	Micro::wait_ms(1000);
    }
}

void init_cam()
{
    mcu::UART_iostream uart;
    uart << "Reseting cam ... ";

    if (OV7670::reset())
	uart << "OK\n";
    else {
	uart << "Can't reset cam\n";
	// TODO: devolver el state de TWI? un state propio de la cam?
    }
}

inline bool check_twi_state(const char* msg)
{
    mcu::UART_iostream uart;
    if (TWI_master::error()){
	uart << "ERROR(" << msg << "): ";
	TWI_master::print_state(uart);
	return true;
    }

    return false;
}


void read_cam_ram_address(uint8_t addr)
{
    mcu::UART_iostream uart;

    uint8_t b = OV7670::read_register(addr);

    uart << "cam[";
    atd::print_int_as_hex(uart, addr);
    uart << "] = ";
    atd::print_int_as_hex(uart, b);
    uart << '\n';

    Micro::wait_ms(1);  // FUNDAMENTAL!!!



}

void test_read_register()
{
    mcu::UART_iostream uart;
    uart << "\n\nread_register test\n"
	      "------------------\n";

    uart << "register[0x0A] == 0x76? ... ";
    if (OV7670::read_register(0x0A) == 0x76)
	uart << "OK\n";
    else
	uart << "ERROR\n";

    uart << "register[0x0B] == 0x73? ... ";
    if (OV7670::read_register(0x0B) == 0x73)
	uart << "OK\n";
    else
	uart << "ERROR\n";

}

void test_write_register()
{
    mcu::UART_iostream uart;
    uart << "\n\nwrite_register test\n"
	        "-------------------\n";

    uint8_t new_value = 0x10;

    uint8_t old = OV7670::read_register(0x01);
    if (old == new_value)
	new_value += 10;
     
    OV7670::write_register(0x01, new_value);
    uint8_t x1 = OV7670::read_register(0x01);
    uart << "(1) write_register ... ";
    if (x1 == new_value)
	uart << "OK\n";
    else
	uart << "FAIL\n";

    OV7670::write_register(0x01, old);
    x1 = OV7670::read_register(0x01);

    uart << "(2) write_register ... ";
    if (x1 == old)
	uart << "OK\n";
    else
	uart << "FAIL\n";


}


int main()
{
    init_uart();
    init_TWI();

    hello();

    test_cam_connected();
    init_cam();

    while(1){
	mcu::UART_iostream uart;
	uart << "\n\n-------------------\n";

	test_read_register();
	test_write_register();

	Micro::wait_ms(2000);
    }
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}

