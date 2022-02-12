// Copyright (C) 2022 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
#include "../../../dev_LCD_HD44780_generic.h"
#include "../../../dev_LCD_font_t1.h"
#include "../../../dev_LCD_HD44780_charset.h"
#include <avr_time.h>
#include <stddef.h>


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
//using LCD = Screen_1602;
using LCD = Screen_2004;
//using LCD = Screen_4004;


using Big_digit_t11_2x1 = dev::Big_digit_t11_2x1<LCD>; 
using Big_digit_t1_3x3 = dev::Big_digit_t1_3x3<LCD>; 

// Mostramos los bricks cargados en memoria
void show_bricks()
{
    LCD lcd;
    
    Big_digit_t1_3x3::init(lcd);
    lcd.clear();
    for (uint8_t i = 0; i < 8; ++i)
	lcd.print_extended(i);

    wait_ms(1000);
}


template <typename BD> // BD = Big_digit
void test_big_digits(LCD& lcd)
{
    BD::init(lcd);
    lcd.clear();
    lcd.cursor_pos(0,0);
    BD::print(lcd, 0);
    BD::print(lcd, 1);
    BD::print(lcd, 2);
    BD::print(lcd, 3);
    BD::print(lcd, 4);
    BD::print(lcd, 5);
    wait_ms(2000);
    lcd.clear();
    BD::print(lcd, 6);
    BD::print(lcd, 7);
    BD::print(lcd, 8);
    BD::print(lcd, 9);
    wait_ms(2000);

    lcd.clear();
    for (uint8_t i = 0; i <= 9; ++i){
	lcd.cursor_pos(0,0);
	BD::print(lcd, i);
	wait_ms(500);
    }
}

void test_big_digits()
{
    LCD lcd;

    while (1){
	lcd.clear();
	lcd.print("Big digit  t1.1 2x1");
	wait_ms(1000);

	test_big_digits<Big_digit_t11_2x1>(lcd);

	lcd.clear();
	lcd.print("Big digit  t1 3x3");
	wait_ms(1000);

	test_big_digits<Big_digit_t1_3x3>(lcd);

    }
}

int main()
{
    while(1){
//	show_bricks();
	test_big_digits();
    }
}



