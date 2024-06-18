// Copyright (C) 2019-2020 Manuel Perez 
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

#include <dev_LCD_screen.h>  // TODO: hay que ponerlo primero, antes que 
				// atd::decimal. print(screen, decimal)
				// depende de este header.
#include <dev_HD44780.h>
#include <atd_decimal.h>
#include <atd_magnitude.h>
#include <avr_types.h>

#include <avr_time.h>
#include <dev_LCD_font_2x1.h>
#include <dev_LCD_font_2x2.h>
#include <dev_LCD_font_2x3.h>
#include <dev_LCD_font_3x3.h>
#include <dev_LCD_font_4x3.h>
#include "../../user_choose_number.h"


// Microcontroller
// ---------------
namespace mcu = atmega;
using Micro   = mcu::Micro;

// Pin connections
// ---------------
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
using Keyrow = dev::Basic_keyrow<Micro, Keyrow_pins, Keyrow_codes>;

// Dispositivos reales conectados
using LCD_1602 = dev::LCD_HD44780_1602<LCD_pins>;
using LCD_2004 = dev::LCD_HD44780_2004<LCD_pins>;

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;

// Screens
using Screen_1602 = dev::LCD_screen_1602<Generic_LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<Generic_LCD_2004>;

// Choose LCD to test
//using LCD = Screen_1602;
using LCD = Screen_2004;

// Fonts
using Font_digit_2x1_t1 = dev::Font_digit_2x1_t1; 
using Font_digit_2x1_t2 = dev::Font_digit_2x1_t2; 
using Font_digit_2x2_t1 = dev::Font_digit_2x2_t1; 
using Font_digit_2x3_t1 = dev::Font_digit_2x3_t1; 
using Font_digit_3x3_t1 = dev::Font_digit_3x3_t1; 
using Font_digit_4x3_t1 = dev::Font_digit_4x3_t1; 

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
    mcu::Micro::wait_ms(700);

    scr.clear();
    scr.print("LINEAL TEST");
    mcu::Micro::wait_ms(700);

{
    scr.clear();
    scr.print("Big font: [5, 40]");
    mcu::Micro::wait_ms(700);
    scr.load<Font_digit_3x3_t1>();


    uint8_t u8  =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					     .between(5u, 40u)
					     .choose2<Font_digit_3x3_t1>(8u);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print<Font_digit_3x3_t1>(u8, nm::Width{2});

    mcu::Micro::wait_ms(1000);

}



{
    scr.clear();
    scr.print("Big number");
    mcu::Micro::wait_ms(1000);
    uint16_t u16  =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					     .between(0, 60000)
					     .choose4(2900u);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    mcu::Micro::wait_ms(1000);

}

//

    scr.clear();
    scr.print("2 digits [5,25]");
    mcu::Micro::wait_ms(1000);
    uint8_t u8 =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    scr.cursor_pos(0,0);
    scr.print("has elegido: ");
    scr.print(u8);


    mcu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("4 [2890, 2910]");
    uint16_t u16  =  dev::user_choose_number_lineal(scr, keyrow).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    mcu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("CIRCULAR TEST");
    mcu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("2 digits [5,25]");
    mcu::Micro::wait_ms(1000);
    u8 =  dev::user_choose_number_circular(scr, keyrow).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    scr.cursor_pos(0,0);
    scr.print("has elegido: ");
    scr.print(u8);

    mcu::Micro::wait_ms(1000);

    scr.clear();
    scr.print("4 [2890, 2910]");
    u16  =  dev::user_choose_number_circular(scr, keyrow).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    mcu::Micro::wait_ms(1000);

{
    scr.clear();
    scr.print("Callback test");
    mcu::Micro::wait_ms(1000);
    uint16_t u16  =  dev::user_choose_number_lineal(app, scr, keyrow).pos(3, 1)
					     .between(10, 30)
					     .callback(&Main::f)
					     .choose4(20);
    scr.cursor_pos(0,0);
    scr.print("elegido : ");
    scr.print(u16);

    mcu::Micro::wait_ms(1000);

}


//{TODO: al migrar al compilador avr-g++ 11.3.0 esto dejó de funcionar.Revisar!!
//using namespace avr::literals;
//    scr.clear();
//    scr.print("Freq. [5,25]");
//
//    avr::Frequency u8 =
//        dev::user_choose_number_lineal<LCD, Keyrow, avr::Frequency>
//	     (scr, keyrow)
//            .pos(3, 1)
//            .between(5_Hz, 25_Hz)
//            .choose2(10_Hz);
//
//    scr.cursor_pos(0,0);
//    scr.print("has elegido: ");
//    atd::print(scr, u8);
//
//    mcu::Micro::wait_ms(1000);
//}
//{TODO: al migrar al compilador avr-g++ 11.3.0 esto dejó de funcionar.Revisar!!
//    using Rep = atd::Decimal<uint16_t, 2>;
//    scr.clear();
//    scr.print("Decimal [5,25]");
//    mcu::Micro::wait_ms(1000);
//    Rep u8 = dev::user_choose_number_lineal<LCD, Keyrow, Rep>(scr, keyrow)
//		     .pos(3, 1)
//		     .between(5, 25)
//		     .choose2(10);
//
//    scr.cursor_pos(0,0);
//    scr.print("has elegido: ");
//    atd::print(scr, u8);
//
//    mcu::Micro::wait_ms(1000);
//}

}

void test_bugs()
{
    Main app;

    LCD& scr = app.scr;

    Keyrow keyrow;

    scr.clear();
    scr.print("Bugs");
    mcu::Micro::wait_ms(700);

{
    scr.clear();
    // Bug: Hay que pulsar varias veces para que pase de 12 a 1.
    scr.print("circular 12->1");
    dev::user_choose_number_circular(scr, keyrow).pos(3, 1)
					     .between(1, 12)
					     .choose2(12);
}
}



int main()
{
    while(1){
//	test_bugs();
	test_choose_number2();
    }
}



