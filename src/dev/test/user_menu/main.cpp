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
#include "../../user_menu.h"
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


constexpr const char menu_unidad_tiempo[] = "hora\nmin\nseg";


 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_lcd_menu()
{
    // Si lo conectamos solo a 4 pins de datos
    LCD_HD44780_1602_ostream lcd;
    // LCD_HD44780_2004_ostream lcd;

    Keyboard<keyboard_down_pin, keyboard_up_pin, keyboard_enter_pin> key;

    const char* menu =
        // "1. Primera opcion";
        "1. Primera opcion, un rato larga\n"
        "2. Segunda\n"
        "3. Tercera opcion tambien larga\n"
        "4. Cuarta\n"
        "5. Quinta";

    const char menu2[] = "seg\nmin\nhora\ndia";

    while(1){
	lcd.clear();
	lcd << "Elige: ";
	uint8_t unidad = dev::User_menu{lcd.screen(), key, menu_unidad_tiempo}
                         .num_rows(1)
                         .num_cols(4)
                         .pos(7, 1)
                         .show(1);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(unidad);
	wait_ms(1000);

	lcd.clear();
	lcd << "Elige: ";
	// uint16_t en lugar de uint8_t para poder imprimirlo en lcd <<.
        uint16_t seleccion = dev::User_menu{lcd.screen(), key, menu}
                                 .num_rows(1)
                                 .num_cols(4)
                                 .pos(6, 1)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);

	lcd.clear();
	lcd << "Elige: xxxx = unidades";
        seleccion = dev::User_menu{lcd.screen(), key, menu2}
                                 .num_rows(1)
                                 .num_cols(4)
				 .pos(7,0)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);


	lcd.clear();
	seleccion = dev::User_menu{lcd.screen(), key, menu}
			    .num_rows(2)
			    .pos(0, 1)
			    .show(3);

	lcd.clear();
	lcd << "seleccion:\n" << seleccion;
	wait_ms(1000);

	lcd.clear();
	lcd << "Todo bien? ";
	seleccion = dev::User_menu{lcd.screen(), key, "si\nno"}.show(3);

	lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	wait_ms(1000);
    }

}




int main()
{
    test_lcd_menu();
}



