// Copyright (C) 2023 Manuel Perez 
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

#ifndef __DEV_ONE_WIRE_H__
#define __DEV_ONE_WIRE_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Implementación del protocolo 1-Wire de Maxim
 *
 * REFERENCES
 *    App note 126: 1-wire communication through software
 *    App note 128: 1-wire search algorithm
 *    Datasheet of DS18B20
 *
 * NOTA
 *    De momento solo se implementa el modo donde los dispositivos 1-wire
 *    conectados estén conectados a VCC (no usan el parasite power mode).
 *    Si se quiere usar ese modo hay que modificar la implementación.
 *
 * HISTORIA
 *    Manuel Perez
 *    23/08/2023 v0.0 Lo escribo de acuerdo a la explicación dada en la
 *		      datasheet del DS18B20.
 *
 *
 ****************************************************************************/

#include <dev_interrupts.h>

namespace dev{


template <typename Micro0,
	  typename Pin0,
	  typename Miniclock_us0>
struct One_wire_cfg{
    using Micro = Micro0;		
    using Pin	= Pin0;			
    using Miniclock_us = Miniclock_us0; 
};

// Pin: pin que usamos para comunicarnos con los dispositivos one-wire.
//      Es de entrada-salida.
template <typename Cfg>
class One_wire{
public:
    using Micro	    = typename Cfg::Micro;
    using Pin	    = typename Cfg::Pin;
//    using Miniclock = typename Cfg::Miniclock;

// Transaction sequence
// Step 1: Initializacion
    // Devuelve true si ha detectado slave presentes, false en caso contrario.
    bool reset();

// Step 2: ROM Commands
// quién está conectado? 
    search_rom

// dime tú nombre (address)? (cuando solo hay un slave conectado)
    read_rom

// Atención: quiero hablar con 
    match_rom

// Atención todo el mundo!
    skip_rom

// quién tiene el `alarm flag` set?
    alarm_search


// Step 3: Function Commands

// Funciones de escritura/lectura de bytes
    void write(uint8_t x);
    uint8_t read();	    // devuelve el byte leido

// Los del DS18B20 son
// convert_T
// write_scratchpad
// read_scratchpad
// copy_scratchpad
// recall_e2
// read_power_supply

    
private:
//  Table 2.1 of App note 126
//  Valores standard de tiempos usados en el protocolo
//  (como estoy implementando un traductor me limito a escribir en código la
//  tabla, pero podían haber elegido nombres más descriptivos)
    constexpr uint8_t A_us =  6;    // Arduino toma 10 us
    constexpr uint8_t B_us =  64;   //    "         55 us
    constexpr uint8_t C_us =  60;   //    "         65 us
    constexpr uint8_t D_us =  10;   //    "          5 us
    constexpr uint8_t E_us =  9;
    constexpr uint8_t F_us =  55;
    constexpr uint8_t G_us =  0;
    constexpr uint8_t H_us =  480;
    constexpr uint8_t I_us =  70;
    constexpr uint8_t J_us =  410;

// Funciones de escritura/lectura de bits
// Estas funciones no son responsables de:
//	+ desactivar las interrupciones
//	+ ni de dejar el micro en tristate o en output.
    // Escribe 0 si x == 0, 1 en caso contrario.
    void write_bit(uint8_t x);

    void write_bit_zero();
    void write_bit_one();

    // Devuelve `0` si el bit leído es 0, y `1` si el bit leído es 1.
    uint8_t read_bit();


};


//  datasheet of DS18B20: pag 15
//  1. El master escribe 0 mínimo durante 480 us
//  2. El sensor espera de 15 a 60 us.
//  3. El sensor escribe 0 por un periodo de 60 a 240 us.
//
//  Para calcular la ventana en la que tenemos que leer si el sensor ha
//  respondido con un 0, miremos los casos posibles:
//  I) El sensor responde a los 15 us. Como el pulso que genera puede durar de
//     60 a 240 us, estará a 0 de 75 a 255 us.
//  II) El sensor responde a los 60 us ==> estará a 0 de 120 a 300 us.
//
//  Luego el sensor puede poner el pin a 0 desde los 15 hasta los 300 us. That
//  has to be our sampling window. 
//
//  Otra forma de hacerlo es usar la ventana más pequeña de 60 a 75 us que
//  fijo tiene que estar a cero (si empieza el pulso del sensor a los 15 us +
//  60 us minimo = 75 us, tiene que estar a 0 de 15 a 75 us; pero si empieza
//  el pulso a los 60 us, sabemos que de 60 en adelante está a 0 ==> de 60 a
//  75 us fijo que tiene que estar a 0).
//
//  Y otra forma de hacerlo es muestrearlo a los 68 us (que es el punto medio
//  de 60 a 75 us).
//
//  Conclusión: tenemos 3 formas de mirar si el sensor nos ha respondido o no.
//  1) Totalmente segura: muestrear de 15 a 300 us.
//  2) Segura: muestrear de 60 a 75 us.
//  3) Solo hacer un muestreo a los 68 us.
//
//  La app note 126 opta por la tercera opción, haciendo el muestreo a los
//  I_us = 70 us.
//
//  La siguiente implementación es similar a la dada por la app note 126, pag.
//  5 (función OWTouchReset).
template <typename C>
bool One_wire::reset()
{
    Disable_interrupts<Micro> lock;

// write_zero_in<Pin>_for(480)_us:
    wait_us(G_us);
    Pin::as_output();
    Pin::write_zero();
    Micro::wait_us(H_us);

    Pin::as_input_without_pullup(); // release the bus

    Micro::wait_us(I_us);
    bool res = Pin::is_zero();

    wait_us(J_us);  // <-- esto no aparece claro en la datasheet del DS1820

    return res;

}


// See: "App note 128: 1-wire search algorithm"
template <typename C>
void One_wire::search_rom()
{
    write(0xF0);
    AQUI
}


// Copia de la app note 126
// precondition: Pin como de salida.
template <typename C>
void One_wire::write_bit_one()
{
    Pin::write_zero();
    wait_us(A_us);

    Pin::write_one();
    wait_us(B_us);
}



// Copia de la app note 126
// precondition: Pin como de salida.
template <typename C>
void One_wire::write_bit_zero()
{
    Pin::write_zero();
    wait_us(C_us);

    Pin::write_one();
    wait_us(D_us);
}

template <typename C>
inline void One_wire::write_bit(uint8_t x)
{
    if (x) 
	write_bit_one();

    else 
	write_bit_zero();
}



// Copia de la app note 126
template <typename C>
uint8_t One_wire::read_bit()
{
    Pin::as_output();
    Pin::write_zero();
    
    wait_us(A_us);

    Pin::as_input_without_pullup(); // release the bus
    
    wait_us(E_us);

    uint8_t res = 0x00;

    if (Pin::is_one())
	res = 0x01;

    wait_us(F_us);

    return res;
}


// Copia de la app note 126. 
// Observar que se envía primero el LSb.
//
// NOTA: los de Arduino comentan que dependiendo de si se 
// conectan los dispositivos en parasite power mode, no liberar el bus.
// Pero en ese modo, según la datasheet del DS1820 hay que controlar más
// tiempos y es más complicado. Por ello yo libero el bus.
template <typename C>
void One_wire::write(uint8_t x)
{
    Disable_interrupts<Micro> lock;

    Pin::as_output();

    for (uint8_t i = 0; i < 8; ++i){
	write_bit(x & 0x01);
	x >>= 1;
    }

    Pin::as_input_without_pullup(); // release the bus
}


// Copia de la app note 126. 
// Observar que se envía primero el LSb.
template <typename C>
uint8_t One_wire::read()
{
    Disable_interrupts<Micro> lock;

    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; ++i){
	res >>= 1;

	if (read_bit())
	    res |= 0x80; // atd::write_bit<7>::to<1>::in(res);
    }

    return res;

}


}// namespace


#endif


