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
#define ATMEGA328P
//#define ATMEGA4809

#include "../../../dev_HD44780_basic.h"


#ifdef ATMEGA328P
#include <mega.h>
#include <rom_glyphs_5x8.h> // siempre despues de avr_atmega.h o equivalente
			    
namespace myu = atmega;

// Si lo conectamos solo a 4 pins de datos
using LCD_pins = dev::HD44780_pins4<dev::HD44780_RS<4>,
				       dev::HD44780_RW<5>,
				       dev::HD44780_E<6>,
				       dev::HD44780_D4<11,12,13,14>
				       >;

void myu_init()
{}

#elif defined ATMEGA4809

#include <mega0.h>
#include <rom_glyphs_5x8.h> // siempre despues de avr_atmega.h o equivalente

namespace myu = atmega4809_40;

// para el atmega4809
using LCD_pins = dev::HD44780_pins4<dev::HD44780_RS<8>,
				       dev::HD44780_RW<9>,
				       dev::HD44780_E<10>,
				       dev::HD44780_D4<11,12,13,14>
				       >;
// Lo pruebo a 1MHz para empezar
void myu_init()
{
    myu::init();
    myu::Clock_controller::clk_main_divided_by_16(); // a 1 MHz
}
#endif

using LCD = dev::HD44780<myu::Micro, LCD_pins>;

namespace gl = rom::glyphs_5x8;
using Glyph = gl::Glyph<myu::ROM_read>;

void print(LCD& lcd, const char* c)
{
    while(*c)
	lcd.write_data_to_CG_or_DDRAM(*c++);
}

void test_static()
{
    LCD::init();

    // Probamos que funcionen las funciones básicas de impresión
    // Si no el resto no funcionará.
    LCD::write_data_to_CG_or_DDRAM('O');
    LCD::write_data_to_CG_or_DDRAM('K');
    LCD::write_data_to_CG_or_DDRAM('?');
    myu::Micro::wait_ms(1000);
}


// Probamos el LCD conectado a 4 pines de datos 
void test_lcd4()
{
    LCD lcd;

    const char shift_izda[] = "shift izda";
    const char shift_dcha[] = "shift dcha";

    // Probamos que funcionen las funciones básicas de impresión
    // Si no el resto no funcionará.
    lcd.write_data_to_CG_or_DDRAM('O');
    lcd.write_data_to_CG_or_DDRAM('K');
    lcd.write_data_to_CG_or_DDRAM('!');
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
    lcd.return_home();
    print(lcd, "copiando esta fi");
    for (uint8_t x = 0; x < 16; ++x){
	lcd.set_ddram_address(0x00 + x);
	uint8_t c = lcd.read_data_from_CG_or_DDRAM();
	lcd.set_ddram_address(0x40 + x);
	lcd.write_data_to_CG_or_DDRAM(c);
	myu::Micro::wait_ms(100);
    }
    myu::Micro::wait_ms(1000);


    lcd.clear_display();
    print(lcd, "Probando clear");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
    myu::Micro::wait_ms(1000);

    print(lcd, "Pruebo return_home");
    myu::Micro::wait_ms(1000);
    lcd.return_home();
    print(lcd, "VUELVO!!!");
    myu::Micro::wait_ms(1500);

    lcd.clear_display();
    print(lcd, "Shift izda:");
    lcd.entry_mode(true, true);
    myu::Micro::wait_ms(500);
    const char* p = &shift_izda[0];
    while (*p){
	lcd.write_data_to_CG_or_DDRAM(*p);
	myu::Micro::wait_ms(500);
	++p;
    }
    myu::Micro::wait_ms(1000);

    lcd.entry_mode(true, false);	// recordar dejar shift = false
    lcd.clear_display();
    print(lcd, "Shift dcha");
    myu::Micro::wait_ms(1000);
    lcd.entry_mode(false, true);
    myu::Micro::wait_ms(500);
    p = &shift_dcha[0];
    while (*p){
	lcd.write_data_to_CG_or_DDRAM(*p);
	myu::Micro::wait_ms(500);
	++p;
    }
    myu::Micro::wait_ms(1000);
    lcd.entry_mode(true, false);	// recordar dejar shift = false

    lcd.clear_display();
    print(lcd, "Apago por 1 segundo");
    lcd.display_control(true, false, false);
    myu::Micro::wait_ms(1000);
    lcd.display_control(false, false, false);
    myu::Micro::wait_ms(1000);
    lcd.display_control(true, false, false);
    myu::Micro::wait_ms(1000);
    lcd.clear_display();
    print(lcd, "Encendido?");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
    print(lcd, "Cursor on");
    lcd.display_control(true, true, false);
    myu::Micro::wait_ms(1000);
    lcd.clear_display();
    print(lcd, "Cursor off");
    lcd.display_control(true, false, false);
    myu::Micro::wait_ms(2000);
    lcd.clear_display();
    print(lcd, "Cursor blink on");
    lcd.display_control(true, false, true);
    myu::Micro::wait_ms(3000);
    lcd.clear_display();
    print(lcd, "Cursor blink off");
    lcd.display_control(true, false, false);
    myu::Micro::wait_ms(1000);
    // Lo dejamos sin cursor
    lcd.display_control(true, false, false);


    lcd.display_control(true, true, false);
    lcd.clear_display();
    print(lcd, "Esta fila hace un shift a la izda");
    for (uint8_t i = 0; i < 40; ++i){
	lcd.cursor_or_display_shift(true, false);
	myu::Micro::wait_ms(200);
    }
    // Lo dejamos como estaba
    lcd.return_home();

    lcd.clear_display();
    print(lcd, "Esta fila hace un shift a la dcha");
    for (uint8_t i = 0; i < 40; ++i){
	lcd.cursor_or_display_shift(true, true);
	myu::Micro::wait_ms(200);
    }
    // Lo dejamos como estaba
    lcd.return_home();

    lcd.clear_display();
    print(lcd, "Move cursor izda");
    for (uint8_t i = 0; i < 15; ++i){
	lcd.cursor_or_display_shift(false, false);
	myu::Micro::wait_ms(300);
    }

    lcd.clear_display();
    print(lcd, "Move cursor dcha");
    lcd.set_ddram_address(0); // coloco el cursor al principio
    for (uint8_t i = 0; i < 15; ++i){
	lcd.cursor_or_display_shift(false, true);
	myu::Micro::wait_ms(300);
    }



    lcd.clear_display();
    lcd.set_ddram_address(3);
    print(lcd, "a partir 3");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
    lcd.set_ddram_address(0x00);
    print(lcd, "Segunda fila");
    lcd.set_ddram_address(0x40);
    print(lcd, "Primera fila");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();

}


void new_glyph(LCD& lcd, const Glyph& glyph)
{
    for (uint8_t i  = 0; i < 8; ++i)
	lcd.write_data_to_CG_or_DDRAM(glyph[i]);

}


void show_glyphs(LCD& lcd,
	const Glyph& g1, 
	const Glyph& g2, 
	const Glyph& g3, 
	const Glyph& g4, 
	const Glyph& g5, 
	const Glyph& g6, 
	const Glyph& g7, 
	const Glyph& g8)
{
    lcd.set_cgram_address(0x00);
    new_glyph(lcd, g1);
    new_glyph(lcd, g2);
    new_glyph(lcd, g3);
    new_glyph(lcd, g4);
    new_glyph(lcd, g5);
    new_glyph(lcd, g6);
    new_glyph(lcd, g7);
    new_glyph(lcd, g8);


    lcd.set_ddram_address(0x00);
    lcd.write_data_to_CG_or_DDRAM(0x00);
    lcd.write_data_to_CG_or_DDRAM(0x01);
    lcd.write_data_to_CG_or_DDRAM(0x02);
    lcd.write_data_to_CG_or_DDRAM(0x03);
    lcd.write_data_to_CG_or_DDRAM(0x04);
    lcd.write_data_to_CG_or_DDRAM(0x05);
    lcd.write_data_to_CG_or_DDRAM(0x06);
    lcd.write_data_to_CG_or_DDRAM(0x07);
}

void test_cgram4()
{
    LCD lcd;
    show_glyphs(lcd,
                gl::bell,
                gl::arrow_up,
                gl::arrow_down,
                gl::man,
                gl::skull,
                gl::speaker_right,
                gl::plug,
                gl::musical_note);

    myu::Micro::wait_ms(2000);

    show_glyphs(lcd,
                gl::pacman,
		gl::pacman_phantom,
                gl::exponent2,
                gl::exponent3,
                gl::cuberoot,
                gl::speaker_right,
                gl::plug,
                gl::musical_note);
    myu::Micro::wait_ms(2000);

    show_glyphs(lcd,
                gl::heart_full,
                gl::heart_empty,
                gl::padlock_close,
                gl::padlock_open,
                gl::battery_empty,
                gl::battery_half,
                gl::battery_full,
                gl::exponent2);

    for (uint8_t i = 0; i < 10; ++i)
    {
	lcd.set_ddram_address(0x40);
	lcd.write_data_to_CG_or_DDRAM(0x00);
	lcd.write_data_to_CG_or_DDRAM(0x02);
	myu::Micro::wait_ms(500);
	lcd.set_ddram_address(0x40);
	lcd.write_data_to_CG_or_DDRAM(0x01);
	lcd.write_data_to_CG_or_DDRAM(0x03);
	myu::Micro::wait_ms(500);
    }


}


int main()
{
    myu_init();

    while(1){
	test_static();
	test_lcd4();
	test_cgram4();
    }
}



