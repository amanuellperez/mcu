// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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


#include "../../../dev_LCD_HD44780.h"
#include <avr_time.h>
#include <stddef.h>


// Si lo conectamos solo a 4 pins de datos
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

using LCD_HD44780_1602_ostream = dev::LCD_HD44780_1602_ostream<LCD_HD44780>;
using LCD_HD44780_2004_ostream = dev::LCD_HD44780_2004_ostream<LCD_HD44780>;


void test_lcd_ostream4()
{
//    LCD_HD44780_1602_ostream lcd;
    LCD_HD44780_2004_ostream lcd;

    while(1){
	lcd.clear();
	lcd << "LCD_HD44780_ostream";
	wait_ms(1000);
	char c = 'x';
	lcd << "\nUn caracter (x): [" << c << "]";
	wait_ms(1000);
	lcd << "\nUna frase un poco larga...";
	wait_ms(1500);
	char u8 = 'c';
	lcd << "\nUn uint8_t(c): [" << u8 << "]";
	wait_ms(1000);
	int8_t i8 = -100;
	lcd << "\nUn int8_t (-100): [" << i8 << "]";
	wait_ms(1000);
	uint16_t u16 = 65500;
	lcd << "\nUn uint16_t(65500): [" << u16 << "]";
	wait_ms(1000);
	int16_t i16 = -10000;
	lcd << "\nUn int16_t (-10000): [" << i16 << "]";
	wait_ms(1000);
	uint32_t u32 = 1000000;
	lcd << "\nUn uint32_t(1000000): [" << u32 << "]";
	wait_ms(1000);
    }
}


int main()
{
    test_lcd_ostream4();
}



