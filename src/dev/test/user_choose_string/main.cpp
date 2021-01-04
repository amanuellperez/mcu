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
#include "../../user_choose_string.h"
#include "../../dev_LCD_HD44780.h"
#include "../../dev_keyboard.h"

#include <atd_cstring.h>    // nstring

#include <avr_time.h>
#include <array>

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


constexpr std::array menu_unidad_tiempo = {"hora", "min", "seg"};


 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_lcd_menu()
{
    // Si lo conectamos solo a 4 pins de datos
    // LCD_HD44780_1602_ostream lcd;
    LCD_HD44780_2004_ostream lcd;

    Keyboard keyboard;

    constexpr std::array menu ={
	"0. cero",
        "1. Primera opcion, un rato larga",
        "2. Segunda",
        "3. Tercera opcion tambien larga",
        "4. Cuarta",
        "5. Quinta"};

    constexpr std::array menu2 ={ "seg", "min", "hora", "dia"};


    while(1){

//	constexpr std::array days ={'D', 'L', 'M', 'X', 'J', 'V', 'S'};
	constexpr const char* days = "DLMXJVS";

	lcd.clear();
	lcd << "Elige: ";
	uint8_t day = dev::user_choose_string<1>(lcd.screen(), keyboard, 
		    atd::Array_const_nstrings{days, 1})
                         .pos(7, 1)
                         .show(0);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(day);
	wait_ms(1000);

//	constexpr std::array days2 ={"Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sa"};
	constexpr const char* days2 = "DoLuMaMiJuViSa";

	lcd.clear();
	lcd << "Elige: ";
	uint8_t day2 = dev::user_choose_string<2>(lcd.screen(), keyboard, 
		    atd::Array_const_nstrings{days2, 2})
                         .pos(7, 1)
                         .show(2);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(day2);
	wait_ms(1000);


	lcd.clear();
	lcd << "Elige: ";
	uint8_t unidad = dev::user_choose_string<4>(lcd.screen(), keyboard, menu_unidad_tiempo)
                         .pos(7, 1)
                         .show(1);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(unidad);
	wait_ms(1000);

	lcd.clear();
	lcd << "Elige: ";
	// uint16_t en lugar de uint8_t para poder imprimirlo en lcd <<.
        uint16_t seleccion = dev::user_choose_string<4>(lcd.screen(), keyboard, menu)
                                 .pos(6, 1)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);

	lcd.clear();
	lcd << "Elige: xxxx = unidades";
        seleccion = dev::user_choose_string<4>(lcd.screen(), keyboard, menu2)
				 .pos(7,0)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);


	lcd.clear();
	seleccion = dev::user_choose_string<10,2>(lcd.screen(), keyboard, menu)
			    .pos(0, 1)
			    .show(3);

	lcd.clear();
	lcd << "seleccion:\n" << seleccion;
	wait_ms(1000);

	lcd.clear();
	lcd << "Todo bien? ";
	constexpr std::array sino = {"si", "no"};
        seleccion =
            dev::user_choose_string<2,2>(lcd.screen(), keyboard, sino).show(0);

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);

	lcd.clear();
	lcd << "Todo bien (2)? ";
        seleccion =
            dev::user_choose_string<2>(lcd.screen(), keyboard, sino).show(0);

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);
    }

}




int main()
{
    test_lcd_menu();
}



