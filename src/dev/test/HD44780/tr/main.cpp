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


#include "../../../dev_LCD_HD44780_tr.h"
#include <avr_time.h>


// Si lo conectamos solo a 4 pins de datos
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;


void print(LCD_HD44780& lcd, const char* c)
{
    while(*c)
	lcd.write_data_to_CG_or_DDRAM(*c++);
}

// Probamos el LCD conectado a 4 pines de datos 
void test_lcd4()
{
    LCD_HD44780 lcd;

    const char shift_izda[] = "shift izda";
    const char shift_dcha[] = "shift dcha";

    while(1){
	// Probamos que funcionen las funciones básicas de impresión
	// Si no el resto no funcionará.
	lcd.write_data_to_CG_or_DDRAM('O');
	lcd.write_data_to_CG_or_DDRAM('K');
	lcd.write_data_to_CG_or_DDRAM('?');
	wait_ms(1000);

	lcd.clear_display();
	lcd.return_home();
	print(lcd, "copiando esta fi");
	for (uint8_t x = 0; x < 16; ++x){
	    lcd.set_ddram_address(0x00 + x);
	    uint8_t c = lcd.read_data_from_CG_or_DDRAM();
	    lcd.set_ddram_address(0x40 + x);
	    lcd.write_data_to_CG_or_DDRAM(c);
	    wait_ms(100);
	}
	wait_ms(1000);


	lcd.clear_display();
	print(lcd, "Probando clear");
	wait_ms(1000);

	lcd.clear_display();
	wait_ms(1000);

	print(lcd, "Pruebo return_home");
	wait_ms(1000);
	lcd.return_home();
	print(lcd, "VUELVO!!!");
	wait_ms(1500);

	lcd.clear_display();
	print(lcd, "Shift izda:");
	lcd.entry_mode(true, true);
	wait_ms(500);
	const char* p = &shift_izda[0];
	while (*p){
	    lcd.write_data_to_CG_or_DDRAM(*p);
	    wait_ms(500);
	    ++p;
	}
	wait_ms(1000);

	lcd.entry_mode(true, false);	// recordar dejar shift = false
	lcd.clear_display();
	print(lcd, "Shift dcha");
	wait_ms(1000);
	lcd.entry_mode(false, true);
	wait_ms(500);
	p = &shift_dcha[0];
	while (*p){
	    lcd.write_data_to_CG_or_DDRAM(*p);
	    wait_ms(500);
	    ++p;
	}
	wait_ms(1000);
	lcd.entry_mode(true, false);	// recordar dejar shift = false

	lcd.clear_display();
	print(lcd, "Apago por 1 segundo");
	lcd.display_control(true, false, false);
	wait_ms(1000);
	lcd.display_control(false, false, false);
	wait_ms(1000);
	lcd.display_control(true, false, false);
	lcd.clear_display();
	print(lcd, "Encendido?");
	wait_ms(1000);

	lcd.clear_display();
	print(lcd, "Cursor on");
	lcd.display_control(true, true, false);
	wait_ms(1000);
	lcd.clear_display();
	print(lcd, "Cursor off");
	lcd.display_control(true, false, false);
	wait_ms(2000);
	lcd.clear_display();
	print(lcd, "Cursor blink on");
	lcd.display_control(true, false, true);
	wait_ms(3000);
	lcd.clear_display();
	print(lcd, "Cursor blink off");
	lcd.display_control(true, false, false);
	wait_ms(1000);
	// Lo dejamos sin cursor
	lcd.display_control(true, false, false);


	lcd.display_control(true, true, false);
	lcd.clear_display();
	print(lcd, "Esta fila hace un shift a la izda");
	for (uint8_t i = 0; i < 40; ++i){
	    lcd.cursor_or_display_shift(true, false);
	    wait_ms(200);
	}
	// Lo dejamos como estaba
	lcd.return_home();

	lcd.clear_display();
	print(lcd, "Esta fila hace un shift a la dcha");
	for (uint8_t i = 0; i < 40; ++i){
	    lcd.cursor_or_display_shift(true, true);
	    wait_ms(200);
	}
	// Lo dejamos como estaba
	lcd.return_home();

	lcd.clear_display();
	print(lcd, "Move cursor izda");
	for (uint8_t i = 0; i < 15; ++i){
	    lcd.cursor_or_display_shift(false, false);
	    wait_ms(300);
	}

	lcd.clear_display();
	print(lcd, "Move cursor dcha");
	lcd.set_ddram_address(0); // coloco el cursor al principio
	for (uint8_t i = 0; i < 15; ++i){
	    lcd.cursor_or_display_shift(false, true);
	    wait_ms(300);
	}



	lcd.clear_display();
	lcd.set_ddram_address(3);
	print(lcd, "a partir 3");
	wait_ms(1000);

	lcd.clear_display();
	lcd.set_ddram_address(0x00);
	print(lcd, "Segunda fila");
	lcd.set_ddram_address(0x40);
	print(lcd, "Primera fila");
	wait_ms(1000);

	lcd.clear_display();

    }
}


int main()
{
   test_lcd4();
}



