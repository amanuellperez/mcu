// Copyright (C) 2019-2020 Manuel Perez 
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


#include "../../../dev_HD44780_basic.h"

#include <cstring>
#include <avr_atmega.h>
#include <rom_glyphs_5x8.h>

namespace myu = atmega;

// Si lo conectamos solo a 4 pins de datos
using LCD_pins = dev::LCD_HD44780_4004_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_4004_E<6, 10>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD = dev::LCD_HD44780_4004<LCD_pins>;

namespace gl = rom::glyphs_5x8;
using Glyph = gl::Glyph<myu::ROM_read>;

void print1(LCD& lcd, const char* c)
{
    while(*c)
	lcd.write_data_to_CG_or_DDRAM1(*c++);
}

void print2(LCD& lcd, const char* c)
{
    while(*c)
	lcd.write_data_to_CG_or_DDRAM2(*c++);
}

// Probamos el LCD conectado a 4 pines de datos 
void test_lcd4(LCD& lcd)
{
// -----
    // Probamos que funcionen las funciones básicas de impresión
    // Si no el resto no funcionará.
    lcd.clear_display();
    lcd.write_data_to_CG_or_DDRAM1('O');
    lcd.write_data_to_CG_or_DDRAM1('K');
    lcd.write_data_to_CG_or_DDRAM1('?');
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear_display();
    lcd.return_home();
    const char copy_msg[] = "Duplicating this row";
    print1(lcd, copy_msg);
    for (uint8_t x = 0; x < std::strlen(copy_msg); ++x){
	lcd.set_ddram_address1(0x00 + x);
	uint8_t c = lcd.read_data_from_CG_or_DDRAM1();
	lcd.set_ddram_address1(0x40 + x);
	lcd.write_data_to_CG_or_DDRAM1(c);

	lcd.set_ddram_address2(0x00 + x);
	lcd.write_data_to_CG_or_DDRAM2(c);

	lcd.set_ddram_address2(0x40 + x);
	lcd.write_data_to_CG_or_DDRAM2(c);
	myu::Micro::wait_ms(100);
    }
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear_display();
    print1(lcd, "clear_display: is clear the display? ");
    myu::Micro::wait_ms(2000);

// -----
    lcd.clear_display();
    print1(lcd, "Testing return_home");
    print2(lcd, "Testing return_home");
    myu::Micro::wait_ms(1000);
    lcd.return_home();
    print1(lcd, "At the begining (1)?");
    print2(lcd, "At the begining (2)?");
    myu::Micro::wait_ms(1500);

// -----
    const char left_shift[] = "This is a very long left shift.";

    lcd.clear_display();
    print1(lcd, "Shift to the left.");
    print2(lcd, "Shift to the left.");
    lcd.entry_mode1(true, true);
    myu::Micro::wait_ms(500);
    const char* p = &left_shift[0];
    while (*p){
	lcd.write_data_to_CG_or_DDRAM1(*p);
	lcd.write_data_to_CG_or_DDRAM2(*p);
	myu::Micro::wait_ms(500);
	++p;
    }
    myu::Micro::wait_ms(1000);

// -----
    const char right_shift[] = "very long right shift";

    lcd.clear_display();
    lcd.entry_mode1(true, false);	// recordar dejar shift = false
    print1(lcd, "Shift to the right.");
    print2(lcd, "Shift to the right.");
    myu::Micro::wait_ms(1000);
    lcd.entry_mode1(false, true);
    myu::Micro::wait_ms(500);
    p = &right_shift[0];
    while (*p){
	lcd.write_data_to_CG_or_DDRAM1(*p);
	lcd.write_data_to_CG_or_DDRAM2(*p);
	myu::Micro::wait_ms(500);
	++p;
    }
    myu::Micro::wait_ms(1000);
    lcd.entry_mode1(true, false);	// recordar dejar shift = false

// -----
    lcd.clear_display();
    print1(lcd, "Turn off 1 second");
    print2(lcd, "Turn off 1 second");
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);
    myu::Micro::wait_ms(1000);
    lcd.display_control1(false, false, false);
    lcd.display_control2(false, false, false);
    myu::Micro::wait_ms(1000);
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);
    myu::Micro::wait_ms(1000);
    lcd.clear_display();
    print1(lcd, "On?");
    print2(lcd, "On?");
    myu::Micro::wait_ms(1000);

// -----
    lcd.clear_display();
    print1(lcd, "Cursor on");
    print2(lcd, "Cursor on");
    lcd.display_control1(true, true, false);
    lcd.display_control2(true, true, false);
    myu::Micro::wait_ms(1000);
    lcd.clear_display();
    print1(lcd, "Cursor off");
    print2(lcd, "Cursor off");
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);
    myu::Micro::wait_ms(2000);
    lcd.clear_display();
    print1(lcd, "Cursor blink on");
    print2(lcd, "Cursor blink on");
    lcd.display_control1(true, false, true);
    lcd.display_control2(true, false, true);
    myu::Micro::wait_ms(3000);
    lcd.clear_display();
    print1(lcd, "Cursor blink off");
    print2(lcd, "Cursor blink off");
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);
    myu::Micro::wait_ms(1000);
    // Lo dejamos sin cursor
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);

// -----
    lcd.clear_display();

    lcd.display_control1(true, true, false);
    lcd.display_control2(true, true, false);

    print1(lcd, left_shift);
    print2(lcd, left_shift);
    for (uint8_t i = 0; i < 40; ++i){
	lcd.cursor_or_display_shift1(true, false);
	myu::Micro::wait_ms(200);
    }
    // Lo dejamos como estaba
    lcd.return_home();

    lcd.clear_display();
    print1(lcd, right_shift);
    print2(lcd, right_shift);
    for (uint8_t i = 0; i < 40; ++i){
	lcd.cursor_or_display_shift1(true, true);
	myu::Micro::wait_ms(200);
    }
    // Lo dejamos como estaba
    lcd.return_home();

    lcd.clear_display();
    print1(lcd, "Move cursor to the left");
    print2(lcd, "Move cursor to the left");
    for (uint8_t i = 0; i < 15; ++i){
	lcd.cursor_or_display_shift1(false, false);
	myu::Micro::wait_ms(300);
    }

// -----
    lcd.clear_display();
    print1(lcd, "Move cursor to the right");
    print2(lcd, "Move cursor to the right");
    lcd.set_ddram_address1(0); // coloco el cursor al principio
    lcd.set_ddram_address2(0); // coloco el cursor al principio
    for (uint8_t i = 0; i < 15; ++i){
	lcd.cursor_or_display_shift1(false, true);
	myu::Micro::wait_ms(300);
    }

    // Lo dejamos sin cursor
    lcd.display_control1(true, false, false);
    lcd.display_control2(true, false, false);


// -----
    lcd.clear_display();
    lcd.set_ddram_address1(3);
    print1(lcd, "from x = 3");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
    lcd.set_ddram_address1(0x00);
    print1(lcd, "First row");
    lcd.set_ddram_address1(0x40);
    print1(lcd, "Second row");
    lcd.set_ddram_address2(0x00);
    print2(lcd, "Third row");
    lcd.set_ddram_address2(0x40);
    print2(lcd, "Fourth row");
    myu::Micro::wait_ms(1000);

    lcd.clear_display();
}



void new_glyph(LCD& lcd, const Glyph& glyph)
{
    for (uint8_t i  = 0; i < 8; ++i)
	lcd.write_data_to_CG_or_DDRAM1(glyph[i]);

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
    lcd.set_cgram_address1(0x00);
    new_glyph(lcd, g1);
    new_glyph(lcd, g2);
    new_glyph(lcd, g3);
    new_glyph(lcd, g4);
    new_glyph(lcd, g5);
    new_glyph(lcd, g6);
    new_glyph(lcd, g7);
    new_glyph(lcd, g8);


    lcd.set_ddram_address1(0x00);
    lcd.write_data_to_CG_or_DDRAM1(0x00);
    lcd.write_data_to_CG_or_DDRAM1(0x01);
    lcd.write_data_to_CG_or_DDRAM1(0x02);
    lcd.write_data_to_CG_or_DDRAM1(0x03);
    lcd.write_data_to_CG_or_DDRAM1(0x04);
    lcd.write_data_to_CG_or_DDRAM1(0x05);
    lcd.write_data_to_CG_or_DDRAM1(0x06);
    lcd.write_data_to_CG_or_DDRAM1(0x07);
}

void test_cgram4(LCD& lcd)
{

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
                gl::heart_full,
                gl::heart_empty,
                gl::padlock_close,
                gl::padlock_open,
                gl::battery_empty,
                gl::battery_half,
                gl::battery_full,
                gl::musical_note);

    for (uint8_t i = 0; i < 10; ++i)
    {
	lcd.set_ddram_address1(0x40);
	lcd.write_data_to_CG_or_DDRAM1(0x00);
	lcd.write_data_to_CG_or_DDRAM1(0x02);
	myu::Micro::wait_ms(500);
	lcd.set_ddram_address1(0x40);
	lcd.write_data_to_CG_or_DDRAM1(0x01);
	lcd.write_data_to_CG_or_DDRAM1(0x03);
	myu::Micro::wait_ms(500);
    }


}




int main()
{
    LCD lcd;

    while(1){
	test_lcd4(lcd);
	test_cgram4(lcd);
    }
}



