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
#include "../../dev_pulsador.h"

#include <avr_time.h>


constexpr uint8_t teclado_enter_pin = 23;
constexpr uint8_t teclado_up_pin = 24;
constexpr uint8_t teclado_down_pin  = 25;

using namespace dev;

template <uint8_t down_pin, uint8_t up_pin, uint8_t enter_pin>
struct Teclado{
    Pulsador<down_pin> down;
    Pulsador<up_pin> up;
    Pulsador<enter_pin> enter;
};



 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_choose_number2()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780_1602_ostream lcd{ LCD_HD44780::DPin_RS{4}
		    , LCD_HD44780::DPin_RW{5}
		    , LCD_HD44780::DPin_E {6}
		    , LCD_HD44780::DPin_D4{11, 12, 13, 14}};

    Teclado<teclado_down_pin, teclado_up_pin, teclado_enter_pin> key;

    while(1){
	lcd.clear();
	lcd << "Choose number";
	wait_ms(1000);

	lcd.clear();
	lcd << "2 cifras [5,25]";
	wait_ms(1000);
	uint8_t u8 =  User_choose_number{lcd, key}.pos(3, 1)
					     .between(5, 25)
					     .choose2(10);
	lcd.cursor_pos(0,0);
	lcd << "has elegido: " << static_cast<uint16_t>(u8);

	wait_ms(1000);

	lcd.clear();
	lcd << "4 [2890, 2910]";
	uint16_t u16  =  User_choose_number{lcd, key}.pos(3, 1)
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



