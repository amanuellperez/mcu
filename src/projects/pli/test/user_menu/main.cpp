// Copyright (C) 2021 Manuel Perez 
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
#include "../../user_menu.h"
#include <dev_HD44780.h>
#include <dev_LCD_screen.h>
#include <dev_keyrow.h>

#include <avr_atmega.h>

// Microcontroller
// ---------------
namespace myu = atmega;
using Micro   = myu::Micro;

// Pin connections
// ---------------
// LCD
using LCD_pins = dev::LCD_HD44780_pins4<dev::LCD_HD44780_RS<4>,
				       dev::LCD_HD44780_RW<5>,
				       dev::LCD_HD44780_E<6>,
				       dev::LCD_HD44780_D4<11,12,13,14>
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

// Generic devices
using Generic_LCD_1602 = dev::Generic_LCD<LCD_1602>;
using Generic_LCD_2004 = dev::Generic_LCD<LCD_2004>;


// Screens
using Screen_1602 = dev::LCD_screen_1602<Generic_LCD_1602>;
using Screen_2004 = dev::LCD_screen_2004<Generic_LCD_2004>;

//using LCD = Screen_1602;
using LCD = Screen_2004;


struct Main{
    void a2() {lcd.clear(); lcd << "ejecutando a2"; myu::Micro::wait_ms(500);}
    void b1() {lcd.clear(); lcd << "ejecutando b1"; myu::Micro::wait_ms(500);}
    void c1() {lcd.clear(); lcd << "ejecutando c1"; myu::Micro::wait_ms(500);}
    void c2() {lcd.clear(); lcd << "ejecutando c2"; myu::Micro::wait_ms(500);}
    void c3() {lcd.clear(); lcd << "ejecutando c3"; myu::Micro::wait_ms(500);}
    void d1() {lcd.clear(); lcd << "ejecutando d1"; myu::Micro::wait_ms(500);}
    void d2() {lcd.clear(); lcd << "ejecutando d2"; myu::Micro::wait_ms(500);}
    

    void run();

// Hardware
    LCD lcd;
    Keyrow keyrow;

    void show_menu(uint8_t n);

};


#include "menu.h"


void Main::run()
{
    auto menu = dev::lcd_menu<Options>(this, lcd, keyrow, menu_str);

    while(1){
	menu.show_menu(0);
    }

}



void test_user_menu()
{
    Main app;
    app.run();
}


int main()
{
    test_user_menu();
}



