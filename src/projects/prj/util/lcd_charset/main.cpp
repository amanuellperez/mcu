// Copyright (C) 2021 Manuel Perez 
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

// ¿Cuál es el código de caracteres que tiene el LCD?
#include <dev_HD44780.h>

#include <algorithm>

#include <avr_atmega.h>

namespace myu = atmega;
using Micro = myu::Micro;

// pins
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
using LCD_1602 = dev::LCD_HD44780_1602<Micro, LCD_pins_1602>;
using LCD_2004 = dev::LCD_HD44780_2004<Micro, LCD_pins_2004>;
using LCD_4004 = dev::LCD_HD44780_4004<Micro, LCD_pins_4004>;

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


void print(LCD& lcd, char i)
{
    lcd.clear();
    for (char c = i; c < i + 16; ++c){
	lcd.print(c);
    }
    lcd.cursor_pos(0,1);
    i += 16;
    for (char c = i; c < std::min(i + 16, 255); ++c){
	lcd.print(c);
    }
    if (i == 240)
	lcd.print(255);
}


void show_charset()
{
    LCD lcd;

    while(1){
	for (char i = 0; i < 8; ++i){
	    print(lcd, 32*i);
	    myu::Micro::wait_ms(3000);
	}
    }
}

int main()
{
    show_charset();
}

