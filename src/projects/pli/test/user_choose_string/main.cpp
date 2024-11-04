// Copyright (C) 2019-2022 Manuel Perez 
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

// Conectar el LCD y 3 pulsadores a los pines indicados
#include <dev_LCD_screen.h> // TODO: siempre ponerlo al principio
#include "../../user_choose_string.h"
#include <mega.h>

#include <dev_HD44780_generic.h>
#include <dev_keyrow.h>

#include <atd_cstring.h>    // nstring

#include <avr_time.h>
#include <array>

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

// Pin connections
// ---------------
// LCD
using LCD_pins = dev::HD44780_pins4<dev::HD44780_RS<4>,
				       dev::HD44780_RW<5>,
				       dev::HD44780_E<6>,
				       dev::HD44780_D4<11,12,13,14>
				       >;

// keyrow
using Keyrow_pins  = dev::Keyrow_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyrow_codes  = dev::Keyrow_codes<OK_KEY, UP_KEY, DOWN_KEY>;



// dispositivos que conectamos
// ---------------------------
using Keyrow = dev::Basic_keyrow<Micro, Keyrow_pins, Keyrow_codes>;

// Dispositivos reales conectados
using LCD_1602 = dev::LCD_HD44780_1602<Micro, LCD_pins>;
using LCD_2004 = dev::LCD_HD44780_2004<Micro, LCD_pins>;

// Screens
using Screen_1602 = dev::LCD_screen_1602<LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<LCD_2004>;

// Choose LCD to test
//using LCD = Screen_1602;
using LCD = Screen_2004;


constexpr std::array menu_unidad_tiempo = {"hora", "min", "seg"};


// (RRR) Relleno el lcd con puntos para ver si el menu rellena su ventana
//       correctamente.
void lcd_clear(LCD& lcd)
{
    lcd.clear();
    for (uint8_t r = 0; r < lcd.rows(); ++r){
	lcd.cursor_pos(0, r);
	for (uint8_t c = 0; c < lcd.cols(); ++c)
	    lcd.print('.');
    }
    
    lcd.cursor_pos(0,0);
}

 
// Probamos el LCD_stream conectado a 4 pines de datos 
void test_lcd_menu()
{
    LCD lcd;

    Keyrow keyrow;

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

	lcd_clear(lcd);
//	lcd.clear();
	lcd << "Elige (circular): ";
	uint8_t day = dev::user_choose_string_circular<1>(lcd, keyrow, 
		    atd::Array_const_nstrings{days, 1})
                         .pos(7, 1)
                         .show(0);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(day);
	myu::Micro::wait_ms(1000);

//	constexpr std::array days2 ={"Do", "Lu", "Ma", "Mi", "Ju", "Vi", "Sa"};
	constexpr const char* days2 = "DoLuMaMiJuViSa";

//	lcd.clear();
	lcd_clear(lcd);
	lcd << "Elige: (circular) ";
	uint8_t day2 = dev::user_choose_string_circular<2>(lcd, keyrow, 
		    atd::Array_const_nstrings{days2, 2})
                         .pos(7, 1)
                         .show(2);

	lcd.clear();
	lcd_clear(lcd);
	lcd << "Seleccion: " << static_cast<uint16_t>(day2);
	myu::Micro::wait_ms(1000);


//	lcd.clear();
	lcd_clear(lcd);
	lcd << "Elige: ";
	uint8_t unidad = dev::user_choose_string_lineal<4>(lcd, keyrow, menu_unidad_tiempo)
                         .pos(7, 1)
                         .show(1);

	lcd.clear();
	lcd << "Seleccion: " << static_cast<uint16_t>(unidad);
	myu::Micro::wait_ms(1000);

//	lcd.clear();
	lcd_clear(lcd);
	lcd << "Elige: ";
	// uint16_t en lugar de uint8_t para poder imprimirlo en lcd <<.
        uint16_t seleccion = dev::user_choose_string_lineal<4>(lcd, keyrow, menu)
                                 .pos(6, 1)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	myu::Micro::wait_ms(1000);

	lcd_clear(lcd);
	lcd << "Elige: xxxx = unidades";
        seleccion = dev::user_choose_string_lineal<4>(lcd, keyrow, menu2)
				 .pos(7,0)
                                 .show();

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	myu::Micro::wait_ms(1000);


	lcd_clear(lcd);
	seleccion = dev::user_choose_string_lineal<10,2>(lcd, keyrow, menu)
			    .pos(0, 1)
			    .show(3);

	lcd.clear();
	lcd << "seleccion:\n" << seleccion;
	myu::Micro::wait_ms(1000);

	lcd_clear(lcd);
	lcd << "Todo bien? ";
	constexpr std::array sino = {"si", "no"};
        seleccion =
            dev::user_choose_string_lineal<2,2>(lcd, keyrow, sino).show(0);

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	myu::Micro::wait_ms(1000);

	lcd_clear(lcd);
	lcd << "Todo bien (2)? ";
        seleccion =
            dev::user_choose_string_lineal<2>(lcd, keyrow, sino).show(0);

        lcd.clear();
	lcd << "seleccion:\n" << seleccion;

	myu::Micro::wait_ms(1000);
    }

}




int main()
{
    test_lcd_menu();
}



