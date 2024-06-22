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

#pragma once

#ifndef __SENSOR_MAIN_H__
#define __SENSOR_MAIN_H__

#include "dev.h"
#include "cfg.h"

class Main;

// Idea: concibo lo que quiero mostrar como un scroll (= papel con un número 
// fijo de filas) donde muestro solo una pequeña parte a través de una
// ventana. La ventana es del ancho del scroll, así que solo tenemos que
// moverla de arriba abajo.
struct Window_main{
    Window_main(Main* main):main_{main} {} 

    void run();

    void show();

    void scroll_up(); 
    void scroll_down();
    
    void print_date(uint8_t row, const RTC::Time_point& t0);
    void print_time(uint8_t row, const RTC::Time_point& t0);
    void print(uint8_t row,
               const Sensor::Celsius& T,
               const Sensor::Relative_humidity& H);
    void print(uint8_t row, const Sensor::Hectopascal& hp);
    
    void draw_cursor();

    void lcd_fill_blank();
    
    bool redraw();

    // window rows
    static constexpr uint8_t wrows_ = LCD::rows();
    static constexpr uint8_t wcols_ = LCD::cols();

    // scroll rows
    static constexpr uint8_t srows_ = 4;

    // window position
    uint8_t yw_ = 1; // Fila donde empezamos a dibujar el scroll

    // cursor position (relative to the window)
    uint8_t yc_ = 0;     

    // ¿hay que redibujar la ventana?
    bool redraw_ = true;

// (RRR) Necesito acceder a todo el hardware y a funciones de main.
    Main* main_;

    LCD& lcd();
    Keyrow keyboard();
    RTC& rtc();
    Sensor& sensor();

};


class Main{
public:
    Main();
    void run();

// Hardware
    LCD lcd_;
    Keyrow keyboard_;

    Sensor sensor_;
    RTC rtc_;

// Time
    void window_set_time();

private:
// init: hardware
    void init_TWI();
    void init_lcd();
    void init_keyboard() { }
    void init_sensor();
    void init_rtc_clock();


// Window: main
    void window_main();
    Window_main window_main_{this};

// Window : set_time
    void window_set_time(RTC::Time_point& t);

// Helping functions
// user
    bool user_press_change_time();
    void wait_user_release_change_time();

// time
    void init_time(RTC::Time_point& t);
    void print_datetime(atd::Generic_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0);
    void user_get_datetime(atd::Generic_time_view<RTC::Time_point> t, uint8_t x0, uint8_t y0);

// sensor
    void print_sensor();

};


inline bool Main::user_press_change_time()
{
    return (keyboard_.key<OK_KEY>().is_pressed()
	and keyboard_.key<DOWN_KEY>().is_pressed());
}

inline void Main::wait_user_release_change_time()
{
    while (keyboard_.key<OK_KEY>().is_pressed()
	 or keyboard_.key<DOWN_KEY>().is_pressed())
	my_mcu::Micro::wait_ms(100);
}

inline void wait_release_key()
{
    my_mcu::Micro::wait_ms(time_wait_release_key);
}



inline LCD& Window_main::lcd() {return main_->lcd_;}
inline Keyrow Window_main::keyboard() {return main_->keyboard_;}
inline RTC& Window_main::rtc() {return main_->rtc_;}
inline Sensor& Window_main::sensor() {return main_->sensor_;}


#endif

