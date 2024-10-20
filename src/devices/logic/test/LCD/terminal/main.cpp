// Copyright (C) 2019-2022 Manuel Perez 
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

#include "../../../dev_LCD_terminal.h"
#include <dev_HD44780_generic.h>
#include <avr_atmega.h>
#include <stddef.h>

namespace myu = atmega;

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
using LCD_1602 = dev::LCD_HD44780_1602<myu::Micro, LCD_pins_1602>;
using LCD_2004 = dev::LCD_HD44780_2004<myu::Micro, LCD_pins_2004>;
using LCD_4004 = dev::LCD_HD44780_4004<myu::Micro, LCD_pins_4004>;

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;
using Generic_LCD_4004 = dev::Generic_LCD<LCD_4004>;

// Terminals
using Terminal_1602 = dev::LCD_terminal_1602<Generic_LCD_1602>;
using Terminal_2004 = dev::LCD_terminal_2004<Generic_LCD_2004>;
using Terminal_4004 = dev::LCD_terminal_4004<Generic_LCD_4004>;

// Choose Terminal to test
//using Terminal = Terminal_1602;
using Terminal = Terminal_2004;
//using Terminal = Terminal_4004;



// glyph de una campana
constexpr const uint8_t bell[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0001110,
			    0b0001110,
			    0b0011111,
			    0b0000100,
			    0b0000000 };

constexpr const uint8_t heart_full[8] = 
			  { 0b0000000,
			    0b0001010,
			    0b0011111,
			    0b0011111,
			    0b0001110,
			    0b0000100,
			    0b0000000,
			    0b0000000};

// Probar a cambiar la definición y ver cómo varia el tamaño del programa.
// Curiosamente, al usar PROGMEM aumenta el tamaño del programa. Es culpa
// (creo, y de acuerdo con el manual de avr-libc) de llamar a las funciones
// pgm_read_byte que ocupan código. 
constexpr const atd::ROM_array<uint8_t, 8, myu::ROM_read> arrow_up PROGMEM = 
//constexpr const char arrow_up[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0001110,
			    0b0010101,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0000000 };


constexpr const atd::ROM_array<uint8_t, 8, myu::ROM_read> arrow_down PROGMEM = 
//constexpr const char arrow_down[8] = 
			  { 0b0000000,
			    0b0000100,
			    0b0000100,
			    0b0000100,
			    0b0010101,
			    0b0001110,
			    0b0000100,
			    0b0000000};


// repeat print
void rprint(Terminal& lcd, const char* msg)
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

void test_extended_chars(Terminal& lcd)
{
    lcd.new_extended_char(0, bell);
    lcd.new_extended_char(1, heart_full);
    lcd.new_extended_char(2, arrow_up);
    lcd.new_extended_char(3, arrow_down);

    lcd.clear();
    lcd.print("A bell: [");
    lcd.print_extended(0);
    lcd.print("]\n");

    lcd.print("A heart: [");
    lcd.print_extended(1);
    lcd.print("]\n");

    lcd.print("Arrows: [");
    lcd.print_extended(2);
    lcd.print_extended(3);
    lcd.print("]\n");

    myu::Micro::wait_ms(1000);


}

void test_lcd_terminal4_1602()
{
    Terminal lcd;

// ------
    lcd.clear();
    lcd.print("Terminal terminal(1602)");
    myu::Micro::wait_ms(1000);

// ------
    {
    lcd.clear();
    lcd.cursor_pos(15,0);
    const char* n = "Align";
    lcd.print_align_to_the_right(n, n+5);
    lcd.cursor_pos(4,1);
    lcd.print_align_to_the_right(n, n+5);
    myu::Micro::wait_ms(1000);
    }

    lcd.clear();
    lcd.print("Test align");
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("uint16_t: [123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(uint16_t{123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("int16_t: [-123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(int16_t{-123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("uint32_t: [123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(uint32_t{123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("int32_t: [-123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(int32_t{-123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("uint64_t: [123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(uint64_t{123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("int64_t: [-123]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right(int64_t{-123});
    myu::Micro::wait_ms(1000);

    lcd.clear();
    lcd.print("double: [23.47]");
    lcd.cursor_pos(15, 1);
    lcd.print_align_to_the_right<2>(double{23.47});
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    rprint(lcd, "clear test");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    myu::Micro::wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This is written in 2 lines");

    myu::Micro::wait_ms(2000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?");
    myu::Micro::wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(0);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("No delete this\n");
    lcd.print("Erase this line");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(1);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("scroll up? yes\n");
    lcd.print("12345678901234");
    myu::Micro::wait_ms(1000);
    lcd.print('5');
    myu::Micro::wait_ms(1000);
    lcd.print('6');
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("scroll up? no\n");
    lcd.print("12345678901234");
    myu::Micro::wait_ms(1000);
    lcd.print('5');
    myu::Micro::wait_ms(1000);
    lcd.print('6');
    myu::Micro::wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    myu::Micro::wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    myu::Micro::wait_ms(1000);
    lcd.print("\nthree");
    myu::Micro::wait_ms(1000);
    lcd.print("\nfour");
    myu::Micro::wait_ms(1000);

// -----
    const char menu[] = "First option, is very long";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.print("print4chars(1)");
    lcd.cursor_pos(4, 1);
    const char* p = lcd.print_line_nowrap("1.one\n2.two", 4);
    myu::Micro::wait_ms(1000);

// -----
    lcd.cursor_pos(0,0);
    lcd.print("print4chars(2)");
    lcd.cursor_pos(4, 1);
    p = lcd.print_line_nowrap(p, 4);
    myu::Micro::wait_ms(1000);


// -----
    lcd.cursor_pos(0,0);
    lcd.print("print4chars'1'");
    lcd.cursor_pos(4, 1);
    lcd.print_line_nowrap("1", 4);
    myu::Micro::wait_ms(1000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	myu::Micro::wait_ms(40);
	++p;
    }
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(15,0);
    lcd.print('X');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    myu::Micro::wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en (15,1)
    lcd.cursor_pos(15,1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    lcd.display_off();
    myu::Micro::wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    lcd.clear();
    lcd.print("cursor on");
    lcd.cursor_on();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(lcd);
}


void test_lcd_terminal4_2004()
{
    Terminal lcd;

// ------
    lcd.clear();
    lcd.print("Terminal terminal (20 x 04)");
    myu::Micro::wait_ms(1000);


// ------
    lcd.clear();
    rprint(lcd, "clear test");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    myu::Micro::wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This sentence has to"
              "be written in 4 line"
              "s. If not, it means "
              "that print is wrong");
    myu::Micro::wait_ms(2000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?\n");
    lcd.print("in 3 row?\n");
    lcd.print("in 4 row?");
    myu::Micro::wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this\n");
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(0);
    lcd.clear_row(2);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line\n");
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(1);
    lcd.clear_row(3);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("\n\nscroll up? yes\n");
    lcd.print("123456789012345678");
    myu::Micro::wait_ms(1000);
    lcd.print('9');
    myu::Micro::wait_ms(1000);
    lcd.print('0');
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("\n\nscroll up? no\n");
    lcd.print("123456789012345678");
    myu::Micro::wait_ms(1000);
    lcd.print('9');
    myu::Micro::wait_ms(1000);
    lcd.print('0');
    myu::Micro::wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    myu::Micro::wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    myu::Micro::wait_ms(1000);
    lcd.print("\nthree");
    myu::Micro::wait_ms(1000);
    lcd.print("\nfour");
    myu::Micro::wait_ms(1000);
    lcd.print("\nfive");
    myu::Micro::wait_ms(1000);

// -----
    const char menu[] = "First option, is very long\n"
                        "Second option\n"
                        "Third";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(3000);
    lcd.wrap();

// -----
    constexpr const char wrap_msg[] = "1.first line\n2.second line";
    lcd.clear();
    lcd.print("print_line_nowrap");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("Result: '1.firs'?");
    lcd.cursor_pos(0, 1);
    const char* p = lcd.print_line_nowrap(wrap_msg, 6);
    lcd.cursor_pos(0, 2);
    lcd.print("Res:'2.second line'?");
    lcd.cursor_pos(0, 3);
    p = lcd.print_line_nowrap(p, 20);
    myu::Micro::wait_ms(3000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	myu::Micro::wait_ms(50);
	++p;
    }
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(lcd.cols()-1,0);
    lcd.print('X');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    myu::Micro::wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en la esquina
    lcd.cursor_pos(lcd.cols()-1,lcd.rows()-1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    lcd.display_off();
    myu::Micro::wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    lcd.clear();
    lcd.print("cursor on");
    lcd.cursor_on();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(lcd);
}


void test_lcd_terminal4_4004()
{
    Terminal lcd;

// ------
    lcd.clear();
    lcd.print("Terminal terminal (40 x 04)");
    myu::Micro::wait_ms(1000);


// ------
    lcd.clear();
    rprint(lcd, "clear test");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    myu::Micro::wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("This sentence has to be written in a dis"
              "play of 40 columns by 4 rows. It is long"
              "because I want to test the print functio"
              "n. Is it ok? ------------------ the end");

    myu::Micro::wait_ms(5000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("Am I in 2 row?\n");
    lcd.print("in 3 row?\n");
    lcd.print("in 4 row?");
    myu::Micro::wait_ms(2000);

// ------
    lcd.clear();
    lcd.print("Erase this\n");
    lcd.print("But don't this\n");
    lcd.print("Erase this\n");
    lcd.print("But don't this");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(0);
    lcd.clear_row(2);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line\n");
    lcd.print("Don't delete this\n");
    lcd.print("Erase this line");
    myu::Micro::wait_ms(1000);
    lcd.clear_row(1);
    lcd.clear_row(3);
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("\n\nscroll up? yes\n");
    lcd.print("12345678901234567890123456789012345678");
    myu::Micro::wait_ms(1000);
    lcd.print('9');
    myu::Micro::wait_ms(1000);
    lcd.print('0');
    myu::Micro::wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(true);
    lcd.print("\n\nscroll up? no\n");
    lcd.print("12345678901234567890123456789012345678");
    myu::Micro::wait_ms(1000);
    lcd.print('9');
    myu::Micro::wait_ms(1000);
    lcd.print('0');
    myu::Micro::wait_ms(1000);
    lcd.stop_brcorner(false);

// ------
    lcd.clear();
    lcd.print("Testing scroll");
    lcd.print("\none");
    myu::Micro::wait_ms(1000);
    lcd.print('\n');
    lcd.print("two");
    myu::Micro::wait_ms(1000);
    lcd.print("\nthree");
    myu::Micro::wait_ms(1000);
    lcd.print("\nfour");
    myu::Micro::wait_ms(1000);
    lcd.print("\nfive");
    myu::Micro::wait_ms(1000);

// -----
    const char menu[] = "The First option, is a very very long sentence\n"
                        "Second option\n"
                        "Third";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(2000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap true");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    myu::Micro::wait_ms(2000);
    lcd.wrap();

// -----
    constexpr const char wrap_msg[] = "1.first line\n2.second line";
    lcd.clear();
    lcd.print("print_line_nowrap");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("Result: '1.firs'?");
    lcd.cursor_pos(0, 1);
    const char* p = lcd.print_line_nowrap(wrap_msg, 6);
    lcd.cursor_pos(0, 2);
    lcd.print("Res:'2.second line'?");
    lcd.cursor_pos(0, 3);
    p = lcd.print_line_nowrap(p, 20);
    myu::Micro::wait_ms(3000);



// -----
    lcd.clear();
    p = long_msg;
    while (*p){
	lcd.print(*p);
	myu::Micro::wait_ms(50);
	++p;
    }
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.cursor_pos(3,0);
    lcd.print('3');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(6,1);
    lcd.print('6');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(lcd.cols()-1,0);
    lcd.print('X');
    myu::Micro::wait_ms(500);
    lcd.cursor_pos(0,1);
    lcd.print('Y');
    myu::Micro::wait_ms(500);
    lcd.stop_brcorner(true);    // impido que haga el scroll en la esquina
    lcd.cursor_pos(lcd.cols()-1,lcd.rows()-1);
    lcd.print('Z');
    lcd.stop_brcorner(false);
    myu::Micro::wait_ms(1000);


// -----
    lcd.clear();
    lcd.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    lcd.display_off();
    myu::Micro::wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    lcd.clear();
    //lcd.print("cursor on");
    rprint(lcd, "cursor on");
    lcd.cursor_on();
    lcd.cursor_pos(9, 0);
    myu::Micro::wait_ms(1000);
    lcd.cursor_pos(9, 1);
    myu::Micro::wait_ms(1000);
    lcd.cursor_pos(9, 2);
    myu::Micro::wait_ms(1000);
    lcd.cursor_pos(9, 3);
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor off");
    lcd.cursor_off();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor blink");
    lcd.cursor_blink();
    myu::Micro::wait_ms(1000);
    lcd.clear();
    lcd.print("cursor no blink");
    lcd.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(lcd);
}

void test_lcd_terminal4_basico()
{
    Terminal lcd;

    lcd.clear();
    lcd.print("Hola");
    myu::Micro::wait_ms(1000);
}


void test_lcd_terminal4()
{
    switch(Terminal::cols()){
	case 16: test_lcd_terminal4_1602(); break;
	case 20: test_lcd_terminal4_2004(); break;
	case 40: test_lcd_terminal4_4004(); break;

	default: test_lcd_terminal4_1602(); break;
    }
}


int main()
{
    while(1){
	// test_lcd_terminal4_basico();
	test_lcd_terminal4();
    }
}



