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
#include <atd_istream.h>

#include <mega.h>
#include "../../dev_OV7670_rom.h"
#include <mcu_TWI_master.h>
#include <mcu_TWI_master_ioxtream.h>

#include "../../dev_OV7670.h" // TODO: orden
// Micro
// -----
namespace myu = atmega; 
using Micro   = myu::Micro;

// Pin connections
// ---------------
// using UART: pins 2 and 3
constexpr uint8_t vsync_pin = 4;
constexpr uint8_t href_pin  = 5;

// PORTD_HIGH: 13, 12, 11, 6
constexpr uint8_t d4_pin =  6;

// VCC and GND: 7, 8

// crystal of 16MHz in pins 9-10

constexpr uint8_t d5_pin = 11;
constexpr uint8_t d6_pin = 12;
constexpr uint8_t d7_pin = 13;

constexpr uint8_t xclk_pin  = 14;   // = CLKO
		
constexpr uint8_t pclk_pin  = 15;

// CUIDADO: al principio conecté la CAM a los pins de SPI para aprovecharlos
// pero luego daba errores el programador (posiblemente conflicto de que la
// CAM interaccionaba con el SPI (???))
// avalaible: (Not using SPI:) 16,17, 18, 19

// AVCC, AREF, GND: 20,21,22

// PORTC_LOW: 26, 25, 24, 23
constexpr uint8_t d0_pin =  23;
constexpr uint8_t d1_pin =  24;
constexpr uint8_t d2_pin =  25;
constexpr uint8_t d3_pin =  26;

// using TWI: pins 27 and 28 (SIOC/SIOD)

// Voy a usar como OV7670_D port [PD_high, PC_low]
// PD_high: pins 6, 11, 12, 13
// PC_low : pins 23, 24, 25, 26
struct OV7670_DPort{
    static void as_input_without_pullup()
    {
	atd::write_bits<4,5,6,7>::to<0,0,0,0>::in(DDRD);
	atd::write_bits<4,5,6,7>::to<0,0,0,0>::in(PORTD);

	atd::write_bits<0,1,2,3>::to<0,0,0,0>::in(DDRC);
	atd::write_bits<0,1,2,3>::to<0,0,0,0>::in(PORTC);
    }


//    // Fundamental: esta conversión tiene que ser eficiente
//    operator uint8_t() const 
//    { return value();}

    uint8_t value() const // ??? volatile
    { return (PIND & 0xF0) | (PINC & 0x0F);}
};


// UART
// ----
// El número de bytes por segundo que podemos transmitir es 
//		baud_rate / 8.
//
// Ejemplo: si baud_rate = 500.000 bit/second ==> transmitimos 
//	    62.500 bytes por segundo.
constexpr uint32_t baud_rate = 500'000u;
#warning "Remember to use baud_rate == 500'000"

// TWI
// ---
constexpr uint8_t TWI_buffer_size = 100; // TODO: determinar cantidad

using TWI_master_cfg = dev::TWI_master_cfg<Micro, 
                                           myu::hwd::TWI,
					   TWI_buffer_size>;

using TWI_master = dev::TWI_master<TWI_master_cfg>;
using TWI        = dev::TWI_master_ioxtream<TWI_master>;


constexpr uint8_t TWI_frequency = 100; // 100 kHz

// Hwd Devices
// -----------
using OV7670_pins =
	dev::OV7670_pins<dev::OV7670_VSYNC<vsync_pin>,
			 dev::OV7670_HREF<href_pin>,
			 dev::OV7670_PCLK<pclk_pin>,
			 OV7670_DPort>;

using OV7670_cfg = dev::OV7670_cfg<Micro, TWI, OV7670_pins>;
using OV7670 = dev::OV7670<OV7670_cfg>;





// Functions
// ---------

void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}


void help()
{
    myu::UART_iostream uart;
    uart << "*** IMPORTANT ***\n"
	    "Remember to run the command `make fuses` "
	    "to change the value of the LFUSE\n"
	    "to generate the XCLK signal in pin " << (int) xclk_pin <<
	    ".\nAnd don't forget to use the programmer and the "
	    "FTDI cable with 3.3V\n\n";
}

void hello()
{
    myu::UART_iostream uart;
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
    myu::UART_iostream uart;

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
    myu::UART_iostream uart;
    uart << "Init cam ... ";

    if (OV7670::init())
	uart << "OK\n";
    else {
	uart << "Can't init cam\n";
	// TODO: devolver el state de TWI? un state propio de la cam?
    }
}

inline bool check_twi_state(const char* msg)
{
    myu::UART_iostream uart;
    if (TWI_master::error()){
	uart << "ERROR(" << msg << "): ";
	TWI_master::print_state(uart);
	return true;
    }

    return false;
}


void read_cam_ram_address(uint8_t addr)
{
    myu::UART_iostream uart;

    uint8_t b = OV7670::read_register(addr);

    if (OV7670::last_operation_fail()){
	uart << "Error: read_register fail\n";
	return;
    }

    uart << "cam[";
    atd::print_int_as_hex(uart, addr);
    uart << "] = ";
    atd::print_int_as_hex(uart, b);
    uart << '\n';

    Micro::wait_ms(1);  // FUNDAMENTAL!!!



}

void test_read_register()
{
    myu::UART_iostream uart;
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

    uint8_t hM = (OV7670::read_register(0x1C));
    uint8_t lM = (OV7670::read_register(0x1D));
    uint16_t manufacturer_id = atd::concat_bytes<uint16_t>(hM, lM);
    uart << "Manufacturer ID = ";
    atd::print_int_as_hex(uart, manufacturer_id);
    uart << '\n';



}

void test_write_register()
{
    myu::UART_iostream uart;
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

template <typename It>
void write_registers(It p0, It pe)
{
    myu::UART_iostream uart;

    uart << "Writing registers ... ";
    if (OV7670::write(p0, pe) == true)
	uart << "OK\n";
    else{
	uart << "ERROR\n";
    }
}

void test_interactive_read()
{
    myu::UART_iostream uart;
    uart << "\n\nRead register\n"
	    "-------------\n"
	    "Register address (in hex): ";

    uint8_t address{};
    atd::read_int_as_hex(uart, address);

    uart << "\nRegister[";
    atd::print_int_as_hex(uart, address);
    uart << "] = ";

    uint8_t value = OV7670::read_register(address);
    if (OV7670::last_operation_fail()){
	uart << "ERROR: can't read register\n";
	return;
    }

    atd::print_int_as_hex(uart, value);
    uart << '\n';
    

}

void test_resolution()
{
    myu::UART_iostream uart;
    uart << "\n\nResolution:\n"
	    "1. VGA\n"
	    "2. QVGA\n"
	    "3. QQVGA\n";

    char opt{};
    uart >> opt;

    namespace cfg = dev::OV7670_register_cfg;
    switch(opt){
	break; case '2': write_registers(cfg::qvga.begin(), cfg::qvga.end());
	break; case '3': write_registers(cfg::qqvga.begin(), cfg::qqvga.end());
	break; default : write_registers(cfg::vga.begin(), cfg::vga.end());

    }
}

void test_pclk()
{
    namespace cfg = dev::OV7670_register_cfg;

    myu::UART_iostream uart;
    uart << "\n\nPrescalar of PCLK (see changes in oscilloscope)\n";

    while (1){
	uart << "Write value of CLKRC (0 to return): ";
	uint16_t tmp = 0; // no definirlo como uint8_t ya que lee un caracter!!!
	uart >> tmp ;
	if (tmp == 0)
	    return;
	uint8_t clkcr = static_cast<uint8_t>(tmp);

	atd::print_int_as_hex(uart, clkcr);
	uart << " =(" << (int) clkcr << ") ";
	uart << " ... ";
	OV7670::write_register(cfg::REG::CLKRC, clkcr);
	if (OV7670::last_operation_fail())
	    uart << "ERROR\n";
	else
	    uart << "OK\n";
    }

}

void test_capture_image()
{
    myu::UART_iostream uart;

    namespace cfg = dev::OV7670_register_cfg;
    write_registers(cfg::color_bar_test.begin(), cfg::color_bar_test.end());
    Micro::wait_ms(300); // TODO: en write_registers
//
//    uart << "Press a key to capture image\n";
//    char ans{};
//    uart >> ans;


    while (1){
    // No imprimimos nada para medir velocidad
    auto [nbytes, nlines] = OV7670::capture_image<false>(uart);
    uart << "\nRead " << nbytes << " bytes in " << nlines << "\n";
    Micro::wait_ms(5000);

    // Leemos imagen
    std::tie(nbytes, nlines) = OV7670::capture_image<true>(uart);
    uart << "\nRead " << nbytes << " bytes in " << nlines << "\n";
    Micro::wait_ms(5000);
    }


}

int main()
{
    init_uart();
    init_TWI();

    hello();

    test_cam_connected();
    init_cam();

    while(1){
	myu::UART_iostream uart;
	uart << "\n\n-------------------\n";

	test_read_register();
//	test_write_register();
//
//	test_pclk();
//	test_resolution();
	test_capture_image();

//	test_interactive_read();

	Micro::wait_ms(2000);
    }
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}

