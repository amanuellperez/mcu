// Copyright (C) 2022 Manuel Perez 
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

#include "../../../dev_LCD_screen.h"
#include <dev_HD44780_generic.h>
#include <rom_glyphs_5x8.h>
#include <avr_atmega.h>
#include <stddef.h>

namespace myu = atmega;

// Conexiones
using LCD_pins_1602 = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;
using LCD_pins_2004 = LCD_pins_1602;



using LCD_pins_4004 = dev::LCD_HD44780_4004_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_4004_E<6, 10>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

// Dispositivos reales conectados
using LCD_1602 = dev::LCD_HD44780_1602<myu::Micro, LCD_pins_1602>;
using LCD_2004 = dev::LCD_HD44780_2004<myu::Micro, LCD_pins_2004>;
using LCD_4004 = dev::LCD_HD44780_4004<myu::Micro, LCD_pins_4004>;

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;
using Generic_LCD_4004 = dev::Generic_LCD<LCD_4004>;

// Screens
using Screen_1602 = dev::LCD_screen_1602<Generic_LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<Generic_LCD_2004>;
using Screen_4004 = dev::LCD_screen_4004<Generic_LCD_4004>;

// Choose LCD to test
using LCD = Screen_1602;
//using LCD = Screen_2004;
//using LCD = Screen_4004; <-- sin probar

namespace gl = rom::glyphs_5x8;
using Glyph = gl::Glyph<myu::ROM_read>;


void new_extended_char(LCD& lcd,
	const Glyph& g1, 
	const Glyph& g2, 
	const Glyph& g3, 
	const Glyph& g4, 
	const Glyph& g5, 
	const Glyph& g6, 
	const Glyph& g7, 
	const Glyph& g8)
{
    lcd.new_extended_char(0, g1);
    lcd.new_extended_char(1, g2);
    lcd.new_extended_char(2, g3);
    lcd.new_extended_char(3, g4);
    lcd.new_extended_char(4, g5);
    lcd.new_extended_char(5, g6);
    lcd.new_extended_char(6, g7);
    lcd.new_extended_char(7, g8);
}

void print_extended(LCD& lcd)
{
    for (uint8_t i = 0; i < 8; ++i)
	lcd.print_extended(i);
}


void test_extended_chars(LCD& lcd)
{
    new_extended_char(lcd,
                gl::bell,
                gl::arrow_up,
                gl::arrow_down,
                gl::man,
                gl::skull,
                gl::speaker_right,
                gl::plug,
                gl::musical_note);
    lcd.clear();
    lcd.print("First:[");
    print_extended(lcd);
    lcd.print("]");
    myu::Micro::wait_ms(2000);

    new_extended_char(lcd,
                gl::pacman,
		gl::pacman_phantom,
                gl::exponent2,
                gl::exponent3,
                gl::cuberoot,
                gl::speaker_right,
                gl::plug,
                gl::musical_note);

    lcd.clear();
    lcd.print("Second:[");
    print_extended(lcd);
    lcd.print("]");
    myu::Micro::wait_ms(2000);

    new_extended_char(lcd,
                gl::heart_full,
                gl::heart_empty,
                gl::padlock_close,
                gl::padlock_open,
                gl::battery_empty,
                gl::battery_half,
                gl::battery_full,
                gl::exponent2);

    lcd.clear();
    for (uint8_t i = 0; i < 10; ++i)
    {
	lcd.cursor_pos(0,0);
	lcd.print_extended(0);
	lcd.print_extended(2);
	myu::Micro::wait_ms(500);
	lcd.cursor_pos(0,0);
	lcd.print_extended(1);
	lcd.print_extended(3);
	myu::Micro::wait_ms(500);
    }

}

void test_lcd_screen4_1602()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen(1602)");
    myu::Micro::wait_ms(1000);

// ------
    test_extended_chars(lcd);
}


void test_lcd_screen4_2004()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen (20 x 04)");
    myu::Micro::wait_ms(1000);


    test_extended_chars(lcd);
}


void test_lcd_screen4_4004()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen (40 x 04)");
    myu::Micro::wait_ms(1000);


    test_extended_chars(lcd);
}



void test_lcd_screen4()
{
    switch(LCD::cols()){
	case 16: test_lcd_screen4_1602(); break;
	case 20: test_lcd_screen4_2004(); break;
	case 40: test_lcd_screen4_4004(); break;

	default: test_lcd_screen4_1602(); break;
    }
}


int main()
{
    while(1){
	test_lcd_screen4();
    }
}



