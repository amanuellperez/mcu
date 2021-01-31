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

#ifndef __DEV_SPEAKER_H__
#define __DEV_SPEAKER_H__

#include <avr_timers.h>
#include <avr_pin.h>


namespace dev{
/*!
 *  \brief  Funciones básicas para manejar un altavoz.
 *
 *  Este speaker solo se puede conectar a las salidas del generador de ondas
 *  del timer seleccionado. En caso de que se intente conectar a un pin 
 *  diferente, dará un error de compilación.
 *
 */
class Speaker{
public:
//    using Timer1 = Timer1_16bits;

    /// Creamos un nuevo Speaker
    // Tengo que crearlo de esta forma ya que no puedo definir el constructor
    // como una template parametrizada por un uint8_t. Para evitar que el
    // usuario de la clase intente crear un Speaker sin usar esta función
    // defino el constructor como privado.
    template<uint8_t num_pin>
    static Speaker nuevo();


    /// Toca una nota de forma secuencial (esto es, esta función no
    /// devuelve el control hasta que no hayan transcurrido 'duracion' ms)
    /// TODO: La 'nota' no es realmente la nota, aunque sí que corresponde
    /// con la nota que queremos tocar. Es
    /// el OCR que tenemos que meterle al timer!!!
    /// Se calcula a partir de la fórmula de la frecuencia del
    /// timer:
    ///	    fs = f/(2d(x+1))
    ///	    fs = frecuencia que queremos generar
    ///	    f = frecuencia del microcontrolador
    ///	    d = división del clock (preescaler)
    ///	    x = lo que aquí llamo nota.
    ///  Luego x se calcula como: 
    ///		x = f/(2dfs) - 1
    /// Si el timer es de 16 bits no es necesario preescalar el reloj del
    /// mcu, pudiendo tomar d = 1. (De hecho eso es lo que estamos haciendo)
    /// (mirar nuevo())
    void play_note(uint16_t nota, uint16_t duracion_en_ms);


    /// Devuelve la nota que le corresponde a la frecuencia f_s
    template<uint32_t f_s>
    static uint16_t nota()
    {
	return static_cast<uint16_t>(avr::Clock::frecuencia_en_Hz()/(2*f_s)-1);
    }

    /// Devuelve la nota que le corresponde a la frecuencia f_s.
    /// Esta versión dinámica es para experimentar o generar ficheros con 
    /// las notas de una canción. No está pensada para ser usada en un 
    /// programa real.
    static uint16_t nota(uint32_t f_s)
    {
	return uint16_t(avr::Clock::frecuencia_en_Hz()/(2*f_s)-1);
    }

private:
    /// Definimos la conexión del hardware y lo inicializamos
    explicit Speaker(uint8_t num_pin):pin_{num_pin}{}

    // Datos
    avr::Pin pin_;
};


template<uint8_t num_pin>
inline Speaker Speaker::nuevo()
{
    avr::Timer1::square_wave_init<num_pin>(avr::Clock::Divisor::no_prescaling);

    return Speaker{num_pin};
}


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

