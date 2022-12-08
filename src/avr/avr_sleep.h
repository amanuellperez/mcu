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

#pragma once

#ifndef __AVR_SLEEP_H__
#define __AVR_SLEEP_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para dormir el avr
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    01/01/2020 v0.0
 *
 ****************************************************************************/
#include <avr/sleep.h>


namespace avr_{
// ----------------
// Traductor básico 
// ----------------
/// Si a_dormir == true duerme el avr, cambiando el valor de a_dormir = false.
/// postcondition: a_dormir == false; (salvo que se cambie en alguna
/// interrupción)
/// precondition: Se ha llamado a set_sleep_mode antes. <-- no olvidarlo.
// Devuelve si el avr se ha dormido o no.
bool sleep_if(volatile bool& to_sleep);

// De momento defino este interfaz para no tener que usar abreviaturas
struct Sleep{
    static void set_mode_power_down()
    { set_sleep_mode(SLEEP_MODE_PWR_DOWN); }

};


/*!
 *  \brief  Gestiona el encendido apagado del avr (sleep or not to sleep)
 *
 *  v0.0: Nunca he manejado esto. Voy a suponer (???) que una aplicación solo
 *  tiene una forma de gestionar cómo dormir el avr: o duermes solo una parte,
 *  o duermes todo... Por ello voy a probar a definir interfaces que gestionen
 *  de una única forma el modo de sleep, ocultando toda la gestión al cliente
 *  de tal forma que no tenga que preocuparse de nada.
 *
 *  Como siempre, si el usuario quiere hacer una gestión diferente, puede
 *  llamar directamente a Sleep, e incluso a las funciones de avrlibc.
 *
 */
class Power {
public:
    static void init() { Sleep::set_mode_power_down(); }
    static void turn_off_next_time() {to_sleep_ = true;}
    static void turn_on_next_time() {to_sleep_ = false;}

    // turn off if neccesary: 
    // Esto es, si se ha llamado turn_off_next_time, esta función apaga el
    // avr.
    // Devuelve si el avr se ha dormido o no. En caso de haberse dormido, 
    // puede que haya que reinicializar algún periférico (ejemplo: un LCD si
    // se desconecta de corriente al dormirse el avr, hay que
    // reinicializarlo).
    static bool turn_off_if() {return sleep_if(to_sleep_);}

private:
    // ¿quiero dormirme?
    // Tiene que ser static ya que queremos acceder a ella desde ISR.
    // De esta forma evitamos definirla como variable global. (es global pero
    // metida dentro del scope Power).
    static volatile bool to_sleep_;
};


}// namespace

#endif


