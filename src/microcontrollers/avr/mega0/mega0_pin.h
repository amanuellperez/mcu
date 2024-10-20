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

#ifndef __MEGA0_PIN_H__
#define __MEGA0_PIN_H__

/****************************************************************************
 *
 * DESCRIPCION
 *	Clases para manejar pines del AVR
 *
 * HISTORIA
 *    Manuel Perez
 *    19/10/2024 Escrito
 *
 ****************************************************************************/
#include "mega0_cfg.h"

#include <mcu_pin.h>

namespace mega0_{

namespace private_ {
template <uint8_t n, typename Cfg>
class Pin{
public:
    using number_type = uint8_t;

    static constexpr bool is_a_valid_pin()
    {return Cfg::template is_a_valid_pin<n>(); }


// CONSTRUCCIÓN
    // constexpr Pin(){}	
    Pin() = delete;

    // No es posible copiar pins (aunque como es una template static no
    // debería de generar ningún problema)
    Pin(const Pin&)		= delete;
    Pin& operator=(const Pin&)	= delete;

// INFO
    static constexpr uint8_t number = n;

// CONFIGURAMOS EL cfg::PIN
    static void as_output() __attribute__((always_inline));
    static void as_input_with_pullup() __attribute__((always_inline));
    static void as_input_without_pullup() __attribute__((always_inline));

    static bool is_output_pin();
    static bool is_input_pin();
    static bool is_input_with_pullup_pin();
    static bool is_input_without_pullup_pin();


// FUNCIONES DE LECTURA
    // Si el bit es '1' devuelve un número distinto de cero.
    // Si el bit es '0' devuelve cero.
    static bool read() __attribute__((always_inline));

    /// Devuelve si el bit es 0 o no. 
    static bool is_zero() __attribute__((always_inline));

    /// Devuelve si el bit es 1 o no. 
    static bool is_one() __attribute__((always_inline));

// FUNCIONES DE ESCRITURA
    static void write_one() __attribute__((always_inline));
    static void write_zero() __attribute__((always_inline));

    /// Cambiamos el valor del pin (de 0 a 1 ó de 1 a 0).
    static void toggle() __attribute__((always_inline));

    /// Escribimos en el pin. Un valor distinto de cero es 1.
    static void write(uint8_t x) __attribute__((always_inline));
    
    // Este tipo de micros can be inverted. Pero no veo el uso y complicaría
    // esta clase. Hasta que no se vea la necesidad no suministro la funcion
    // correspondiente.

// TODO:
//// INTERRUPTS
//    // PCINT interrupts
//    // ----------------
//    // Genera una interrupción cada vez que el pin cambia de 0->1 ó de 1->0
//    static void enable_change_level_interrupt();
//    static void disable_change_level_interrupt();
//
//    using INT = INT__<cfg::nINT_of_pin<n>()>;
//

private:
    static constexpr auto bitmask() {return Cfg::template bitmask<n>();}
    static constexpr auto PORT()    {return Cfg::template PORT<n>();}
    static constexpr auto pullupen_mask()    {return Cfg::pullupen_mask;}
};


// Implementación
// --------------
template <uint8_t n, typename Cfg>
inline void Pin<n, Cfg>::as_output() 
{ PORT()->DIRSET = bitmask();}

template <uint8_t n, typename C>
inline bool Pin<n, C>::is_output_pin()
{ return (PORT()->DIR & bitmask());}


template <uint8_t n, typename C>
inline void Pin<n, C>::as_input_with_pullup() 
{
    // de entrada:
    PORT()->DIRCLR = bitmask();

    // enable pull-up resistor 
    // DUDA: mejor PORT()->PIN2CTRL = pullupen_mask(); ??? funciona como DIRCLR?
    PORT()->PIN2CTRL |= pullupen_mask();

}

template <uint8_t n, typename C>
inline void Pin<n, C>::as_input_without_pullup() 
{
    // de entrada:
    PORT()->DIRCLR = bitmask();

    // disable pull-up resistor 
    PORT()->PIN2CTRL &= ~pullupen_mask();
}

template <uint8_t n, typename C>
bool Pin<n, C>::is_input_pin()
{ return (PORT()->DIR & bitmask()) ==  0; }


template <uint8_t n, typename C>
bool Pin<n, C>::is_input_with_pullup_pin()
{ 
    return (is_input_pin() and
	   (PORT()->PIN2CTRL & pullupen_mask()) != 0);
}


template <uint8_t n, typename C>
bool Pin<n, C>::is_input_without_pullup_pin()
{
    return (is_input_pin() and
	   (PORT()->PIN2CTRL & pullupen_mask()) == 0);
}


template <uint8_t n, typename C>
inline bool Pin<n, C>::read()
{return PORT()->IN & bitmask();}

template <uint8_t n, typename C>
inline bool Pin<n, C>::is_zero()
{return (PORT()->IN & bitmask()) == 0;}

template <uint8_t n, typename C>
inline bool Pin<n, C>::is_one() 
{return (PORT()->IN & bitmask()) != 0;}

template <uint8_t n, typename C>
inline void Pin<n, C>::write_one()	
{ PORT()->OUTSET = bitmask(); }

template <uint8_t n, typename C>
inline void Pin<n, C>::write_zero()	
{ PORT()->OUTCLR = bitmask(); }

template <uint8_t n, typename C>
inline void Pin<n, C>::toggle()  
{ PORT()->OUTTGL = bitmask(); }

template <uint8_t n, typename C>
inline void Pin<n, C>::write(uint8_t x)
{
    if(x) write_one();
    else  write_zero();
}


//// Interrupts
//// ----------
///TODO: cambiar. Esto es copia de atmega32
//template<>
//inline void Pin<2>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT16>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<2>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT16>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<3>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT17>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<3>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT17>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<4>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT18>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<4>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT18>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<5>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT19>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<5>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT19>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<6>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT20>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<6>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT20>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<9>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT6>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<9>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT6>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<10>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT7>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<10>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT7>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<11>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT21>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<11>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT21>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<12>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT22>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<12>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT22>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<13>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE2>::to<1>::in(PCICR);
//    atd::write_bit<PCINT23>::to<1>::in(PCMSK2);
//}
//
//template<>
//inline void Pin<13>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT23>::to<0>::in(PCMSK2); }
//
//
//template<>
//inline void Pin<14>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT0>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<14>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT0>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<15>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT1>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<15>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT1>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<16>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT2>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<16>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT2>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<17>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT3>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<17>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT3>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<18>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT4>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<18>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT4>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<19>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE0>::to<1>::in(PCICR);
//    atd::write_bit<PCINT5>::to<1>::in(PCMSK0);
//}
//
//template<>
//inline void Pin<19>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT5>::to<0>::in(PCMSK0); }
//
//
//template<>
//inline void Pin<23>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT8>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<23>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT8>::to<0>::in(PCMSK1); }
//
//
//template<>
//inline void Pin<24>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT9>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<24>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT9>::to<0>::in(PCMSK1); }
//
//
//template<>
//inline void Pin<25>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT10>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<25>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT10>::to<0>::in(PCMSK1); }
//
//
//template<>
//inline void Pin<26>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT11>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<26>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT11>::to<0>::in(PCMSK1); }
//
//
//template<>
//inline void Pin<27>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT12>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<27>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT12>::to<0>::in(PCMSK1); }
//
//
//template<>
//inline void Pin<28>::enable_change_level_interrupt()
//{
//    atd::write_bit<PCIE1>::to<1>::in(PCICR);
//    atd::write_bit<PCINT13>::to<1>::in(PCMSK1);
//}
//
//template<>
//inline void Pin<28>::disable_change_level_interrupt()
//{ atd::write_bit<PCINT13>::to<0>::in(PCMSK1); }
//
//
//
//// Pin_connection
//// --------------
//// De momento voy a usar el número de pin en `hwd` para identificar si se
//// trata de un pin or a connection.
//template <uint8_t n>
//class Pin_connection{
//public:
//    static constexpr uint8_t number = n;
//
//    static constexpr bool is_a_valid_pin()
//    {return Pin<n>::is_a_valid_pin();}
//};
//
} // namespace private_


template <uint8_t n, typename Cfg>
using Pin = mcu::Pin<private_::Pin<n, Cfg>>::type;

}// namespace mega0_


#endif


