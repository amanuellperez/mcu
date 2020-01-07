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
#include "../../dev_push_button.h"

#include <avr_time.h>


constexpr uint8_t keyboard_enter_pin = 23;
constexpr uint8_t keyboard_up_pin = 24;
constexpr uint8_t keyboard_down_pin  = 25;


template <uint8_t down_pin, uint8_t up_pin, uint8_t enter_pin>
struct Keyboard{
    dev::Push_button<down_pin> down;
    dev::Push_button<up_pin> up;
    dev::Push_button<enter_pin> enter;
};

using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
				       >;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

using LCD_HD44780_1602_ostream = dev::LCD_HD44780_1602_ostream<LCD_HD44780>;
using LCD_HD44780_2004_ostream = dev::LCD_HD44780_2004_ostream<LCD_HD44780>;



 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_choose_number2()
{
    LCD_HD44780_1602_ostream lcd;

    Keyboard<keyboard_down_pin, keyboard_up_pin, keyboard_enter_pin> key;

    while(1){
	lcd.clear();
	lcd << "Choose number";
	wait_ms(1000);

	lcd.clear();
	lcd << "2 cifras [5,25]";
	wait_ms(1000);
	uint8_t u8 =  dev::User_choose_number{lcd, key}.pos(3, 1)
					     .between(5, 25)
					     .choose2(10);
	lcd.cursor_pos(0,0);
	lcd << "has elegido: " << static_cast<uint16_t>(u8);

	wait_ms(1000);

	lcd.clear();
	lcd << "4 [2890, 2910]";
	uint16_t u16  =  dev::User_choose_number{lcd, key}.pos(3, 1)
						 .between(2890, 2910)
						 .choose4(2900);
	lcd.cursor_pos(0,0);
	lcd << "has elegido: " << u16;
    }
}




int main()
{
    test_choose_number2();
}



