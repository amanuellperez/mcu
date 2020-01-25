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

#pragma once

#ifndef __BCLOCK_MAIN_H__
#define __BCLOCK_MAIN_H__


#include "dev.h"
#include "cfg.h"
#include <dev_system_clock.h>
#include <avr_time.h>

/*!
 *  \brief  Aplicación que ejecutamos.
 *
 */
class Main {
public:
    Main();
    void run();

private:
// HARDWARE CONECTADO
    LCD_ostream lcd_;
    Keyboard keyboard_;


// INIT HARDWARE
    void init_lcd();
    void init_keyboard() { }
    void init_system_clock();


// PANTALLA PRINCIPAL
    void pantalla_principal();
    void show_pantalla_principal();
    // void menu_pantalla_principal();

// PANTALLA CAMBIO DE HORA
    void pantalla_cambio_hora(System_clock::time_point t0);

// FUNCIONES DE AYUDA
    void print(const System_clock::time_point& t0);
};


inline void wait_release_key()
{
    wait_ms(time_wait_release_key);
}



#endif

