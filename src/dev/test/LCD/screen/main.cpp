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

//using LCD = LCD_HD44780_1602_screen;
//using LCD = LCD_HD44780_2004_screen;
using LCD = LCD_HD44780_4004_screen;


void test_lcd_screen4()
{
    LCD lcd;

// ------
    lcd.clear();
    lcd.print("LCD screen");
    wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("Probando clear");
    wait_ms(1000);
    lcd.clear();
    lcd.print("al principio?");
    wait_ms(1000);


// ------
    lcd.clear();
    lcd.print("Esta es una linea muy larga, para que entre en los LCD 40 x 04.");
    wait_ms(2000);
    
    
// ------
    lcd.clear();
    lcd.print("print_return\n");
    lcd.print("En otra linea?");
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Borro this line\n");
    lcd.print("Esta ahora no");
    wait_ms(1000);
    lcd.clear_row(0);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.print("Esta ahora no\n");
    lcd.print("Borro this line");
    wait_ms(1000);
    lcd.clear_row(1);
    wait_ms(1000);

// ------
    lcd.clear();
    lcd.stop_brcorner(false);
    lcd.print("scroll up? si\n");
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
    lcd.print("Probando scroll");
    lcd.print("\nuno");
    wait_ms(1000);
    lcd.print('\n');
    lcd.print("dos");
    wait_ms(1000);
    lcd.print("\ntres");
    wait_ms(1000);
    lcd.print("\ncuatro");
    wait_ms(1000);

// -----
    const char menu[] = "Primera opcion, esta si que es una opcion larga\nSegunda opcion\nTercera\nY "
			"una cuarta para lcds de 4 filas\nQuinta!!!";
    lcd.clear();
    lcd.nowrap();
    lcd.print("nowrap activo");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.wrap();
    lcd.print("wrap activo");
    wait_ms(1000);
    lcd.clear();
    lcd.print(menu);
    wait_ms(3000);
    lcd.wrap();

// -----
    lcd.clear();
    lcd.print("print4chars(1)");
    lcd.cursor_pos(4, 1);
    const char* p = lcd.print_line_nowrap("1.una\n2.dos", 4);
    wait_ms(1000);

    lcd.cursor_pos(0,0);
    lcd.print("print4chars(2)");
    lcd.cursor_pos(4, 1);
    p = lcd.print_line_nowrap(p, 4);
    wait_ms(1000);

    lcd.cursor_pos(0,0);
    lcd.print("print4chars'1'");
    lcd.cursor_pos(4, 1);
    lcd.print_line_nowrap("1", 4);
    wait_ms(1000);



    const char msg[] = "En un lugar de la mancha, de cuyo nombre no quiero "
		       "acordarme. Creo que paso de escribir todo el Quijote."
		       "Aunque necesito escribir mas si quiero ver como se "
		       "comporta el LCD de 40 x 04.";
    lcd.clear();
    p = msg;
    while (*p){
	lcd.print(*p);
	wait_ms(50);
	++p;
    }
    wait_ms(1000);


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


    lcd.clear();
    lcd.print("Apago por 1 segundo");
    wait_ms(1000);
    lcd.display_off();
    wait_ms(1000);
    lcd.display_on();
    lcd.clear();
    lcd.print("Encendido?");
    wait_ms(1000);

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

    lcd.clear();
    lcd.print("Pruebo print");
    wait_ms(2000);
}



void test_lcd_screen4_basico()
{
    LCD lcd;

    lcd.clear();
    lcd.print("Hola");
    wait_ms(1000);
}



int main()
{
    while(1){
	// test_lcd_screen4_basico();
	test_lcd_screen4();
    }
}



