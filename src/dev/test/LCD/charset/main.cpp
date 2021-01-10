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



#include "../../../dev_LCD_screen.h"
#include "../../../dev_LCD_HD44780_basic.h"
#include "../../../dev_LCD_HD44780_charset.h"
#include <avr_time.h>
#include <stddef.h>



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

// Tipos de LCD screen probados
using LCD_HD44780_1602_screen = dev::LCD_screen_1602<LCD_HD44780>;
using LCD_HD44780_2004_screen = dev::LCD_screen_2004<LCD_HD44780>;
using LCD_HD44780_4004_screen = dev::LCD_screen_4004<LCD_HD44780_4004>;

// Choose what LCD to test
//using LCD = LCD_HD44780_1602_screen;
//using LCD = LCD_HD44780_2004_screen;
using LCD = LCD_HD44780_4004_screen;

using symbol = dev::HD44780_charset_A00;

void print1(LCD& lcd)
{
    lcd.clear();
    lcd.print(symbol::of("a")); // si falla imprime '?' si no lo conoce
    lcd.print(symbol::of("→"));
    lcd.print(symbol::of("←"));

    lcd.cursor_pos(0,1);
    lcd.print(symbol::of("｡"));
    lcd.print(symbol::of("｢"));
    lcd.print(symbol::of("｣"));
    lcd.print(symbol::of("､"));
    lcd.print(symbol::of("･"));
    lcd.print(symbol::of("ｦ"));
    lcd.print(symbol::of("ｧ"));
    lcd.print(symbol::of("ｨ"));
    lcd.print(symbol::of("ｩ"));
    lcd.print(symbol::of("ｪ"));
    lcd.print(symbol::of("ｫ"));
    lcd.print(symbol::of("ｬ"));
    lcd.print(symbol::of("ｭ"));
    lcd.print(symbol::of("ｮ"));
    lcd.print(symbol::of("ｯ"));

}


void print2(LCD& lcd)
{
    lcd.clear();
    lcd.print(symbol::of("ｰ"));
    lcd.print(symbol::of("ｱ"));
    lcd.print(symbol::of("ｲ"));
    lcd.print(symbol::of("ｳ"));
    lcd.print(symbol::of("ｴ"));
    lcd.print(symbol::of("ｵ"));
    lcd.print(symbol::of("ｶ"));
    lcd.print(symbol::of("ｷ"));
    lcd.print(symbol::of("ｸ"));
    lcd.print(symbol::of("ｹ"));
    lcd.print(symbol::of("ｺ"));
    lcd.print(symbol::of("ｻ"));
    lcd.print(symbol::of("ｼ"));
    lcd.print(symbol::of("ｽ"));
    lcd.print(symbol::of("ｾ"));
    lcd.print(symbol::of("ｿ"));

    lcd.cursor_pos(0,1);
    lcd.print(symbol::of("ﾀ"));
    lcd.print(symbol::of("ﾁ"));
    lcd.print(symbol::of("ﾂ"));
    lcd.print(symbol::of("ﾃ"));
    lcd.print(symbol::of("ﾄ"));
    lcd.print(symbol::of("ﾅ"));
    lcd.print(symbol::of("ﾆ"));
    lcd.print(symbol::of("ﾇ"));
    lcd.print(symbol::of("ﾈ"));
    lcd.print(symbol::of("ﾉ"));
    lcd.print(symbol::of("ﾊ"));
    lcd.print(symbol::of("ﾋ"));
    lcd.print(symbol::of("ﾌ"));
    lcd.print(symbol::of("ﾍ"));
    lcd.print(symbol::of("ﾎ"));
    lcd.print(symbol::of("ﾏ"));
}


void print3(LCD& lcd)
{
    lcd.clear();

    lcd.print(symbol::of("ﾐ"));
    lcd.print(symbol::of("ﾑ"));
    lcd.print(symbol::of("ﾒ"));
    lcd.print(symbol::of("ﾓ"));
    lcd.print(symbol::of("ﾔ"));
    lcd.print(symbol::of("ﾕ"));
    lcd.print(symbol::of("ﾖ"));
    lcd.print(symbol::of("ﾗ"));
    lcd.print(symbol::of("ﾘ"));
    lcd.print(symbol::of("ﾙ"));
    lcd.print(symbol::of("ﾚ"));
    lcd.print(symbol::of("ﾛ"));
    lcd.print(symbol::of("ﾜ"));
    lcd.print(symbol::of("ﾝ"));
    lcd.print(symbol::of("ﾞ"));
    lcd.print(symbol::of("º"));

    lcd.cursor_pos(0,1);
    lcd.print(symbol::of("α"));
    lcd.print(symbol::of("ä"));
    lcd.print(symbol::of("β"));
    lcd.print(symbol::of("ε"));
    lcd.print(symbol::of("μ"));
    lcd.print(symbol::of("σ"));
    lcd.print(symbol::of("ρ"));
    lcd.print(symbol::of("gg"));
    lcd.print(symbol::of("√"));
    lcd.print(symbol::of("-1"));
    lcd.print(symbol::of("jj"));
    lcd.print(symbol::of("ˣ"));
    lcd.print(symbol::of("¢"));
    lcd.print(symbol::of("£"));
    lcd.print(symbol::of("ñ"));
    lcd.print(symbol::of("ö"));
}


void print4(LCD& lcd)
{
    lcd.clear();

    lcd.print(symbol::of("pp"));
    lcd.print(symbol::of("qq"));
    lcd.print(symbol::of("θ"));
    lcd.print(symbol::of("\x22\x1e"));
    lcd.print(symbol::of("Ω"));
    lcd.print(symbol::of("ü"));
    lcd.print(symbol::of("Σ"));
    lcd.print(symbol::of("π"));
    lcd.print(symbol::of("\x53\x43"));
    lcd.print(symbol::of("yy"));
    lcd.print(symbol::of("于"));
    lcd.print(symbol::of("万"));
    lcd.print(symbol::of("円"));
    lcd.print(symbol::of("÷"));
    lcd.print(symbol::of("■"));
    lcd.print(symbol::of("█"));

}

void test_symbols()
{
    LCD lcd;
    lcd.stop_brcorner(true);

    print1(lcd);
    wait_ms(5000);
    print2(lcd);
    wait_ms(5000);
    print3(lcd);
    wait_ms(5000);
    print4(lcd);
    wait_ms(5000);


}


int main()
{
    while(1){
	test_symbols();
    }
}



