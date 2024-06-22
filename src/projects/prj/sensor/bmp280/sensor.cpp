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


void Main::init_sensor()
{
    sensor_.init();
//    sensor_.handheld_device_low_power();
    sensor_.indoor_navigation();

    if (sensor_.error()) {
	lcd_.clear();
	lcd_ << "Sensor no responde";
	my_mcu::Micro::wait_ms(1000);
    }
}

void Main::print_sensor()
{
    auto [T, hP] = sensor_.T_and_hP();

    lcd_.cursor_pos(0, 0);
    lcd_ << T << ' ' << lcd_symbol::of("º") << 'C';
    lcd_.cursor_pos(0, 1);
    lcd_ << hP << " hPa";
}



