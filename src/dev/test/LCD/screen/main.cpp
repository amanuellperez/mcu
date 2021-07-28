// Copyright (C) 2019-2020 A.Manuel L.Perez 
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
#include "../../../dev_LCD_HD44780_basic.h"
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

// Choose LCD to test
//using LCD = LCD_HD44780_1602_screen;
//using LCD = LCD_HD44780_2004_screen;
using LCD = LCD_HD44780_4004_screen;


// repeat print
void rprint(LCD& lcd, const char* msg)
{
    for (uint8_t i = 0; i < lcd.rows(); ++i){
	lcd.cursor_pos(0, i);
	lcd.print(msg);
    }
    

}

const char long_msg[] = "En un lugar de La Mancha, de cuyo nombre"
                        "no quiero acordarme, no ha mucho tiempo "
                        "que vivia un hidalgo de los de lanza en "
                        "astillero, adarga antigua, rocin flaco y"
			"galgo corredor. Don Quijote (Cervantes)";


void test_lcd_screen4_1602()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen(1602)");
    wait_ms(1000);


// ------
    lcd.clear();
    rprint(lcd, "clear test");
    wait_ms(1000);
    lcd.clear();
    wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This is written in 2 lines");

    wait_ms(2000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?");
    wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    wait_ms(1000);
    lcd.clear_row(0);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("No delete this\n");
    lcd.print("Erase this line");
    wait_ms(1000);
    lcd.clear_row(1);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("scroll up? yes\n");
    lcd.print("12345678901234");
    wait_ms(1000);
    lcd.print('5');
    wait_ms(1000);
    lcd.print('6');
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("scroll up? no\n");
    lcd.print("12345678901234");
    wait_ms(1000);
    lcd.print('5');
    wait_ms(1000);
    lcd.print('6');
    wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    wait_ms(1000);
    lcd.print("\nthree");
    wait_ms(1000);
    lcd.print("\nfour");
    wait_ms(1000);

// -----
    const char menu[] = "First option, is very long";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.print("print4chars(1)");
    lcd.cursor_pos(4, 1);
    const char* p = lcd.print_line_nowrap("1.one\n2.two", 4);
    wait_ms(1000);

// -----
    lcd.cursor_pos(0,0);
    lcd.print("print4chars(2)");
    lcd.cursor_pos(4, 1);
    p = lcd.print_line_nowrap(p, 4);
    wait_ms(1000);


// -----
    lcd.cursor_pos(0,0);
    lcd.print("print4chars'1'");
    lcd.cursor_pos(4, 1);
    lcd.print_line_nowrap("1", 4);
    wait_ms(1000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	wait_ms(40);
	++p;
    }
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    wait_ms(500);
    lcd.cursor_pos(15,0);
    lcd.print('X');
    wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en (15,1)
    lcd.cursor_pos(15,1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    wait_ms(1000);
    lcd.display_off();
    wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    wait_ms(1000);

// -----
    lcd.clear();
    lcd.print("cursor on");
    lcd.cursor_on();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    wait_ms(1000);
}


void test_lcd_screen4_2004()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen (20 x 04)");
    wait_ms(1000);


// ------
    lcd.clear();
    rprint(lcd, "clear test");
    wait_ms(1000);
    lcd.clear();
    wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This sentence has to"
              "be written in 4 line"
              "s. If not, it means "
              "that print is wrong");
    wait_ms(2000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?\n");
    lcd.print("in 3 row?\n");
    lcd.print("in 4 row?");
    wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this\n");
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    wait_ms(1000);
    lcd.clear_row(0);
    lcd.clear_row(2);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line\n");
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line");
    wait_ms(1000);
    lcd.clear_row(1);
    lcd.clear_row(3);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("\n\nscroll up? yes\n");
    lcd.print("123456789012345678");
    wait_ms(1000);
    lcd.print('9');
    wait_ms(1000);
    lcd.print('0');
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("\n\nscroll up? no\n");
    lcd.print("123456789012345678");
    wait_ms(1000);
    lcd.print('9');
    wait_ms(1000);
    lcd.print('0');
    wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    wait_ms(1000);
    lcd.print("\nthree");
    wait_ms(1000);
    lcd.print("\nfour");
    wait_ms(1000);
    lcd.print("\nfive");
    wait_ms(1000);

// -----
    const char menu[] = "First option, is very long\n"
                        "Second option\n"
                        "Third";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    constexpr const char wrap_msg[] = "1.first line\n2.second line";
    lcd.clear();
    lcd.print("print_line_nowrap");
    wait_ms(1000);
    lcd.clear();
    lcd.print("Result: '1.firs'?");
    lcd.cursor_pos(0, 1);
    const char* p = lcd.print_line_nowrap(wrap_msg, 6);
    lcd.cursor_pos(0, 2);
    lcd.print("Res:'2.second line'?");
    lcd.cursor_pos(0, 3);
    p = lcd.print_line_nowrap(p, 20);
    wait_ms(3000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	wait_ms(50);
	++p;
    }
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    wait_ms(500);
    lcd.cursor_pos(lcd.cols()-1,0);
    lcd.print('X');
    wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en la esquina
    lcd.cursor_pos(lcd.cols()-1,lcd.rows()-1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    wait_ms(1000);
    lcd.display_off();
    wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    wait_ms(1000);

// -----
    lcd.clear();
    lcd.print("cursor on");
    lcd.cursor_on();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    wait_ms(1000);
}


void test_lcd_screen4_4004()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen (40 x 04)");
    wait_ms(1000);


// ------
    lcd.clear();
    rprint(lcd, "clear test");
    wait_ms(1000);
    lcd.clear();
    wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This sentence has to be written in a dis"
              "play of 40 columns by 4 rows. It is long"
              "because I want to test the print functio"
              "n. Is it ok? ------------------ the end");

    wait_ms(5000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?\n");
    lcd.print("in 3 row?\n");
    lcd.print("in 4 row?");
    wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this\n");
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    wait_ms(1000);
    lcd.clear_row(0);
    lcd.clear_row(2);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line\n");
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line");
    wait_ms(1000);
    lcd.clear_row(1);
    lcd.clear_row(3);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("\n\nscroll up? yes\n");
    lcd.print("12345678901234567890123456789012345678");
    wait_ms(1000);
    lcd.print('9');
    wait_ms(1000);
    lcd.print('0');
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("\n\nscroll up? no\n");
    lcd.print("12345678901234567890123456789012345678");
    wait_ms(1000);
    lcd.print('9');
    wait_ms(1000);
    lcd.print('0');
    wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    wait_ms(1000);
    lcd.print("\nthree");
    wait_ms(1000);
    lcd.print("\nfour");
    wait_ms(1000);
    lcd.print("\nfive");
    wait_ms(1000);

// -----
    const char menu[] = "The First option, is a very very long sentence\n"
                        "Second option\n"
                        "Third";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(2000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(2000);
    lcd.wrap();

// -----
    constexpr const char wrap_msg[] = "1.first line\n2.second line";
    lcd.clear();
    lcd.print("print_line_nowrap");
    wait_ms(1000);
    lcd.clear();
    lcd.print("Result: '1.firs'?");
    lcd.cursor_pos(0, 1);
    const char* p = lcd.print_line_nowrap(wrap_msg, 6);
    lcd.cursor_pos(0, 2);
    lcd.print("Res:'2.second line'?");
    lcd.cursor_pos(0, 3);
    p = lcd.print_line_nowrap(p, 20);
    wait_ms(3000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	wait_ms(50);
	++p;
    }
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    wait_ms(500);
    lcd.cursor_pos(lcd.cols()-1,0);
    lcd.print('X');
    wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en la esquina
    lcd.cursor_pos(lcd.cols()-1,lcd.rows()-1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    wait_ms(1000);
    lcd.display_off();
    wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    wait_ms(1000);

// -----
    lcd.clear();
    //lcd.print("cursor on");
    rprint(lcd, "cursor on");
    lcd.cursor_on();
    lcd.cursor_pos(9, 0);
    wait_ms(1000);
    lcd.cursor_pos(9, 1);
    wait_ms(1000);
    lcd.cursor_pos(9, 2);
    wait_ms(1000);
    lcd.cursor_pos(9, 3);
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    wait_ms(1000);


}

void test_lcd_screen4_basico()
{
    LCD lcd;

    lcd.clear();
    lcd.print("Hola");
    wait_ms(1000);
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
	// test_lcd_screen4_basico();
	test_lcd_screen4();
    }
}



