// Copyright (C) 2019-2020 Manuel Perez 
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

#ifndef __MEGA_UART_BAUD_RATE_H__
#define __MEGA_UART_BAUD_RATE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *  Calculamos el valor del UBBRn y el tipo de velocidad
 *  (normal mode o double speed) a partir de la frecuencia de reloj usada
 *  y del baud rate que queremos.
 *
 *  REFERENCIAS
 *	Datasheet atmega328p: pag 227.
 *	App. note AVR306.
 *
 *  HISTORIA
 *	Manuel Perez
 *	27/08/2019 v0.0
 *	07/12/2023 Lo reescribo todo.
 *
 ****************************************************************************/

#include <atd_cast.h>
#include <atd_type_traits.h>
#include <cstdint>
#include <utility>

namespace mega_{
// Modos de funcionamento del UART
enum class UART_mode{
    normal, 
    double_speed
    // TODO: synchronous_master
};

namespace impl_of{

// La tabla 24.1 (pag. 227) de la datasheet muestra cómo calcular 
// el valor de UBRR.
inline
constexpr uint32_t baud_rate_normal_mode(uint32_t f_clock, uint32_t UBBR)
{ return f_clock / (16 * (UBBR + 1)); }

inline
constexpr uint32_t baud_rate_double_speed_mode(uint32_t f_clock, uint32_t UBBR)
{ return f_clock / (8 * (UBBR + 1)); }

// Nombre tan largo para que no se llame nunca
inline 
constexpr int8_t calculate_percentage_error(uint32_t b, uint32_t baud_rate)
{
    if (b >= baud_rate)
	return ((b - baud_rate) * 100) / baud_rate;

    else
	return ((baud_rate - b) * 100) / baud_rate;
}

struct UBBR_and_error{
    uint16_t UBBR;
    int8_t error;
};

// Return: [UBBRn, % de error como número]
// Devuelve 100% de error en caso de que no se pueda calcular.
inline
constexpr UBBR_and_error
    UBBR_and_error_normal_mode(uint32_t f_clock, uint32_t baud_rate)
{
    if (f_clock < 16 * baud_rate) // overflow
	return {0, 100};

    uint32_t u0 = (f_clock -16u*baud_rate)/(16u*baud_rate); // UBBR
    uint32_t u1 = u0 + 1;
    
    uint32_t b0 = baud_rate_normal_mode(f_clock, u0); // baud_rate_closest_match
    uint32_t b1 = baud_rate_normal_mode(f_clock, u1);

    int8_t e0 = calculate_percentage_error(b0, baud_rate);
    int8_t e1 = calculate_percentage_error(b1, baud_rate);

    if (atd::abs(e0) <= atd::abs(e1)) // Elegimos el que menor error tenga
	return {static_cast<uint16_t>(u0), e0};

    else 
	return {static_cast<uint16_t>(u1), e1};
}


// Return: [UBBRn, % de error como número]
// Devuelve 100% de error en caso de que no se pueda calcular.
inline
constexpr UBBR_and_error
    UBBR_and_error_double_speed_mode(uint32_t f_clock, uint32_t baud_rate)
{
    if (f_clock < 16 * baud_rate) // overflow
	return {0, 100};

    uint32_t u0 = (f_clock -8u*baud_rate)/(8u*baud_rate); // UBBR
    uint32_t u1 = u0 + 1;
    
    uint32_t b0 = baud_rate_double_speed_mode(f_clock, u0); // baud_rate_closest_match
    uint32_t b1 = baud_rate_double_speed_mode(f_clock, u1);

    int8_t e0 = calculate_percentage_error(b0, baud_rate);
    int8_t e1 = calculate_percentage_error(b1, baud_rate);

    if (atd::abs(e0) <= atd::abs(e1)) // Elegimos el que menor error tenga
	return {static_cast<uint16_t>(u0), e0};

    else 
	return {static_cast<uint16_t>(u1), e1};
}






}// impl_of



// La tabla 24.1 (pag. 227) de la datasheet muestra cómo calcular 
// el valor de UBRR.
//
// Ejemplo:
//	f_clock = 1MHz, baud_rate = 9600
//	==> UBBR = f_clock/(16baud_rate) - 1 = 6
//
//	El problema con este 6 es que el baud_rate real que estamos generando 
//	es igual a
//	    baud_rate_real = f_clock/(16(UBBR + 1)) = 8929
//	un 7% más pequeño del baud_rate 9600 que queríamos generar.
//
//	Si en lugar de usar normal_mode usamos double_speed_mode obtenemos
//	==> UBBR = f_clock/(8baud_rate) - 1 = 12
//	que genera un baud_rate_real
//	    baud_rate_real = f_clock/(8(UBBR + 1)) = 9615
//	un 2 por mil de diferencia respecto del buscado.
//	
//
//  Como se ve con el ejemplo no siempre va a ser interesante usar el
//  normal_mode, por eso, incluimos una tolerancia. Si el valor calculado con 
//  el normal_mode difiere demasiado del valor buscado, probamos con
//  double_speed_mode.


/// Calcula el valor del UBBR que hay que usar para generar el baud_rate
/// indicado usando la frecuencia del reloj f_clock. Devuelve también el
/// modo (normal o double). Esta función es la básica para configurar
/// el baud_speed del UART.
// La llamo UBBRn y no UBBR por si acaso los de avrlibc definen alguna macro
// llamada UBBR.
template <uint32_t f_clock, uint32_t baud_rate, uint32_t max_error = 2>
constexpr inline std::pair<UART_mode, uint32_t> UBBRn()
{
    constexpr impl_of::UBBR_and_error normal = 
		    impl_of::UBBR_and_error_normal_mode(f_clock, baud_rate);

    constexpr impl_of::UBBR_and_error double_speed = 
		    impl_of::UBBR_and_error_double_speed_mode(f_clock, baud_rate);

    if constexpr (normal.error <= double_speed.error){
	if constexpr (normal.error <= max_error)
	    return {UART_mode::normal, normal.UBBR};

	else
	static_assert(
        max_error - max_error,
	    "\nUBBRn: Can't achieve that baud_rate with so small max_error."
              "Increment max_error or try to increase the frequency F_CPU.");
    }

    else {
	if constexpr (double_speed.error <= max_error)
	    return {UART_mode::double_speed, double_speed.UBBR};
	else
	static_assert(
        max_error - max_error,
	    "\nUBBRn: Can't achieve that baud_rate with so small max_error."
              "Increment max_error or try to increase the frequency F_CPU.");
    }

    // Aqui nunca llega. Pongo el return para que no genere el warning el
    // compilador.
    return {UART_mode::double_speed, 0};
}

}// namespace

#endif
