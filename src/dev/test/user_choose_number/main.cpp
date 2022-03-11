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

// Conectar el LCD y 3 pulsadores a los pines indicados
#include "../../user_choose_number.h"
#include "../../dev_LCD_HD44780.h"
#include "../../dev_LCD_screen.h"
#include <atd_decimal.h>
#include <atd_magnitude.h>
#include <atd_eng_magnitude.h>
#include <avr_types.h>

#include <avr_time.h>



// pines que usamos
// ----------------
// LCD
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

// keyrow
// pines a los que conectamos el teclado
using Keyrow_pins  = dev::Keyrow_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyrow_codes  = dev::Keyrow_codes<OK_KEY, UP_KEY, DOWN_KEY>;



// dispositivos que conectamos
// ---------------------------
using Keyrow = dev::Basic_keyrow<Keyrow_pins, Keyrow_codes>;

// Dispositivos reales conectados
using LCD_1602 = dev::LCD_HD44780_1602<LCD_pins>;
using LCD_2004 = dev::LCD_HD44780_2004<LCD_pins>;

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;

// Screens
using Screen_1602 = dev::LCD_screen_1602<Generic_LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<Generic_LCD_2004>;


// ostreams
//using LCD_ostream_1602 = dev::LCD_ostream_1602<Generic_LCD_1602>;
//using LCD_ostream_2004 = dev::LCD_ostream_2004<Generic_LCD_2004>;

// Choose LCD to test
using LCD = Screen_1602;
//using LCD = LCD_ostream_1602;
// using LCD = LCD_ostream_2004;

struct Main{

    LCD scr;

    void f(uint16_t x)
    {
	scr.cursor_pos(10, 1);
	scr.print('(');
	scr.print(x);
	if (x % 2)
	    scr.print('i');
	else
	    scr.print('p');

	scr.print(')');
    }
};
 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_choose_number2()
{
    Main app;

    LCD& scr = app.scr;

    Keyrow keyrow;

    scr.clear();
    scr.print("Choose number");
    wait_ms(1000);

    scr.clear();
    scr.print("LINEAL TEST");
    wait_ms(1000);

{
    scr.clear();
    scr.print("Big number");
    wait_ms(1000);
    uint16_t u16  =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					     .between(0, 60000)
					     .choose4(2900u);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    wait_ms(1000);

}

//

    scr.clear();
    scr.print("2 digits [5,25]");
    wait_ms(1000);
    uint8_t u8 =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    scr.cursor_pos(0,0);
    scr.print("has elegido: ");
    scr.print_number(u8);


    wait_ms(1000);

    scr.clear();
    scr.print("4 [2890, 2910]");
    uint16_t u16  =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    wait_ms(1000);

    scr.clear();
    scr.print("CIRCULAR TEST");
    wait_ms(1000);

    scr.clear();
    scr.print("2 digits [5,25]");
    wait_ms(1000);
    u8 =  dev::user_choose_number_circular(scr, keyrow).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    scr.cursor_pos(0,0);
    scr.print("has elegido: ");
    scr.print_number(u8);

    wait_ms(1000);

    scr.clear();
    scr.print("4 [2890, 2910]");
    u16  =  dev::user_choose_number_circular(scr, keyrow).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    wait_ms(1000);

{
    scr.clear();
    scr.print("Callback test");
    wait_ms(1000);
    uint16_t u16  =  dev::user_choose_number_lineal(app, scr, keyrow).pos(3, 1)
					     .between(10, 30)
					     .callback(&Main::f)
					     .choose4(20);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    wait_ms(1000);

}


{
using namespace avr::literals;
    scr.clear();
    scr.print("Freq. [5,25]");
    scr.print("\nTODO!!!");
//    avr::Frequency u8 =
//        dev::user_choose_number_lineal<LCD, Keyrow, avr::Frequency>
//	     (scr, keyrow)
//            .pos(3, 1)
//            .between(5_Hz, 25_Hz)
//            .choose2(10_Hz);
//
//    scr.cursor_pos(0,0);
//    scr.print("has elegido: ");
//    u8.print(scr); <-- ¿cómo implementar esto?

    wait_ms(1000);
}
{
//using Rep = atd::Decimal<uint16_t, 2>; TODO
    scr.clear();
    scr.print("Decimal [5,25]");
    scr.print("\nTODO!!!");
    wait_ms(1000);
//    Rep u8 = dev::user_choose_number_lineal<LCD, Keyrow, Rep>(scr, keyrow)
//		     .pos(3, 1)
//		     .between(5, 25)
//		     .choose2(10);
//
//    scr.cursor_pos(0,0);
//    scr.print("has elegido: ");
//    scr.print_number(u8); <-- ¿cómo implementar esto?
//
//    wait_ms(1000);
}

}



int main()
{
    while(1){
	test_choose_number2();
    }
}



