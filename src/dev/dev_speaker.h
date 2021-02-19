// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __DEV_SPEAKER_H__
#define __DEV_SPEAKER_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Speaker driver
 *
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    16/02/2021 v0.0
 *
 ****************************************************************************/

#include "dev_signal_generator.h"


namespace dev{
/*!
 *  \brief  Funciones básicas para manejar un altavoz.
 *
 */
class Speaker{
public:
    void play_note(uint16_t nota, uint16_t duracion_en_ms);


};


inline void Speaker::play_note(uint16_t nota, uint16_t duracion /* en ms */) 
{
    avr::Timer1::reset();
    avr::Timer1::comparador1(nota);

    // conectamos el speaker al timer0
    pin_.as_output();

    while (duracion) {
	_delay_ms(1);
	--duracion;
    }

    // desconectamos el speaker al timer0
    pin_.as_input_without_pullup();
}




/***************************************************************************
 *			ALGUNOS SONIDOS DE INTERES
 ***************************************************************************/
//--------------------------------------------------------------------------
// Si se quieren hace alarmas o sirenas probar con las siguientes:
//
//	sirena01(speaker, 2000, 4000, 10);
//	sirena02(speaker, 400, 800, 500);
//	sirena02(speaker, 400, 800, 65);
//	alarma01(speaker, 3000, 50);
//
//  Para jugar, probar en test_speaker02.cpp
//--------------------------------------------------------------------------

//
//
//// Precondición: frecuencia1 <= frecuencia2
//// En esta sirena vamos de forma continua de frecuencia1 a frecuencia2
//// y luego volvemos de forma continua de frecuencia2 a frecuencia1.
//inline void sirena01(Speaker& speaker
//	    , uint16_t frecuencia1
//	    , uint16_t frecuencia2
//	    , uint16_t duracion /* en ms */)
//{
//    auto nota1 = Speaker::nota(frecuencia1);
//    auto nota2 = Speaker::nota(frecuencia2);
//
//    for(uint16_t i = nota2; i <= nota1; ++i)
//	speaker.play_note(i, duracion);
//
//    for(uint16_t i = nota1; i >= nota2; --i)
//	speaker.play_note(i, duracion);
//}
//
//
//// En esta sirena nos limitamos a alternar las dos frecuencias
//inline void sirena03(Speaker& speaker
//	    , uint16_t frecuencia1
//	    , uint16_t frecuencia2
//	    , uint16_t duracion1 /* en ms */
//	    , uint16_t duracion2 /* en ms */)
//{
//    auto nota1 = Speaker::nota(frecuencia1);
//    auto nota2 = Speaker::nota(frecuencia2);
//
//    speaker.play_note(nota1, duracion1);
//    speaker.play_note(nota2, duracion2);
//
//}
//
//
//// En esta sirena nos limitamos a alternar las dos frecuencias
//inline void sirena02(Speaker& speaker
//	    , uint16_t frecuencia1
//	    , uint16_t frecuencia2
//	    , uint16_t duracion /* en ms */)
//{
//    sirena03(speaker, frecuencia1, frecuencia2, duracion, duracion);
//
//}
//
//inline void alarma01(Speaker& speaker, uint16_t frecuencia, uint16_t duracion)
//{
//    speaker.play_note(Speaker::nota(frecuencia), duracion);
//    avr::wait_ms(duracion);
//}
//


}// namespace


#endif

