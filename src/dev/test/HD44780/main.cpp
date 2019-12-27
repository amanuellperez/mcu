// Copyright (C) 2019-2020 A.Manuel L.Perez
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


#include "../../dev_LCD_HD44780.h"
#include <avr_time.h>
#include <stddef.h>
//#include <avr_USART.h>

using namespace dev;


// Si lo conectamos solo a 8 pins de datos
// LCD_HD44780 lcd{ LCD_HD44780::Pin_RS{10}
//		, LCD_HD44780::Pin_RW{11}
//		, LCD_HD44780::Pin_E {12}
//		    , LCD_HD44780::Pin_D8{2, 3, 4, 5, 6, 7, 8, 9}};


 
void print(LCD_HD44780& lcd, const char* c)
{
    while(*c)
	lcd.write_data_to_CG_or_DDRAM(*c++);
}

// Probamos el LCD conectado a 4 pines de datos 
void test_lcd4()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780 lcd{ LCD_HD44780::DPin_RS{4}
		    , LCD_HD44780::DPin_RW{5}
		    , LCD_HD44780::DPin_E {6}
		    , LCD_HD44780::DPin_D4{11, 12, 13, 14}};

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
	lcd.set_ddram_address_2row();
	print(lcd, "Segunda fila");
	lcd.set_ddram_address_1row();
	print(lcd, "Primera fila");
	wait_ms(1000);

	lcd.clear_display();

    }
}


// Probamos el LCD_screen conectado a 4 pines de datos 
void test_lcd_screen4()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780_1602_screen lcd{ LCD_HD44780::DPin_RS{4}
    // LCD_HD44780_2004_screen lcd{ LCD_HD44780::DPin_RS{4}
		    , LCD_HD44780::DPin_RW{5}
		    , LCD_HD44780::DPin_E {6}
		    , LCD_HD44780::DPin_D4{11, 12, 13, 14}};


    while(1){


        lcd.clear();
	lcd.print("LCD oscreen");
	wait_ms(1000);


	lcd.clear();
	lcd.print("Probando clear");
	wait_ms(1000);
	lcd.clear();
	lcd.print("al principio?");
	wait_ms(1000);


	lcd.clear();
	lcd.print("Esta es una linea muy larga");
	wait_ms(1000);
	
	
	lcd.clear();
	lcd.print("print_return\n");
	lcd.print("En otra linea?");
	wait_ms(1000);
    
	lcd.clear();
	lcd.print("Borro this line\n");
	lcd.print("Esta ahora no");
	wait_ms(1000);
	lcd.clear_row(0);
	wait_ms(1000);

	lcd.clear();
	lcd.print("Esta ahora no\n");
	lcd.print("Borro this line");
	wait_ms(1000);
	lcd.clear_row(1);
	wait_ms(1000);

	lcd.clear();
	lcd.print("scroll up? si\n");
	lcd.print("12345678901234");
	wait_ms(1000);
	lcd.print('5');
	wait_ms(1000);
	lcd.print('6');
	wait_ms(1000);

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

        const char menu[] = "Primera opcion, larga\nSegunda opcion\nTercera\nY "
                            "una cuarta para lcds de 4 filas\nQuinta!!!";
	lcd.clear();
	lcd.nowrap();
	lcd.print("nowrap activo");
	wait_ms(1000);
	lcd.clear();
	lcd.print(menu);
	wait_ms(3000);
	lcd.wrap();

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
                           "acordarme. Creo que paso de escribir todo el Quijote.";
	lcd.clear();
	p = msg;
	while (*p){
	    lcd.print(*p);
	    wait_ms(100);
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
	wait_ms(1000);
    }
}


void test_lcd_ostream4()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780_1602_ostream lcd{ LCD_HD44780::DPin_RS{4}
    //LCD_HD44780_2004_ostream lcd{ LCD_HD44780::DPin_RS{4}
		    , LCD_HD44780::DPin_RW{5}
		    , LCD_HD44780::DPin_E {6}
		    , LCD_HD44780::DPin_D4{11, 12, 13, 14}};


    while(1){
	lcd.clear();
	lcd << "LCD_HD44780_ostream";
	wait_ms(1000);
	char c = 'x';
	lcd << "\nUn caracter (x): [" << c << "]";
	wait_ms(1000);
	lcd << "\nUna frase un poco larga...";
	wait_ms(1500);
	char u8 = 'c';
	lcd << "\nUn uint8_t(c): [" << u8 << "]";
	wait_ms(1000);
	int8_t i8 = -100;
	lcd << "\nUn int8_t (-100): [" << i8 << "]";
	wait_ms(1000);
	uint16_t u16 = 65500;
	lcd << "\nUn uint16_t(65500): [" << u16 << "]";
	wait_ms(1000);
	int16_t i16 = -10000;
	lcd << "\nUn int16_t (-10000): [" << i16 << "]";
	wait_ms(1000);
	uint32_t u32 = 1000000;
	lcd << "\nUn uint32_t(1000000): [" << u32 << "]";
	wait_ms(1000);
    }
}
void test_lcd_screen4_basico()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780_1602_screen lcd{ LCD_HD44780::DPin_RS{4}
		    , LCD_HD44780::DPin_RW{5}
		    , LCD_HD44780::DPin_E {6}
		    , LCD_HD44780::DPin_D4{11, 12, 13, 14}};


    while(1){
	lcd.clear();
	lcd.print("Hola");
	wait_ms(1000);
    }
}




int main()
{
//    UART_iostream uart;
//    cfg_basica(uart);
//    uart.on();

    // test_lcd4();
    // test_lcd_screen4_basico();

    test_lcd_screen4();
    // test_lcd_ostream4();
}



