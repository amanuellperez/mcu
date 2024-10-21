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


#include <dev_HD44780_generic.h>
#include "../../../dev_LCD_ostream.h"
#include <avr_time.h>
#include <stddef.h>

#include <atd_double.h>

namespace myu = atmega;

// pins
using LCD_pins_1602 = dev::HD44780_pins4<dev::HD44780_RS<4>,
				       dev::HD44780_RW<5>,
				       dev::HD44780_E<6>,
				       dev::HD44780_D4<11,12,13,14>
				       >;

using LCD_pins_2004 = LCD_pins_1602;


using LCD_pins_4004 = dev::HD44780_4004_pins4<dev::HD44780_RS<4>,
				       dev::HD44780_RW<5>,
				       dev::HD44780_4004_E<6, 10>,
				       dev::HD44780_D4<11,12,13,14>
				       >;


// Dispositivos reales conectados
using LCD_1602 = dev::LCD_HD44780_1602<myu::Micro, LCD_pins_1602>;
using LCD_2004 = dev::LCD_HD44780_2004<myu::Micro, LCD_pins_2004>;
using LCD_4004 = dev::LCD_HD44780_4004<myu::Micro, LCD_pins_4004>;

// ostreams
using LCD_ostream_1602 = dev::LCD_ostream_1602<LCD_1602>;
using LCD_ostream_2004 = dev::LCD_ostream_2004<LCD_2004>;
using LCD_ostream_4004 = dev::LCD_ostream_4004<LCD_4004>;


using LCD_ostream = LCD_ostream_1602;
//using LCD_ostream = LCD_ostream_2004;
//using LCD_ostream = LCD_ostream_4004;

void test_lcd_ostream4()
{
    LCD_ostream lcd;

    while(1){
	lcd.clear();
	lcd << "LCD_HD44780_ostream";
	myu::Micro::wait_ms(1000);

	char c = 'x';
	lcd << "\nUn caracter (x): [" << c << "]";
	myu::Micro::wait_ms(1000);
        lcd << "\nUna frase que sea un poco larga, para que no entre en una "
               "linea...";
        myu::Micro::wait_ms(1500);
	char u8 = 'c';
	lcd << "\nUn uint8_t(c): [" << u8 << "]";
	myu::Micro::wait_ms(1000);
	int8_t i8 = -100;
	lcd << "\nUn int8_t (-100): [" << i8 << "]";
	myu::Micro::wait_ms(1000);
	uint16_t u16 = 65500;
	lcd << "\nUn uint16_t(65500): [" << u16 << "]";
	myu::Micro::wait_ms(1000);
	int16_t i16 = -10000;
	lcd << "\nUn int16_t (-10000): [" << i16 << "]";
	myu::Micro::wait_ms(1000);
	uint32_t u32 = 1000000;
	lcd << "\nUn uint32_t(1000000): [" << u32 << "]";
	myu::Micro::wait_ms(1000);

    {// double
	double a = 2;
	double b = 8;
	double c = pow(a, b);
	lcd.clear();
	lcd << "double\n2^8 = ";
	atd::print(lcd, c);
	myu::Micro::wait_ms(1000);
    }
    }
}


int main()
{
    test_lcd_ostream4();
}



