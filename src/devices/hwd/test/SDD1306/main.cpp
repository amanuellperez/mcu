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

// Elegir que fuentes mostrar en pantalla
//constexpr int font_test = 1; 
//constexpr int font_test = 2; 
//constexpr int font_test = 3; 
constexpr int font_test = 3; 

#include "../../dev_SDD1306.h"
#include <mega.h>
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
#include <rom_font_HomeVideo_10x14_cr.h>
#include <rom_font_mai10_10x14_cr.h>
#include <rom_font_Minecraft_14x16_cr.h>
#include <rom_font_PerfectDOSVGA437_8x15_cr.h>
#include <rom_font_PerfectDOSVGA437Win_8x15_cr.h>
#include <rom_font_rainyhearts_11x13_cr.h>
#include <rom_font_RetroGaming_11x13_cr.h>
#include <rom_font_upheavtt_13x14_cr.h>
#include <rom_font_VCR_12x17_cr.h>

#include <atd_test.h>
using namespace test;

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;
using UART_iostream = mcu::UART_iostream<myu::UART_8bits>;

// UART
// ----
constexpr uint32_t baud_rate = 9'600;

// Pin connections
// ---------------


// TWI
// ---
constexpr uint8_t TWI_buffer_size = 10;

using TWI_master_cfg = mcu::TWI_master_cfg<Micro, 
                                           myu::hwd::TWI,
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
using Rect = SDD1306::PageCol_rectangle;



// Functions
// ---------
void init_uart()
{
    UART_iostream uart;
    UART_iostream::init();
    uart.turn_on();
}

void init_TWI()
{
    TWI_master::turn_on<TWI_scl_frequency>();
}

void init_sdd1306()
{
    UART_iostream uart;
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
    UART_iostream uart;
    uart << "\n\nSDD1306 test\n"
	        "------------\n"
		"Connections:\n"
		"\tConnect SDD1306 to TWI"
		<< "\n\n";
}






void blink(uint8_t npage, uint8_t x)
{
    uint8_t x1 = x + 4u;
    Rect r{{npage, x}, {npage, x1}};

    SDD1306::fill(r, 0xFF);
    Micro::wait_ms(200);

    SDD1306::fill(r, 0x00);
    Micro::wait_ms(200);

}



// Escribe todos los códigos ASCII
template <typename Font>
void write_font(const char font_name[])
{
    SDD1306::PageCol pos{0, 0};
    
    pos = SDD1306::print<Font>(pos, font_name);

    ++pos.page;
    pos.col = 0;


    for (char c = 32; c <= 125; ++c){
	pos = SDD1306::print<Font>(pos, c);
	if (pos.col + Font::cols >= SDD1306::ncols){
	    pos.col = 0;
	    pos.page += Font::bytes_in_a_column;
	}
    }
}


void test_basic()
{
    SDD1306::fill(0xF0);
    Micro::wait_ms(700);

    SDD1306::clear();

    for (uint8_t page = 0; page < 8; ++page){
	SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0xFF);
	Micro::wait_ms(200);
	SDD1306::fill(Rect{{page, 60}, {page, 68}}, 0x00);
    }

    for (uint8_t i = 0; i < 5; ++i)
	blink(0, 4*i);
}


void test_font()
{
    // Son demasiadas letras para probar a la vez
    // De hecho al intentar probarlas todas el compilador dio overflow de la
    // zona .text xD
    // Por eso uso flags para compilar unas u otras a elección

    SDD1306::clear();

    if constexpr (font_test == 1){
	using alagard = rom::font_alagard_13x15_cr::Font;
	using DePixelBreit = rom::font_DePixelBreit_12x10_cr::Font;
	using DePixelBreitFett = rom::font_DePixelBreitFett_23x13_cr::Font;
	using DePixelHalbfett = rom::font_DePixelHalbfett_11x13_cr::Font;
	using DePixelIllegible = rom::font_DePixelIllegible_8x8_cr::Font;
	using DePixelKlein = rom::font_DePixelKlein_10x10_cr::Font;
	using DePixelSchmal = rom::font_DePixelSchmal_10x11_cr::Font;

	SDD1306::print<DePixelKlein>({0,0},
		    "Lets see some fonts");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::print<alagard>({2,0}, "alagard");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelBreit>({2,0}, "DePixelBreit");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelBreitFett>({0,0}, "DePixelBreitFett");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelHalbfett>({2,0}, "DePixelHalbfett");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelIllegible>({2,0}, "DePixelIllegible");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelKlein>({2,0}, "DePixelKlein");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<DePixelSchmal>({2,0}, "DePixelSchmal");
	Micro::wait_ms(1000);
	SDD1306::clear();
	
	write_font<DePixelKlein>("DePixelKlein (ASCII letters)");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::print<DePixelKlein>({0,0}, "Change font_test to see others fonts");
	Micro::wait_ms(5000);
	SDD1306::clear();

    } else if constexpr (font_test == 2){
	using dogica = rom::font_dogica_8x8_cr::Font;
	using dogicabold = rom::font_dogicabold_8x8_cr::Font;
	using dogicapixel = rom::font_dogicapixel_7x8_cr::Font;
	using dogicapixelbold = rom::font_dogicapixelbold_8x8_cr::Font;
	using HomeVideo = rom::font_HomeVideo_10x14_cr::Font;
	using mai10 = rom::font_mai10_10x14_cr::Font;

	SDD1306::print<dogica>({0,0},
		    "Lets see some fonts");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::print<dogica>({2,0}, "dogica");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<dogicabold>({2,0}, "dogicabold");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<dogicapixel>({2,0}, "dogicapixel");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<dogicapixelbold>({2,0}, "dogicapixelbold");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<HomeVideo>({2,0}, "HomeVideo");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<mai10>({2,0}, "mai10");
	Micro::wait_ms(1000);
	SDD1306::clear();

	write_font<dogica>("dogica (ASCII letters)");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::print<dogica>({2,0}, "Change font_test to see others fonts");
	Micro::wait_ms(5000);
	SDD1306::clear();

    } else if constexpr (font_test == 3){
	using Minecraft = rom::font_Minecraft_14x16_cr::Font;
	using PerfectDOSVGA437 = rom::font_PerfectDOSVGA437_8x15_cr::Font;
	using PerfectDOSVGA437Win = rom::font_PerfectDOSVGA437Win_8x15_cr::Font;
	using rainyhearts = rom::font_rainyhearts_11x13_cr::Font;
	using RetroGaming = rom::font_RetroGaming_11x13_cr::Font;
	using upheavtt = rom::font_upheavtt_13x14_cr::Font;
	using VCR = rom::font_VCR_12x17_cr::Font;

	SDD1306::print<PerfectDOSVGA437>({0,0},
		    "Lets see some fonts");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::clear();
	SDD1306::print<Minecraft>({2,0}, "Minecraft");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<PerfectDOSVGA437>({2,0}, "PerfectDOSVGA437");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<PerfectDOSVGA437Win>({2,0}, "PerfectDOSVGA437Win");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<rainyhearts>({2,0}, "rainyhearts");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<RetroGaming>({2,0}, "RetroGaming");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<upheavtt>({2,0}, "upheavtt");
	Micro::wait_ms(1000);
	SDD1306::clear();

	SDD1306::print<VCR>({2,0}, "VCR");
	Micro::wait_ms(1000);
	SDD1306::clear();

	write_font<PerfectDOSVGA437>("PerfectDOSVGA437 (ASCII letters)");
	Micro::wait_ms(2000);
	SDD1306::clear();

	SDD1306::print<PerfectDOSVGA437>({0,0}, "Change font_test to see others fonts");
	Micro::wait_ms(5000);
	SDD1306::clear();
    }
}



// Main
// ----
int main() 
{

    init_uart();
    hello();

    init_TWI();

    SDD1306::init<SDD1306_init_cfg>();

    UART_iostream uart;

    while(1){
//	test_basic();
	test_font();
    }
}


ISR_TWI
{
    TWI_master::handle_interrupt();
}


