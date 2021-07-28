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

#pragma once

#ifndef __AVR_UART_BAUD_RATE_H__
#define __AVR_UART_BAUD_RATE_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Calculamos el valor del UBBRn y el tipo de velocidad
 *	(normal mode o double speed) a partir de la frecuencia de reloj usada
 *	y del baud rate que queremos.
 *
 *   - COMENTARIOS: avrlibc suministra util/setbaud.h. Básicamente aquí hago
 *	lo mismo que allí con 2 diferencias fundamentales:
 *	    1.- No uso macros, solo constexpr.
 *	    2.- setbaud.h selecciona la velocidad definiendo la variable
 *	    USE_2X lo cual es una forma muy oscura de razonar. Prefiero llamar
 *	    a una función UBBRn() que me devuelve UBBR y el modo al que
 *	    configurar el UART.
 *
 *   - HISTORIA:
 *           A.Manuel L.Perez- 27/08/2019 v0.0
 *
 ****************************************************************************/
#include <atd_cast.h>
#include <atd_type_traits.h>
#include <cstdint>
#include <utility>

namespace avr{
// Modos de funcionamento del UART
enum class UART_mode{
    normal, 
    double_speed
    // TODO: synchronous_master
};

// Calculamos el UBBR que corresponde al normal mode para esta frecuencia
// de reloj y el baud rate dado. Usamos la tabla tabla 24.1 (pag. 227) de 
// la datasheet.
template <uint32_t f_clock, uint32_t baud_rate, uint32_t tolerance = 2>
struct UART_normal_mode{
    // Si se produce overflow generamos UBBRn = 0 y dentro_tolerancia = false.
    static constexpr bool overflow = f_clock > 16*baud_rate? 1:0;

    static constexpr uint32_t UBBRn32 = 
	(f_clock -8u*baud_rate)/(16u*baud_rate) * overflow;

    static constexpr uint16_t UBBRn =
		    atd::safe_static_cast<uint16_t, uint32_t, UBBRn32>();

    static constexpr bool dentro_tolerancia
	= atd::Pertenece_al_intervalo_cerrado<uint32_t, 
        100u * f_clock * overflow, 
	(16u * baud_rate * (UBBRn + 1) * (100 - tolerance)),
	(16u * baud_rate * (UBBRn + 1) * (100 + tolerance))>::value;
};


// Calculamos el UBBR que corresponde al double speed mode para esta frecuencia
// de reloj y el baud rate dado. Usamos la tabla tabla 24.1 (pag. 227) de 
// la datasheet.
template <uint32_t f_clock, uint32_t baud_rate, uint32_t tolerance = 2>
struct UART_double_speed_mode{
    // Si se produce overflow generamos UBBRn = 0 y dentro_tolerancia = false.
    static constexpr bool overflow = f_clock > 8*baud_rate? 1:0;

    static constexpr uint32_t UBBRn32 = 
	(f_clock -4u*baud_rate)/(8u*baud_rate) * overflow;

    static constexpr uint16_t UBBRn =
		    atd::safe_static_cast<uint16_t, uint32_t, UBBRn32>();

    static constexpr bool dentro_tolerancia
	= atd::Pertenece_al_intervalo_cerrado<uint32_t, 
        100u * f_clock * overflow, 
	(8u * baud_rate * (UBBRn + 1) * (100 - tolerance)),
	(8u * baud_rate * (UBBRn + 1) * (100 + tolerance))>::value;
};



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
template <uint32_t f_clock, uint32_t baud_rate, uint32_t tolerance = 2>
constexpr inline std::pair<UART_mode, uint32_t> UBBRn()
{
    if constexpr (UART_normal_mode<f_clock, baud_rate, tolerance>::dentro_tolerancia)
	return {UART_mode::normal, 
			    UART_normal_mode<f_clock, baud_rate, tolerance>::UBBRn};

    if constexpr (UART_double_speed_mode<f_clock, baud_rate, tolerance>::dentro_tolerancia)
	return {UART_mode::double_speed, 
			UART_double_speed_mode<f_clock, baud_rate, tolerance>::UBBRn};

    else // no se puede poner false directamente, de ahi el baud_rate-baud_rate
	static_assert(
        baud_rate - baud_rate, "\nUBBRn: Es imposible conseguir el baud_rate indicado con la frecuencia "
              "de reloj dada. Seguramente tengas que incrementar la frecuencia del reloj.");

    // Aqui nunca llega. Pongo el return para que no genere el warning el
    // compilador.
    return {UART_mode::double_speed, 0};
}

}// namespace

#endif
