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

#include "../../../dev_LCD_screen.h"// TODO: siempre ponerlo al principio
#include <dev_HD44780_generic.h>

#include "../../../dev_LCD_font_2x1.h"
#include "../../../dev_LCD_font_2x2.h"
#include "../../../dev_LCD_font_2x3.h"
#include "../../../dev_LCD_font_3x3.h"
#include "../../../dev_LCD_font_4x3.h"

#include <avr_atmega.h>
#include <stddef.h>

namespace myu = atmega;

// Conexiones
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

// Screens
using Screen_1602 = dev::LCD_screen_1602<LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<LCD_2004>;
using Screen_4004 = dev::LCD_screen_4004<LCD_4004>;

// Choose Screen to test
// TODO: copiar las pruebas del 2004 al 1602 y 4004
//using Screen = Screen_1602;
using Screen = Screen_2004;
//using Screen = Screen_4004;

// Fonts
using Font_digit_2x1_t1 = dev::Font_digit_2x1_t1; 
using Font_digit_2x1_t2 = dev::Font_digit_2x1_t2; 
using Font_digit_2x2_t1 = dev::Font_digit_2x2_t1; 
using Font_digit_2x3_t1 = dev::Font_digit_2x3_t1; 
using Font_digit_3x3_t1 = dev::Font_digit_3x3_t1; 
using Font_digit_4x3_t1 = dev::Font_digit_4x3_t1; 

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
void rprint(Screen& scr, const char* msg)
{
    for (uint8_t i = 0; i < scr.rows(); ++i){
	scr.cursor_pos(0, i);
	scr.print(msg);
    }
    

}

const char long_msg[] = "En un lugar de La Mancha, de cuyo nombre"
                        "no quiero acordarme, no ha mucho tiempo "
                        "que vivia un hidalgo de los de lanza en "
                        "astillero, adarga antigua, rocin flaco y"
			"galgo corredor. Don Quijote (Cervantes)";

void test_extended_chars(Screen& scr)
{
    scr.new_extended_char(0, bell);
    scr.new_extended_char(1, heart_full);
    scr.new_extended_char(2, arrow_up);
    scr.new_extended_char(3, arrow_down);

    scr.clear();
    scr.print("A bell: [");
    scr.print_extended(0);
    scr.print("]\n");

    scr.print("A heart: [");
    scr.print_extended(1);
    scr.print("]\n");

    scr.print("Arrows: [");
    scr.print_extended(2);
    scr.print_extended(3);
    scr.print("]\n");

    myu::Micro::wait_ms(1000);


}

void test_lcd_screen4_1602()
{
    Screen scr;

// ------
    scr.clear();
    scr.print("LCD Screen (1602)");
    myu::Micro::wait_ms(1000);

// ------
    scr.clear();
    rprint(scr, "clear test");
    myu::Micro::wait_ms(1000);
    scr.clear();
    myu::Micro::wait_ms(1000);


// ------
    scr.clear();
    scr.print("This is written in 2 lines");

    myu::Micro::wait_ms(2000);
    
    
// ------
    scr.clear();
    scr.print("print_return\n");
    scr.print("Am I in 2 row?");
    myu::Micro::wait_ms(2000);

// ------
    scr.clear();
    scr.print("Erase this\n");
    scr.print("But don't this");
    myu::Micro::wait_ms(1000);
    scr.clear_row(0);
    myu::Micro::wait_ms(1000);

// ------
    scr.clear();
    scr.print("No delete this\n");
    scr.print("Erase this line");
    myu::Micro::wait_ms(1000);
    scr.clear_row(1);
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.cursor_pos(3,0);
    scr.print('3');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(6,1);
    scr.print('6');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(15,0);
    scr.print('X');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(0,1);
    scr.print('Y');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(15,1);
    scr.print('Z');
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    scr.display_off();
    myu::Micro::wait_ms(1000);
    scr.display_on();
    scr.clear();
    scr.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    scr.clear();
    scr.print("cursor on");
    scr.cursor_on();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor off");
    scr.cursor_off();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor blink");
    scr.cursor_blink();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor no blink");
    scr.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(scr);
}


void test_lcd_screen4_2004()
{
    Screen scr;
// TODO: ordenar las pruebas

// ------
    scr.clear();
//    scr.print("LCD Screen (20 x 04)");
    scr << "LCD Screen(20 x 04)\n";
    scr << 'a' << 21 << "?\notra?"; // probamos operator<<
    myu::Micro::wait_ms(1000);

// -------
// print
    scr.clear();
    scr.print("print [from, to]"); scr.cursor_pos(0, 1);
    scr.print("xx|xxxxx|xxxxxxxxxxx"); scr.cursor_pos(0, 2);
    scr.print("xx|xxxxxxxx|xxxxxxxx"); scr.cursor_pos(0, 3);
    scr.print("xx|xxx|xxxxxxxxxxxxx");
    scr.print(nm::Row{1}, nm::From{3}, nm::To{7}, "print");
    scr.print(nm::Row{2}, nm::From{3}, nm::To{10}, "print");
    scr.print(nm::Row{3}, nm::From{3}, nm::To{5}, "print");
    myu::Micro::wait_ms(3000);

    scr.clear();
    scr.print("print [from, n]"); scr.cursor_pos(0, 1);
    scr.print("xx|xxxxx|xxxxxxxxxxx"); scr.cursor_pos(0, 2);
    scr.print("xx|xxxxxxxx|xxxxxxxx"); scr.cursor_pos(0, 3);
    scr.print("xx|xxx|xxxxxxxxxxxxx");
    scr.print(nm::Row{1}, nm::From{3}, nm::Size{5}, "print");
    scr.print(nm::Row{2}, nm::From{3}, nm::Size{8}, "print");
    scr.print(nm::Row{3}, nm::From{3}, nm::Size{3}, "print");
    myu::Micro::wait_ms(3000);

// -------
// Probamos diferentes fonts
    scr.clear();
    scr.print("Test print fonts");
    scr.clear();
    scr.print(uint8_t{25}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.load<Font_digit_2x1_t1>();
    scr.print<Font_digit_2x1_t1>(uint8_t{2}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.load<Font_digit_2x2_t1>();
    scr.print<Font_digit_2x2_t1>(uint8_t{123}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.load<Font_digit_2x3_t1>();
    scr.print<Font_digit_2x3_t1>(uint8_t{35}, nm::Width{3});
    scr.print(" =? 035");
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.load<Font_digit_3x3_t1>();
    scr.print<Font_digit_3x3_t1>(uint8_t{74}, nm::Width{3});
    scr.print(" =? 074");
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.load<Font_digit_4x3_t1>();
    scr.print<Font_digit_4x3_t1>(uint8_t{12}, nm::Width{3});
    scr.print(" =? 012");
    myu::Micro::wait_ms(1000);

// -------
// Probamos print(int) de ancho fijo
    scr.clear();
    scr.print("Test print int");
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(u8) 009 =?"
	    "\n     ");
    scr.print(uint8_t{9}, nm::Width{3});
    myu::Micro::wait_ms(1000);


    scr.clear();
    scr.print("(u16) 002 =?"
	    "\n      ");
    // scr.print(uint16_t{2}, 3); <-- tambien funciona, pero es más claro:
    scr.print(uint16_t{2}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(u32) 008 =?"
	   "\n      ");
    scr.print(uint32_t{8}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(u64) 072 =?"
	   "\n      ");
    scr.print(uint64_t{72}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s8) 009 =?"
	    "\n     ");
    scr.print(int8_t{9}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s8) -05 =?"
	    "\n     ");
    scr.print(int8_t{-5}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s16) 002 =?"
	   "\n      ");
    scr.print(int16_t{2}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s16) -02 =?"
	   "\n      ");
    scr.print(int16_t{-2}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(u32) 003 =?"
	   "\n      ");
    scr.print(int32_t{3}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s32) -08 =?"
	   "\n      ");
    scr.print(int32_t{-8}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s64) 072 =?"
	   "\n      ");
    scr.print(int64_t{72}, nm::Width{3});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s64) -05 =?"
	   "\n      ");
    scr.print(int64_t{-5}, nm::Width{3});
    myu::Micro::wait_ms(1000);

// -------
    scr.clear();
    scr.print("Test print int\n");
    scr.print("(u16) 61234 =?"
	   "\n      ");
    scr.print(uint16_t{61234});
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("(u32) 123456 =?"
	   "\n      ");
    scr.print(uint32_t{123456});
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("(u64) 123456789 =?"
	   "\n      ");
    scr.print(uint64_t{123456789});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s16) -31234 =?"
	   "\n      ");
    scr.print(int16_t{-31234});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s16) 31234 =?"
	   "\n      ");
    scr.print(int16_t{31234});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s32) -23456 =?"
	   "\n      ");
    scr.print(int32_t{-23456});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s32) 23456 =?"
	   "\n      ");
    scr.print(int32_t{23456});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s64) -123456789 =?"
	   "\n      ");
    scr.print(int64_t{-123456789});
    myu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("(s64) 123456789 =?"
	   "\n      ");
    scr.print(int64_t{123456789});
    myu::Micro::wait_ms(1000);

// -----
    scr.clear();
    scr.print("wrap sentences?\n");
    scr.print("too long: this is a very long sentence");
    myu::Micro::wait_ms(1000);

// -----
// El '\n' del primer print no lo lee, con lo que deja el cursor
// en (cols(), 0). Cualquier intento de escribir va a fallar (print devuelve
// 0)
    scr.clear();
    scr.print("I can't write 5 rows\n");
    scr.print("2 row\n"); 
    scr.print("3 row\n");
    scr.print("4 row\n");
    scr.print("5 row\n");
    myu::Micro::wait_ms(2000);
 
// En la última fila aparecerá 5 row en vez de 4 row, ya que el '\n' de 4 row
// devuelve el cursor al principio de la fila.
    scr.clear();
    scr.print("Second try:\n");
    scr.print("2 row\n");
    scr.print("3 row\n");
    scr.print("4 row\n");
    scr.print("5 row\n");
    myu::Micro::wait_ms(2000);

// ------
    scr.clear();
    rprint(scr, "clear test");
    myu::Micro::wait_ms(1000);
    scr.clear();
    myu::Micro::wait_ms(1000);


// ------
    scr.clear();
    scr.print("print_return\n");
    scr.print("Am I in 2 row?\n");
    scr.print("in 3 row?\n");
    scr.print("in 4 row?");
    myu::Micro::wait_ms(2000);

// ------
    scr.clear();
    scr.print("Erase this\n");
    scr.print("But don't this\n");
    scr.print("Erase this\n");
    scr.print("But don't this");
    myu::Micro::wait_ms(1000);
    scr.clear_row(0);
    scr.clear_row(2);
    myu::Micro::wait_ms(1000);

// ------
    scr.clear();
    scr.print("Don't delete this\n");
    scr.print("Erase this line\n");
    scr.print("Don't delete this\n");
    scr.print("Erase this line");
    myu::Micro::wait_ms(1000);
    scr.clear_row(1);
    scr.clear_row(3);
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.cursor_pos(3,0);
    scr.print('3');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(6,1);
    scr.print('6');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(scr.cols()-1,0);
    scr.print('X');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(0,1);
    scr.print('Y');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(scr.cols()-1,scr.rows()-1);
    scr.print('Z');
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    scr.display_off();
    myu::Micro::wait_ms(1000);
    scr.display_on();
    scr.clear();
    scr.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    scr.clear();
    scr.print("cursor on");
    scr.cursor_on();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor off");
    scr.cursor_off();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor blink");
    scr.cursor_blink();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor no blink");
    scr.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(scr);
}


void test_lcd_screen4_4004()
{
    Screen scr;

// ------
    scr.clear();
    scr.print("LCD screen (40 x 04)");
    myu::Micro::wait_ms(1000);


// ------
    scr.clear();
    rprint(scr, "clear test");
    myu::Micro::wait_ms(1000);
    scr.clear();
    myu::Micro::wait_ms(1000);


// ------
    scr.clear();
    scr.print("print_return\n");
    scr.print("Am I in 2 row?\n");
    scr.print("in 3 row?\n");
    scr.print("in 4 row?");
    myu::Micro::wait_ms(2000);

// ------
    scr.clear();
    scr.print("Erase this\n");
    scr.print("But don't this\n");
    scr.print("Erase this\n");
    scr.print("But don't this");
    myu::Micro::wait_ms(1000);
    scr.clear_row(0);
    scr.clear_row(2);
    myu::Micro::wait_ms(1000);

// ------
    scr.clear();
    scr.print("Don't delete this\n");
    scr.print("Erase this line\n");
    scr.print("Don't delete this\n");
    scr.print("Erase this line");
    myu::Micro::wait_ms(1000);
    scr.clear_row(1);
    scr.clear_row(3);
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("cursor_pos");
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.cursor_pos(3,0);
    scr.print('3');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(6,1);
    scr.print('6');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(scr.cols()-1,0);
    scr.print('X');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(0,1);
    scr.print('Y');
    myu::Micro::wait_ms(500);
    scr.cursor_pos(scr.cols()-1,scr.rows()-1);
    scr.print('Z');
    myu::Micro::wait_ms(1000);


// -----
    scr.clear();
    scr.print("Turn off for 1 second");
    myu::Micro::wait_ms(1000);
    scr.display_off();
    myu::Micro::wait_ms(1000);
    scr.display_on();
    scr.clear();
    scr.print("On?");
    myu::Micro::wait_ms(1000);

// -----
    scr.clear();
    //scr.print("cursor on");
    rprint(scr, "cursor on");
    scr.cursor_on();
    scr.cursor_pos(9, 0);
    myu::Micro::wait_ms(1000);
    scr.cursor_pos(9, 1);
    myu::Micro::wait_ms(1000);
    scr.cursor_pos(9, 2);
    myu::Micro::wait_ms(1000);
    scr.cursor_pos(9, 3);
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor off");
    scr.cursor_off();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor blink");
    scr.cursor_blink();
    myu::Micro::wait_ms(1000);
    scr.clear();
    scr.print("cursor no blink");
    scr.cursor_no_blink();
    myu::Micro::wait_ms(1000);

    test_extended_chars(scr);
}

void test_lcd_screen4_basico()
{
    Screen scr;

    scr.clear();
    scr.print("Hola");
    myu::Micro::wait_ms(1000);
}


void test_lcd_screen4()
{
    switch(Screen::cols()){
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



