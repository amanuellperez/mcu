// Copyright (C) 2021 A.Manuel L.Perez 
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

// ¿Cuál es el código de caracteres que tiene el LCD?
#include <dev_LCD_screen.h>
#include <dev_LCD_HD44780_basic.h>

#include <algorithm>

// 16 x 02 and 20 x 04
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;


// 40 x 04
using LCD_4004_pins = dev::LCD_HD44780_4004_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_4004_E<6, 10>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780_4004 = dev::LCD_HD44780_4004<LCD_4004_pins>;


using LCD_HD44780_screen_1602 = dev::LCD_screen_1602<LCD_HD44780>;
using LCD_HD44780_screen_2004 = dev::LCD_screen_2004<LCD_HD44780>;
using LCD_HD44780_screen_4004 = dev::LCD_screen_4004<LCD_HD44780_4004>;

using LCD = LCD_HD44780_screen_1602;
//using LCD = LCD_HD44780_screen_2004;
//using LCD = LCD_HD44780_screen_4004;

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
    lcd.stop_brcorner(true);

    while(1){
	for (char i = 0; i < 8; ++i){
	    print(lcd, 32*i);
	    wait_ms(3000);
	}
    }
}

int main()
{
    show_charset();
}

