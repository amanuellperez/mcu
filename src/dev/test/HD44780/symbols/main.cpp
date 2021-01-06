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


#include "../../../dev_LCD_HD44780_screen.h"
#include "../../../dev_LCD_HD44780_basic.h"
#include "../../../dev_LCD_HD44780_charset.h"
#include <avr_time.h>
#include <stddef.h>


// Si lo conectamos solo a 4 pins de datos
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;
using LCD_HD44780_1602_screen = dev::LCD_HD44780_1602_screen<LCD_HD44780>;
using LCD_HD44780_2004_screen = dev::LCD_HD44780_2004_screen<LCD_HD44780>;

using LCD = LCD_HD44780_1602_screen;
using symbol = dev::HD44780_charset_A00;


void test_symbols()
{
    LCD lcd;

    lcd.clear();
    lcd.print(symbol::of("a")); // si falla imprime '?' si no lo conoce
    lcd.print(symbol::of("α"));
    lcd.print(symbol::of("\xce\xb1")); // otra forma de escribir alfa
    lcd.print(symbol::of("\x22\x1a"));
    lcd.print(symbol::of("g"));
    lcd.print(symbol::of("ˣ"));
    lcd.print(symbol::of("¢"));
    lcd.print(symbol::of("£"));
    lcd.print(symbol::of("θ"));
    lcd.print(symbol::of("\x22\x1e"));
    lcd.print(symbol::of("Ω"));
    lcd.print(symbol::of("Σ"));
    lcd.print(symbol::of("π"));
    lcd.print(symbol::of("\x53\x43"));
    lcd.print(symbol::of("y"));
    lcd.print(symbol::of("于"));
    lcd.print(symbol::of("万"));
    lcd.print(symbol::of("円"));
    lcd.print(symbol::of("÷"));
    lcd.print(symbol::of("■"));
    lcd.print(symbol::of("█"));


    wait_ms(1000);
}


int main()
{
    while(1){
	test_symbols();
    }
}



