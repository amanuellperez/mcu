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

// Conectar el LCD y 3 pulsadores a los pines indicados
#include "../../user_choose_number.h"
#include "../../dev_LCD_HD44780.h"
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

// keyboard
// pines a los que conectamos el teclado
using Keyboard_pins  = dev::Keyboard_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyboard_codes  = dev::Keyboard_codes<OK_KEY, UP_KEY, DOWN_KEY>;



// dispositivos que conectamos
// ---------------------------
using Keyboard = dev::Basic_keyboard<Keyboard_pins, Keyboard_codes>;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

// Tipos de LCDs probados
using LCD_ostream_1602 = dev::LCD_ostream_1602<LCD_HD44780>;
using LCD_ostream_2004 = dev::LCD_ostream_2004<LCD_HD44780>;

// Choose LCD to test
using LCD = LCD_ostream_1602;
// using LCD = LCD_ostream_2004;


 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_choose_number2()
{
    LCD lcd;

    Keyboard keyboard;

    lcd.clear();
    lcd << "Choose number";
    wait_ms(1000);

    lcd.clear();
    lcd << "LINEAL TEST";
    wait_ms(1000);

//
{
using namespace avr::literals;
    lcd.clear();
    lcd << "ENG frequency";
    using ENG_frequency = avr::ENG_frequency;
    ENG_frequency u8 = dev::user_choose_number_lineal<LCD, Keyboard, ENG_frequency>(
		       lcd, keyboard)
		       .pos(3, 1)
		       .choose4(ENG_frequency{999, 0});

    lcd.cursor_pos(0,0);
    lcd << "has elegido: " << u8;

    wait_ms(1000);
}
{
using namespace avr::literals;
    lcd.clear();
    lcd << "Hertz [5,25]";
    avr::Hertz u8 = dev::user_choose_number_lineal<LCD, Keyboard, avr::Hertz>(
		     lcd, keyboard)
		     .pos(3, 1)
		     .between(5_Hz, 25_Hz)
		     .choose2(10_Hz);

    lcd.cursor_pos(0,0);
    lcd << "has elegido: " << u8;

    wait_ms(1000);
}
{
using Rep = atd::Decimal<uint16_t, 2>;
    lcd.clear();
    lcd << "Decimal [5,25]";
    wait_ms(1000);
    Rep u8 = dev::user_choose_number_lineal<LCD, Keyboard, Rep>(
		     lcd, keyboard)
		     .pos(3, 1)
		     .between(5, 25)
		     .choose2(10);

    lcd.cursor_pos(0,0);
    lcd << "has elegido: " << u8;

    wait_ms(1000);
}
//

    lcd.clear();
    lcd << "2 digits [5,25]";
    wait_ms(1000);
    uint8_t u8 =  dev::user_choose_number_lineal(lcd, keyboard).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    lcd.cursor_pos(0,0);
    lcd << "has elegido: " << static_cast<uint16_t>(u8);

    wait_ms(1000);

    lcd.clear();
    lcd << "4 [2890, 2910]";
    uint16_t u16  =  dev::user_choose_number_lineal(lcd, keyboard).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    lcd.cursor_pos(0,0);
    lcd << "elegido : " << u16;

    wait_ms(1000);

    lcd.clear();
    lcd << "CIRCULAR TEST";
    wait_ms(1000);

    lcd.clear();
    lcd << "2 digits [5,25]";
    wait_ms(1000);
    u8 =  dev::user_choose_number_circular(lcd, keyboard).pos(3, 1)
					 .between(5, 25)
					 .choose2(10);
    lcd.cursor_pos(0,0);
    lcd << "has elegido: " << static_cast<uint16_t>(u8);

    wait_ms(1000);

    lcd.clear();
    lcd << "4 [2890, 2910]";
    u16  =  dev::user_choose_number_circular(lcd, keyboard).pos(3, 1)
					     .between(2890, 2910)
					     .choose4(2900);
    lcd.cursor_pos(0,0);
    lcd << "elegido : " << u16;

    wait_ms(1000);
}


void test_counter()
{
    constexpr uint8_t T_clock = 100;

    LCD lcd;
    lcd.clear();



    dev::__user_choose_number::Counter counter;

    lcd.clear();
    lcd << "Testing velocity";
    wait_ms(1000);
    lcd.clear();
    counter.reset();

    for (uint8_t i = 0; i < 40; ++i) {
        counter.tick();
	if (counter.trigger())
	    lcd << '-';

	wait_ms(T_clock);
    }
    wait_ms(1000);

    lcd << "Counter test:";
    wait_ms(1000);
    lcd.clear();

    for (uint8_t i = 0; i < 40; ++i) {
	lcd.clear();
	lcd << (uint16_t) counter.as_uint();
	if (counter.trigger()){
	    lcd.cursor_pos(5,0);
	    lcd << 'T';
	}
	if (counter.end()){
	    lcd.cursor_pos(5,0);
	    lcd << 'E';
	}

        counter.tick();
	wait_ms(500);
    }

    wait_ms(1000);
}


int main()
{
    while(1){
	//test_counter();
	test_choose_number2();
    }
}



