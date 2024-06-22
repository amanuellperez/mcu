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
#include "../../../dev_LCD_font_2x1.h"
#include "../../../dev_LCD_font_2x2.h"
#include "../../../dev_LCD_font_2x3.h"
#include "../../../dev_LCD_font_3x3.h"
#include "../../../dev_LCD_font_4x3.h"
#include <dev_HD44780_generic.h>
#include <dev_HD44780_charset.h>
#include <avr_time.h>
#include <stddef.h>


namespace my_mcu = atmega;
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
using LCD_1602 = dev::LCD_HD44780_1602<LCD_pins_1602>;
using LCD_2004 = dev::LCD_HD44780_2004<LCD_pins_2004>;
using LCD_4004 = dev::LCD_HD44780_4004<LCD_pins_4004>;

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;
using Generic_LCD_4004 = dev::Generic_LCD<LCD_4004>;

// Screens
using Screen_1602 = dev::LCD_screen_1602<Generic_LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<Generic_LCD_2004>;
using Screen_4004 = dev::LCD_screen_4004<Generic_LCD_4004>;

// Choose LCD to test
//using LCD = Screen_1602; <-- ESTE NO MUESTRA LAS LETRAS DE 3 y 4 FILAS!!!
using LCD = Screen_2004;
//using LCD = Screen_4004;

// Estas son las fuentes que vamos a usar.
// DUDA: ¿mejor llamarlas Font_BD2x1_t1? ¿o Digits_font_2x1_t1?
using Font_digit_default = dev::Font_digit_default;
using Font_digit_2x1_t1 = dev::Font_digit_2x1_t1; 
using Font_digit_2x1_t2 = dev::Font_digit_2x1_t2; 
using Font_digit_2x2_t1 = dev::Font_digit_2x2_t1; 
using Font_digit_2x3_t1 = dev::Font_digit_2x3_t1; 
using Font_digit_3x3_t1 = dev::Font_digit_3x3_t1; 
using Font_digit_4x3_t1 = dev::Font_digit_4x3_t1; 

// Mostramos los bricks cargados en memoria
void show_bricks()
{
    LCD lcd;
    
    Font_digit_3x3_t1::load(lcd);
    lcd.clear();
    for (uint8_t i = 0; i < 8; ++i)
	lcd.print_extended(i);

    my_mcu::Micro::wait_ms(1000);
}


template <typename BD> // BD = Font_digit
void test_big_digits(LCD& lcd, bool stop = false)
{
    BD::load(lcd);
    lcd.clear();
    lcd.cursor_pos(0,0);
    BD::print(lcd, 0);
    BD::print(lcd, 1);
    BD::print(lcd, 2);
    BD::print(lcd, 3);
    BD::print(lcd, 4);
    BD::print(lcd, 5);

    if (stop){
	my_mcu::Micro::wait_ms(2000);
	lcd.clear();
    }

    BD::print(lcd, 6);
    BD::print(lcd, 7);
    BD::print(lcd, 8);
    BD::print(lcd, 9);
    my_mcu::Micro::wait_ms(2000);

    lcd.clear();
    for (uint8_t i = 0; i <= 9; ++i){
	lcd.cursor_pos(0,0);
	BD::print(lcd, i);
	my_mcu::Micro::wait_ms(500);
    }
}

template <typename Digit>
void test_big_digits(LCD& lcd, const char* name, bool stop = false)
{
    lcd.clear();
    lcd.print(name);
    my_mcu::Micro::wait_ms(1000);
    test_big_digits<Digit>(lcd, stop);
}

void test_big_digits_fonts(LCD& lcd)
{
    test_big_digits<Font_digit_default>(lcd, "Default digit");
    test_big_digits<Font_digit_2x1_t1>(lcd, "Big digit 2x1 t1");
    test_big_digits<Font_digit_2x1_t2>(lcd, "Big digit 2x1 t2");
    test_big_digits<Font_digit_2x2_t1>(lcd, "Big digit 2x2 t1");
    test_big_digits<Font_digit_2x3_t1>(lcd, "Big digit 2x3 t1", true);
    test_big_digits<Font_digit_3x3_t1>(lcd, "Big digit 3x3 t1", true);
    test_big_digits<Font_digit_4x3_t1>(lcd, "Big digit 4x3 t1", true);
}

template <typename BD> // BD = Font_digit
void test_big_digits_print(LCD& lcd, const nm::Width<int>& width = 0)
{
    BD::load(lcd);
    lcd.clear();
    for (uint8_t i = 0; i < 20; ++i){
	lcd.cursor_pos(0,0);
	BD::print_number(lcd, i, width);
	my_mcu::Micro::wait_ms(100);
    }

    lcd.cursor_pos(0,0);
    BD::print_number(lcd, 237);
    my_mcu::Micro::wait_ms(500);

}

void test_big_digits_print(LCD& lcd)
{
    lcd.clear();
    lcd.print("Counting without padding");
    my_mcu::Micro::wait_ms(1000);
    test_big_digits_print<Font_digit_default>(lcd);
    test_big_digits_print<Font_digit_2x1_t1>(lcd);
    test_big_digits_print<Font_digit_2x1_t2>(lcd);
    test_big_digits_print<Font_digit_2x2_t1>(lcd);
    test_big_digits_print<Font_digit_2x3_t1>(lcd);
    test_big_digits_print<Font_digit_3x3_t1>(lcd);
    test_big_digits_print<Font_digit_4x3_t1>(lcd);

    lcd.clear();
    lcd.print("Counting with padding");
    my_mcu::Micro::wait_ms(1000);
    test_big_digits_print<Font_digit_2x1_t1>(lcd, 3);
    test_big_digits_print<Font_digit_2x1_t2>(lcd, 3);
    test_big_digits_print<Font_digit_2x2_t1>(lcd, 3);
    test_big_digits_print<Font_digit_2x3_t1>(lcd, 3);
    test_big_digits_print<Font_digit_3x3_t1>(lcd, 3);
    test_big_digits_print<Font_digit_4x3_t1>(lcd, 3);
}


template <typename BD> // BD = Font_digit
void test_big_digits_colon(LCD& lcd)
{
    BD::load(lcd);
    lcd.clear();
    BD::print_number(lcd, 12);
    BD::print_colon(lcd);
    BD::print_number(lcd, 34);
    my_mcu::Micro::wait_ms(2000);

}

void test_big_digits_colon(LCD& lcd)
{
    lcd.clear();
    lcd.print("Testing colon");
    my_mcu::Micro::wait_ms(1000);
    test_big_digits_colon<Font_digit_default>(lcd);
    test_big_digits_colon<Font_digit_2x1_t1>(lcd);
    test_big_digits_colon<Font_digit_2x1_t2>(lcd);
    test_big_digits_colon<Font_digit_2x2_t1>(lcd);
    test_big_digits_colon<Font_digit_2x3_t1>(lcd);
    test_big_digits_colon<Font_digit_3x3_t1>(lcd);
    test_big_digits_colon<Font_digit_4x3_t1>(lcd);
}

template <typename Font, typename Rep>
void test_bugs(
    LCD& lcd, Rep x, const nm::Width<int>& w, uint8_t col, uint8_t row)
{
    lcd.clear();
//    lcd.cursor_pos(col, row);
//    lcd.template print<Font>(x, w);
    lcd.cursor_pos(3, 1);
    lcd.template print<Font>(8u, nm::Width{4});
}

void test_bugs(LCD& lcd)
{
    lcd.clear();
    lcd.print("Bugs");
    my_mcu::Micro::wait_ms(400);
    {
    using Font = Font_digit_2x2_t1;
    Font::load(lcd);
    lcd.clear();
    uint16_t x = 11;
    nm::Width w = 2;
    uint8_t col = 3;
    uint8_t row = 1;
    test_bugs<Font, uint16_t>(lcd, x, w, col, row);
    my_mcu::Micro::wait_ms(2000);
    }
}


void test_big_digits()
{
    LCD lcd;

    while (1){
//	test_bugs(lcd);
	test_big_digits_fonts(lcd);
	test_big_digits_print(lcd);
	test_big_digits_colon(lcd);
    }
}

int main()
{
    while(1){
//	show_bricks();
	test_big_digits();
    }
}



