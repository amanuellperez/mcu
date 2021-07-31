// Copyright (C) 2021 A.Manuel L.Perez 
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
#include "../../user_menu.h"
#include "../../dev_LCD_HD44780.h"
#include "../../dev_keyrow.h"

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
using Keyrow_pins  = dev::Keyrow_pins<23, 24, 25>;

// código asociado a cada tecla del teclado
using namespace dev::Key_codes; // OK_KEY, UP_KEY, DOWN_KEY
using Keyrow_codes  = dev::Keyrow_codes<OK_KEY, UP_KEY, DOWN_KEY>;



// dispositivos que conectamos
// ---------------------------
using Keyrow = dev::Basic_keyrow<Keyrow_pins, Keyrow_codes>;


using LCD_HD44780 = dev::LCD_HD44780<LCD_pins>;

// Tipos de LCDs probados
using LCD_ostream_1602 = dev::LCD_ostream_1602<LCD_HD44780>;
using LCD_ostream_2004 = dev::LCD_ostream_2004<LCD_HD44780>;

// Choose LCD to test
using LCD = LCD_ostream_1602;
//using LCD = LCD_ostream_2004;






struct Main{
    void a2() {lcd.clear(); lcd << "ejecutando a2"; wait_ms(500);}
    void b1() {lcd.clear(); lcd << "ejecutando b1"; wait_ms(500);}
    void c1() {lcd.clear(); lcd << "ejecutando c1"; wait_ms(500);}
    void c2() {lcd.clear(); lcd << "ejecutando c2"; wait_ms(500);}
    void c3() {lcd.clear(); lcd << "ejecutando c3"; wait_ms(500);}
    void d1() {lcd.clear(); lcd << "ejecutando d1"; wait_ms(500);}
    void d2() {lcd.clear(); lcd << "ejecutando d2"; wait_ms(500);}
    

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



