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
#include "../../user_time.h"
#include "../../dev_LCD_HD44780.h"
#include "../../dev_push_button.h"

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
using Keyboard_pins  = dev::Keyboard_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyboard_codes  = dev::Keyboard_codes<OK_KEY, UP_KEY, DOWN_KEY>;



// dispositivos que conectamos
// ---------------------------
using Keyboard = dev::Basic_keyboard<Keyboard_pins, Keyboard_codes>;

using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

using LCD_HD44780_1602_ostream = dev::LCD_HD44780_1602_ostream<LCD_HD44780>;
using LCD_HD44780_2004_ostream = dev::LCD_HD44780_2004_ostream<LCD_HD44780>;


tm new_tm() 
{
    tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 0;
    t.tm_year = 2019 - 1900;
    t.tm_isdst = 0;

    return t;
}

 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_lcd_menu()
{
    // Si lo conectamos solo a 4 pins de datos
    // NO USAR ESTE: LCD_HD44780_1602_ostream lcd;
    LCD_HD44780_2004_ostream lcd;

    Keyboard key;


    while(1){
	lcd.clear();
	lcd << "Probarlo con un LCD de 20 x 4!";
	wait_ms(1000);

	lcd.clear();
	lcd << "Primera prueba: time_t";
	std::time_t t = 630152224; // 20/12/2019 10:17:04
	t = dev::user_get_time(lcd, key, t, 4, 2);

	std::tm* t0 = std::gmtime(&t);
	lcd.cursor_pos(0,0);
	lcd << "Escrito:" << atd::only_date(*t0) << ' ' << atd::only_time(*t0);
	wait_ms(4000);

	{
            lcd.clear();
            lcd << "Segunda prueba: duration";
            std::time_t t = 630152224; // 20/12/2019 10:17:04
            auto td        = std::chrono::system_clock::from_time_t(t);
	    td = dev::user_get_time(lcd, key, td, 4, 2);

	    t = std::chrono::system_clock::to_time_t(td);
            std::tm* t0 = std::gmtime(&t);
            lcd.cursor_pos(0, 0);
            lcd << "Escrito:" << atd::only_date(*t0) << ' '
                << atd::only_time(*t0);
            wait_ms(4000);
        }

    }

}




int main()
{
    test_lcd_menu();
}



