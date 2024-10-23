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

#pragma once

#ifndef __AVR_SLEEP_H__
#define __AVR_SLEEP_H__
/****************************************************************************
 *
 *  DESCRIPCION
 *    Funciones para dormir el avr
 *
 *  NOTAS
 *    Al principio iba a aprovechar las funciones de avr-libc para implementar
 *    esta clase, pero no está nada claro su uso (no están documentados las
 *    macros que hay que usar para definir el sleep mode). Opto por
 *    experimentar y crear un traductor. A fin de cuentas esto es un proyecto
 *    de aprendizaje.
 *
 *  HISTORIA:
 *    Manuel Perez
 *    01/01/2020 v0.0
 *    02/09/2023 Reescrito.
 *
 ****************************************************************************/
#include <avr/io.h> 
#include <atd_bit.h>

namespace mega_{
// ----------------
// Traductor básico 
// ----------------
/// Si a_dormir == true duerme el avr, cambiando el valor de a_dormir = false.
/// postcondition: a_dormir == false; (salvo que se cambie en alguna
/// interrupción)
/// precondition: Se ha llamado a set_sleep_mode antes. <-- no olvidarlo.
// Devuelve si el avr se ha dormido o no.
//bool sleep_if(volatile bool& to_sleep);

// De momento defino este interfaz para no tener que usar abreviaturas
struct Sleep{
// Types
    // (RRR) Mode vs mode: 
    //	     Se lee mejor `Sleep::mode::idle` que `Sleep::Mode::idle`
    enum class mode {idle, 
		ADC_noise_reduction, 
		power_down, power_save,
		standby, extended_standby
    };

// MODOS DE OPERACION
    static void idle_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<0,0,0>::in(SMCR);}

    static void ADC_noise_reduction_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<0,0,1>::in(SMCR);}

    static void power_down_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<0,1,0>::in(SMCR);}

    static void power_save_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<0,1,1>::in(SMCR);}

//    static void reserved_mode()
//    {atd::write_bits<SM2, SM1, SM0>::to<1,0,0>::in(SMCR);}
//    {atd::write_bits<SM2, SM1, SM0>::to<1,0,1>::in(SMCR);}

    static void standby_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<1,1,0>::in(SMCR);}

    static void extended_standby_mode()
    {atd::write_bits<SM2, SM1, SM0>::to<1,1,1>::in(SMCR);}


// SLEEP INSTRUCTION
    // El `volatile` del asm indica al compilador que bajo ningún concepto
    // al optimizar borre esta instrucción
    //
    // (???) La definición de los de avr-libc de esta función es rara. Es su
    // `sleep_cpu` y ejecutan
    //	    asm volatile("sleep" "\n\t" ::); 
    //	¿por qué el \n\t? ¿estética para que se pueda leer el asm generado y
    //	poder depurar? 
    static void instruction()
    {asm volatile ("sleep");}


// SLEEP ENABLE
    // Datasheet (register SMCR)
    // To avoid the MCU entering the sleep mode unless it is the programmer’s 
    // purpose, it is recommended to write the Sleep Enable (SE) bit to one 
    // just before the execution of the SLEEP instruction and to clear it 
    // immediately after waking up.
    //
    // Esto es, recomiendan:
    //	    Sleep::enable();
    //	    Sleep::sleep();
    //	    Sleep::disable();
    static void enable()
    {atd::write_bit<SE>::to<1>::in(SMCR);}

    static void disable()
    {atd::write_bit<SE>::to<0>::in(SMCR);}


// TODO: MCU control register
// TODO: Power reduction register. 

};


// Definimos el modo del sleep
void sleep_mode(Sleep::mode mode);

// Dormimos. 
// Precondicion: haber definido el modo con `sleep_mode`.
void sleep();

// Equivalente a:
//	    sleep_mode(mode);
//	    sleep();
void sleep(Sleep::mode mode);

}// namespace

#endif


