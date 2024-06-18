// Copyright (C) 2024 Manuel Perez 
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

#ifndef __DEV_JOYSTICK_H__
#define __DEV_JOYSTICK_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Driver para manejar un joystick
 *
 * PROBLEMA
 *	Un problema que me encuentro al intentar generalizar es el
 *  acoplamiento que existe en el hardware. Por ejemplo, si yo quiero leer dos
 *  pines usando el ADC en el joystick, pero en otro dispositivo quiero leer
 *  otro pin ADC lo puedo hacer usando el hardware del atmega. El problema es
 *  que cualquier cambio que realice en la configuración del ADC en un device
 *  se refleja en el otro. 
 *	¿Cómo resolver este problema?
 *	Repartiendo responsabilidades:
 *	1) El harwador será el responsable de configurar el ADC en el
 *	   programa y nunca los traductores de dispositivos.
 *
 *	2) Al traductor del device el harwador le pasa un ADC_pin del tipo 
 *	   que corresponda con la configuración que haya hecho el harwador.
 *	   Es el harwador el responsable de que el ADC esté bien configurado y
 *	   que todos los dispositivos que usa la aplicación usen el mismo tipo
 *	   de ADC_pin (ADC_pin_single_mode, ADC_pin_free_running_mode...)
 *
 *  Observar que 1) no es más que un caso particular de la regla: "el harwador
 *  es responsable de configurar los dispositivos del micro que use y de
 *  configurar adecuadamente, en tiempo de compilación, las clases que usará
 *  el programador". El programador NO debe de saber nada del hardware real
 *  que está usando, sino solo de los dispositivos "virtuales" que definimos
 *  en esta capa dev.
 *
 *  NOTA: observar que voy modificando hardwador en harwador. ¿podré llamarlo
 *  jarwador? o mejor ¿jarguador? xD
 *
 *	
 *
 * HISTORIA
 *    Manuel Perez
 *    15/06/2024 Escrito
 *
 ****************************************************************************/

#include <cstdint>
#include <type_traits>

#include <atd_geometry_2d.h>

#include "dev_push_button.h"

namespace dev{


// (suposición) Voy a suponer que ADC_x_pin y ADC_y_pin son del mismo tipo, de
// tal manera que todos los tipos internos (Potential, resolution) coinciden
// para ambas clases.
template <typename Micro0, 
	  typename ADC_x_pin0,
	  typename ADC_y_pin0,
	  uint8_t push_button_pin0, 
	  bool y_axis_increase_from_bottom_up0 = false>
struct Joystick_cfg_type1{
    using Micro = Micro0;
    using ADC_x_pin = ADC_x_pin0;
    using ADC_y_pin = ADC_y_pin0;
    
    static constexpr uint16_t resolution = ADC_x_pin::resolution;

    static constexpr uint8_t push_button_pin = push_button_pin0;

    static constexpr 
	bool y_axis_increase_from_bottom_up = y_axis_increase_from_bottom_up0;
};

// DUDA: ¿qué tipo de joysticks hay en el mercado?
// Como lo desconozco no puedo clasificarlos por nombre, asi que de momento
// uso una terminología genérica:
//
//	Joystick type1 = joystick más un pulsador (el básico de arduino)
//
// DUDA: ¿interfaz static o no?
//  Cuando una clase no tiene state prefiero definirla con un interfaz static,
//  eliminando el constructor forzando así al cliente a usarla como mero
//  interfaz. 
//  Si tiene estado como la presente la defino no static, a pesar de que en
//  este caso concreto el state está asociado únicamente a esta clase (el
//  compilador va a crear una clase nueva por cada joystick basado en los
//  pines) luego podría definirlo como static. 
//  De momento mantengo el criterio: ¿tiene state? sí ==> defino una clase
//  normal.
//  
template <typename Cfg>
class Joystick_type1{
public:
// Cfg
//  El joystick marca un potencial en arefs de 0 a 1023. 
//  Resulta que en el que tengo si coloco el eje x de manera que vaya de 0 a
//  1023 al moverlo de izda a dcha el eje y va de 1023 a 0 al moverlo de abajo
//  hacia arriba (está diseñado así a posta o los hacen a voleo ???) Para
//  poder operar tanto con este joystick como con otros en los que el eje y
//  vaya al revés (de 0 a 1023 al ir de abajo->arriba) defino esta variable
//	Si el valor del eje y va de 0 a 1023 al ir de abajo->arriba, marcarla
//	como true; en caso contrario como false.
    static constexpr 
	bool y_axis_increase_from_bottom_up 
					= Cfg::y_axis_increase_from_bottom_up;

// Types
    using Micro  = typename Cfg::Micro;
    using X_pin  = Cfg::ADC_x_pin;
    using Y_pin  = Cfg::ADC_y_pin;
    using Button = Push_button<Micro, Cfg::push_button_pin>;
    using Potential = typename X_pin::Potential;
    static_assert(std::is_same_v<Potential, typename Y_pin::Potential>);
    using Point = atd::geo2d::Point<int16_t>;

    // resolución del ADC: si, por ejemplo, resolution = 1024 (10 bits)
    // el valor del ADC_pin irá de 0 a 1023.
    static constexpr uint16_t resolution = X_pin::resolution;

// Constructor
//  precondition: ADC_pin enable and working
    Joystick_type1();
    
// State
    // Devuelve las coordenadas de (-resolution/2 - 1, +resolution / 2)
    int16_t x();
    int16_t y();
    Point point();

    bool is_pressed() { return Button::is_pressed(); }
    bool is_not_pressed() {return Button::is_not_pressed(); }

// Debug 
    Potential x_as_volts() {return X_pin::read_potential();}
    Potential y_as_volts() {return Y_pin::read_potential();}

private:
// Data
    int16_t x_correct_ = 0;
    int16_t y_correct_ = 0;

// Helpers
    void init();

    // De momento la llamo en init.
    // Si se suelta el joystick no marca (0,0) sino que marca potencial.
    // Esta función averigua el punto medio del potencial asignando los
    // valores de las correciones
    void calibrate();

};

template <typename Cfg>
Joystick_type1<Cfg>::Joystick_type1() 
{
    init();
    calibrate(); // precondition: ADC enable
}

template <typename Cfg>
inline void Joystick_type1<Cfg>::init()
{ 
    Button::init();
    // X_pin::init(); // El cliente lo inicializa vía ADC
    // Y_pin::init();
}

template <typename Cfg>
void Joystick_type1<Cfg>::calibrate()
{
    // TODO or not TODO? if (!ADC::is_enable()) error()!!!
    // (TonT = TODO or not TODO? xD)
    x_correct_ = x();

    // El valor de y puede estar cambiado de signo en la función y(),
    // por eso tengo que leerlo directamente
    auto arefs = Y_pin::read_arefs();
    y_correct_ = arefs - resolution / 2;
}


template <typename Cfg>
inline int16_t Joystick_type1<Cfg>::x()
{
    auto arefs = X_pin::read_arefs();
    return arefs - resolution / 2 - x_correct_;
}


template <typename Cfg>
inline int16_t Joystick_type1<Cfg>::y()
{
    auto arefs = Y_pin::read_arefs();
    int16_t y = arefs - resolution / 2 - y_correct_;

    if constexpr (y_axis_increase_from_bottom_up)
	return y;

    else 
	return -y;

}

template <typename Cfg>
inline Joystick_type1<Cfg>::Point Joystick_type1<Cfg>::point()
{ return Point{x(), y()}; }

}// namespace

#endif




