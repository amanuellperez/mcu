// Copyright (C) 2019-2020 A.Manuel L.Perez 
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
// NO USAR EL DE 16 x 02. USAR SOLO EL DE 20x04 para que se vea bien.
using LCD_ostream_2004 = dev::LCD_ostream_2004<LCD_HD44780>;

// Choose LCD to test
using LCD = LCD_ostream_2004;


tm new_tm() 
{
    tm t;
    t.tm_sec = 0;
    t.tm_min = 0;
    t.tm_hour = 0;
    t.tm_mday = 1;
    t.tm_mon = 0;
    t.tm_year = 2019 - 1900;

    t.tm_wday = 2;

    t.tm_isdst = 0;

    return t;
}

 
constexpr const char* week_days = "DoLuMaMiJuViSa";
constexpr const uint8_t week_days_length = 2;

template <typename LCD, typename Keyrow, typename T>
void get_time(LCD& lcd, Keyrow& key, 
	      atd::Generic_time<T>& t, 
	      uint8_t x0, uint8_t y0)
{
    lcd.cursor_pos(x0, y0);
    atd::print_date(lcd, t);

    lcd.cursor_pos(x0, y0 + 1);
    atd::print_time(lcd, t);

    lcd.cursor_pos(x0 + 9, y0+1);
    atd::print_weekday<week_days_length>(lcd, t, week_days);

    dev::user_get_date(lcd, key, t, x0, y0);
    dev::user_get_time(lcd, key, t, x0, y0 + 1);
    dev::user_get_weekday<week_days_length>(lcd, key, t, 
					    x0 + 9, y0 + 1,
					    week_days);

}


template <typename LCD, typename Keyrow>
std::time_t get_time(
    LCD& lcd, Keyrow& key, const std::time_t& t0, uint8_t x0, uint8_t y0)
{
    std::tm* mt = std::gmtime(&t0);
    
    atd::Generic_time<std::tm> t{*mt};

    get_time(lcd, key, t, x0, y0);

    return std::mktime(mt);
}




template <typename LCD, typename Keyrow, typename Clock, typename Duration>
std::chrono::time_point<Clock, Duration> user_get_datetime(LCD& lcd,
                                         Keyrow& key,
                                         const std::chrono::time_point<Clock, Duration>& t0,
                                         uint8_t x0, uint8_t y0)
{
    time_t t = std::chrono::system_clock::to_time_t(t0);
    t = get_time(lcd, key, t, x0, y0);

    return std::chrono::system_clock::from_time_t(t);
}

template <typename LCD, typename Keyrow, typename T>
void test_user_time(LCD& lcd, Keyrow key, atd::Generic_time<T> t)
{
    get_time(lcd, key, t, 4, 2);

    lcd.cursor_pos(0,0);

    lcd << "Escrito: " << atd::only_date(t)
	<< ' ' << atd::only_time(t) << ' ';
    atd::print_weekday<week_days_length>(lcd, t, week_days);
    

    wait_ms(4000);
}


void test_user_time()
{
    LCD lcd;

    Keyrow key;


    while(1){
	lcd.clear();
	lcd << "Probarlo con un LCD de 20 x 4!";
	wait_ms(1000);

	lcd.clear();
	lcd << "Primera prueba: time_t";
	std::time_t time0 = 630152224; // 20/12/2019 10:17:04
	std::tm* t0 = std::gmtime(&time0);

	test_user_time(lcd, key, atd::Generic_time<std::tm>{*t0});


	{
            lcd.clear();
            lcd << "Segunda prueba: duration";
            std::time_t t = 630152224; // 20/12/2019 10:17:04
            auto td        = std::chrono::system_clock::from_time_t(t);
	    td = user_get_datetime(lcd, key, td, 4, 2);

	    t = std::chrono::system_clock::to_time_t(td);
            std::tm* t0 = std::gmtime(&t);
            lcd.cursor_pos(0, 0);
            lcd << "Escrito:" << atd::only_date(*t0) << ' '
                << atd::only_time(*t0)
		<< " xx";
//            atd::print_weekday<week_days_length>(
//                lcd, atd::Generic_time<std::tm>{*t0}, week_days);
            wait_ms(4000);
        }

    }

}




int main()
{
    test_user_time();
}



