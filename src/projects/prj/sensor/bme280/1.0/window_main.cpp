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

#include "main.h"

bool Window_main::redraw()
{
    static uint8_t n = sensor_update_every;
    if (n == sensor_update_every){
	n = 0;
	return true;
    }
    ++n;

    if (redraw_){
	redraw_ = false;
	return true;
    }

    return false;
}


void Window_main::run()
{
    if (redraw())
	show();

    if(keyboard().key<DOWN_KEY>().is_pressed())
	scroll_down();

    else if(keyboard().key<UP_KEY>().is_pressed())
	scroll_up();

    else if(keyboard().key<OK_KEY>().is_pressed()){
	switch(yw_ + yc_){
	    case 0:
	    case 1: 
		main_->window_set_time();
		break;

	    case 2: break;
	    case 3: break;
	}

	redraw_ = true;
    }

}



void Window_main::show()
{
// 1. toma de datos
    RTC::Time_point t0;
    rtc().read(t0);
    
    Sensor::Celsius T;
    Sensor::Pascal P;
    Sensor::Relative_humidity H;
    sensor().T_and_P_and_H(T, P, H);
    Sensor::Hectopascal hP{P};

// 2. print data
    uint8_t y0 = yw_;
    uint8_t y1 = yw_ + wrows_ - 1;
    uint8_t lcd_row = 0;

//    if (y0 <= 0 and 0 <= yM){
    if (y0 == 0){
	print_date(lcd_row, t0);
	++lcd_row;
    }

    if (y0 <= 1 and 1 <= y1){
	print_time(lcd_row, t0);
	++lcd_row;
    }

    if (y0 <= 2 and 2 <= y1){
	print(lcd_row, T, H);
	++lcd_row;
    }

//    if (y0 <= 3 and 3 <= y1){
    if (y1 == 3){
	print(lcd_row, hP);
//	++lcd_row;
    }
	
    draw_cursor();
}

void Window_main::scroll_up()
{
    if (yc_ > 0){
	--yc_;
	redraw_ = true;
    }

    else if(yw_ > 0){
	--yw_;
	redraw_ = true;
    }
}

void Window_main::scroll_down()
{
    if (yc_ < wrows_ -  1){
	++yc_;
	redraw_ = true;
    }

    else if (yw_ + wrows_ < srows_){
	++yw_;
	redraw_ = true;
    }
}

void Window_main::lcd_fill_blank()
{
    // OJO: -1!! el cursor no lo voy a borrar, ya que si no parpadea.
    for (uint8_t x = lcd().cursor_pos_x(); x < lcd().cols() - 1; ++x)
	lcd().print(' ');
}

void Window_main::print_date(uint8_t row, const RTC::Time_point& t0)
{
    atd::const_Generic_time_view<RTC::Time_point> t{t0};
    lcd().cursor_pos(0, row);
    atd::print(lcd(), t.day(), nm::Width{2});
    lcd() << '/';
    atd::print(lcd(), t.month(), nm::Width{2});
    lcd() << '/' << t.year() << ' ';
//    lcd() << std::setw(2) << t.day() << '/'
//	<< std::setw(2) << t.month() << '/'
//			<< t.year() << ' ';

    atd::print_weekday<week_days_length>(lcd(), t, week_days);

    lcd_fill_blank();
}

void Window_main::print_time(uint8_t row, const RTC::Time_point& t0)
{
    atd::const_Generic_time_view<RTC::Time_point> t{t0};
    lcd().cursor_pos(0, row);
    atd::print(lcd(), t.hours(), nm::Width{2});
    lcd() << ':';
    atd::print(lcd(), t.minutes(), nm::Width{2});
    lcd() << ':';
    atd::print(lcd(), t.seconds(), nm::Width{2});
//    lcd() << std::setw(2) << t.hours() << ':'
//	<< std::setw(2) << t.minutes() << ':'
//	<< std::setw(2) << t.seconds();

    lcd_fill_blank();
}

void Window_main::print(uint8_t row,
                        const Sensor::Celsius& T,
                        const Sensor::Relative_humidity& H)
{
    lcd().cursor_pos(0, row);
    lcd() << T << ' ' << lcd_symbol::of("º") << "C "
	 << H << '%';
    lcd_fill_blank();

}

void Window_main::print(uint8_t row, const Sensor::Hectopascal& hP)
{
    lcd().cursor_pos(0, row);
    lcd() << hP << " hPa ";
    lcd_fill_blank();
}


// (RRR) Busco código genérico que pueda reusar, por eso uso un bucle for,
//       para que funcione con cualquier LCD.
// (RRR) Hay que borrar el cursor de las otras filas. Se podría gestionar con 
//       un flag que diga si borrarlo o no. ¿Mejoraría? 
void Window_main::draw_cursor()
{
    for (uint8_t y = 0; y < lcd().rows(); ++y){
	lcd().cursor_pos(wcols_ - 1, y);
	if (y == yc_)
	    lcd() << lcd_symbol::of("←");
	else
	    lcd() << ' ';
    }
}

