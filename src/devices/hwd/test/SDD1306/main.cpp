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

#include "../../dev_SDD1306.h"
#include <avr_atmega.h>
#include <mcu_TWI_master.h>
#include <mcu_TWI_master_ioxtream.h>

// Fonts
#include <rom_font_alagard_13x15_cr.h>
#include <rom_font_DePixelBreit_12x10_cr.h>
#include <rom_font_DePixelBreitFett_23x13_cr.h>
#include <rom_font_DePixelHalbfett_11x13_cr.h>
#include <rom_font_DePixelIllegible_8x8_cr.h>
#include <rom_font_DePixelKlein_10x10_cr.h>
#include <rom_font_DePixelSchmal_10x11_cr.h>
#include <rom_font_dogica_8x8_cr.h>
#include <rom_font_dogicabold_8x8_cr.h>
#include <rom_font_dogica_number_5x7_cr.h>
#include <rom_font_dogicapixel_7x8_cr.h>
#include <rom_font_dogicapixelbold_8x8_cr.h>
#include <rom_font_mai10_10x14_cr.h>
#include <rom_font_Minecraft_14x16_cr.h>
#include <rom_font_rainyhearts_11x13_cr.h>
#include <rom_font_RetroGaming_11x13_cr.h>
#include <rom_font_upheavtt_13x14_cr.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------


// TWI
// ---
constexpr uint8_t TWI_buffer_size = 10;

using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           myu::TWI_basic,
					   TWI_buffer_size>;

using TWI_master = mcu::TWI_master<TWI_master_cfg>;
using TWI = mcu::TWI_master_ioxtream<TWI_master>;

// CUIDADO: la sabiduría interniana dice que funciona a 100kHz
// pero al hacere las primeras pruebas perdía datos.
// Para ver si se pierden datos:
//  1) pintar toda la pantalla blanca
//  2) borrarla
// Si quedan partes sin borrar es que se están perdiendo datos.
constexpr uint16_t TWI_scl_frequency = 400; // 400 kHz


// Devices
// -------
//using SDD1306 = dev::SDD1306_basic<TWI_master>;

struct SDD1306_cfg {
    using TWI_master = ::TWI_master;
    static constexpr TWI_master::Address twi_address = 0x3C;
};
using SDD1306 = dev::SDD1306_I2C_128x64<SDD1306_cfg>;

struct SDD1306_init_cfg {
};

static constexpr uint8_t control_byte = 0x00;
static constexpr uint8_t data_byte    = 0x40;
using Rect = dev::PageCol_rectangle;



// Functions
// ---------
void init_uart()
{
    myu::UART_iostream uart;
    myu::basic_cfg<baud_rate>(uart);
    uart.turn_on();
}

void init_TWI()
{
    TWI_master::turn_on<TWI_scl_frequency>();
}

void init_sdd1306()
{
    myu::UART_iostream uart;
    uart << "Connecting with SDD1306 ... ";

    if (SDD1306::probe()){
	uart << "OK\n";
	return;
    }

    // else
    uart << "FAIL!!! Can't connect with device\n";
    Micro::wait_ms(1000);

}


void hello()
{
    myu::UART_iostream uart;
    uart << "\n\nSDD1306 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect SDD1306 to TWI"
		<< "\n\n";
}






void blink(uint8_t npage, uint8_t x)
{
    uint8_t x1 = x + 4u;
    dev::PageCol_rectangle r{{npage, x}, {npage, x1}};

    SDD1306::fill(r, 0xFF);
    Micro::wait_ms(200);

    SDD1306::fill(r, 0x00);
    Micro::wait_ms(200);

}



void send_data_waiting(TWI& twi, uint8_t x, bool wait)
{
    twi << x;
    if (!wait)
	return;

    twi.close();
    Micro::wait_ms(400);
    twi.open(SDD1306::address);
    twi << data_byte;
}

void basic_drawing()
{
    myu::UART_iostream uart;

    TWI twi(SDD1306::address);
    twi << control_byte;

    
    twi << uint8_t(0x21) << uint8_t{0} << uint8_t{127}; // column add start at 0 y acaba en 127
    twi << uint8_t{0x22} << uint8_t{0x01} << uint8_t{0x02}; // page address start at 0, end at 7
							    
    if (twi.error())
	uart << "1.FAIL\n";

    twi.close();

    twi.open(SDD1306::address);
    twi << data_byte;

    uint16_t colmax = 10;
    bool wait = false;

    for (int j = 0; j < 2; ++j){
	for (uint16_t i = 0; i < colmax - 1; ++i){
	    send_data_waiting(twi, uint8_t{0x81}, wait);
	}

	twi << uint8_t{0xFF};

	for (uint16_t i = 0; i < colmax - 1; ++i){
	    send_data_waiting(twi, uint8_t{0x18}, wait);
	}
	twi << uint8_t{0xFF};
    }

    twi.close();
}



template <typename Font>
dev::PageCol write_letter(char c, const dev::PageCol& pos)
{
    // TODO: a font.h
    constexpr uint8_t char_byte_size = Font::cols * Font::col_in_bytes;
    uint8_t tmp[char_byte_size];

    auto letter = Font::glyph.row(c - Font::index0);
    std::copy(letter.begin(), letter.end(), &tmp[0]);
    
    uint8_t col1 = pos.col + Font::cols - 1;
    uint8_t page1 = pos.page + Font::col_in_bytes - 1;
    SDD1306::vertical_mode(Rect{pos, {page1, col1}});
    SDD1306::gddram_write(tmp);
    return {pos.page, col1};
}

template <typename Font>
dev::PageCol write(dev::PageCol pos, const char msg[])
{
    for (const char* p = msg; *p != '\0'; ++p){
	pos = write_letter<Font>(*p, pos);
	if (pos.col + 6 >= SDD1306::ncols){
	    pos.col = 0;
	    ++pos.page;
	}
    }
    return pos;
}


template <typename Font>
void write_font(const char font_name[])
{
    dev::PageCol pos{0, 0};
    
    pos = write<Font>(pos, font_name);

//    for (const char* p = font_name; *p != '\0'; ++p){
//	pos = write_letter<Font>(*p, pos);
//	if (pos.col + 6 >= SDD1306::ncols){
//	    pos.col = 0;
//	    ++pos.page;
//	}
//    }
    ++pos.page;
    pos.col = 0;


    for (char c = 32; c <= 125; ++c){
	pos = write_letter<Font>(c, pos);
	if (pos.col + 6 >= SDD1306::ncols){
	    pos.col = 0;
	    ++pos.page;
	}
    }
}


void test_display()
{

//    SDD1306::fill(0xF0);
//    Micro::wait_ms(700);
//
//    SDD1306::clear();

//    for (uint8_t page = 0; page < 8; ++page){
//	SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0xFF);
//	Micro::wait_ms(200);
//	SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0x00);
//    }

//    for (uint8_t i = 0; i < 5; ++i)
//	blink(0, 4*i);

//    basic_drawing();
    
    using Dogica = rom::font_dogica_8x8_cr::Font;

    SDD1306::clear();
    write_letter<Dogica>('0', {0,0});
    write_letter<Dogica>('1', {1,0});
    write_letter<Dogica>('2', {2,0});
    write_letter<Dogica>('3', {3,0});

    write_letter16({2,10});

    SDD1306::clear();

    using Minecraft = rom::font_Minecraft_14x16_cr::Font;
    write<Minecraft>({2,5}, "Minecraft");
    Micro::wait_ms(1000);

    using DePixelBreitFett = rom::font_DePixelBreitFett_23x13_cr::Font;
    write<DePixelBreitFett>({2,5}, "DePixel");
    Micro::wait_ms(1000);

    SDD1306::clear();

    write_font<Dogica>("Dogica");
    Micro::wait_ms(1000);

    using DogicaBold = rom::font_dogicabold_8x8_cr::Font;
    write_font<DogicaBold>("Dogica bold");
    Micro::wait_ms(1000);

    using DogicaPixel = rom::font_dogicapixel_7x8_cr::Font;
    write_font<DogicaPixel>("Dogica pixel");
    Micro::wait_ms(1000);

    using DogicaPixelBold = rom::font_dogicapixelbold_8x8_cr::Font;
    write_font<DogicaPixelBold>("Dogica pixel bold");
    Micro::wait_ms(1000);
//
//    Micro::wait_ms(10'000);
}



// Main
// ----
int main() 
{

    init_uart();
    hello();

    init_TWI();

    SDD1306::init<SDD1306_init_cfg>();

    myu::UART_iostream uart;

    while(1){
	test_display();

	Micro::wait_ms(2000);
    }
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}


