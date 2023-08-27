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
 *    App note 187 (ó 128?): 1-wire search algorithm
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

#include <stdint.h>	    // uint8_t
#include <dev_interrupt.h>  // Disable_interrupts


namespace dev{


template <typename Micro0,
	  typename Pin0>
struct One_wire_cfg{
    using Micro = Micro0;		
    using Pin	= Pin0;			
};

// Pin: pin que usamos para comunicarnos con los dispositivos one-wire.
//      Es de entrada-salida.
template <typename Cfg>
class One_wire{
public:
    using Micro	    = typename Cfg::Micro;
//    using Miniclock = typename Cfg::Miniclock;
    static constexpr uint8_t ROM_size = 8;

// Transaction sequence
// Step 1: Initializacion
    // Devuelve true si ha detectado slave presentes, false en caso contrario.
    static bool reset();

// Step 2: ROM Commands
//// quién está conectado? 
//    search_rom
//
//// dime tú nombre (address)? (cuando solo hay un slave conectado)
//    read_rom
//
//// Atención: quiero hablar con 
    static void match_rom(const uint8_t ROM[ROM_size]);

    // La datasheet lo llama match_rom, pero un nombre más natural es el de
    // `select`
    static void select(const uint8_t ROM[ROM_size]) { match_rom(ROM); }
//
//// Atención todo el mundo!
    static void skip_rom() {write(0xCC);}
//
//// quién tiene el `alarm flag` set?
//    alarm_search
//

// Step 3: Function Commands

// Funciones de escritura/lectura de bits
    // Escribe 0 si x == 0, 1 en caso contrario.
    static void write_bit(uint8_t x);

    static void write_bit_zero();
    static void write_bit_one();

    // Devuelve `0` si el bit leído es 0, y `1` si el bit leído es 1.
    static uint8_t read_bit();

// Funciones de escritura/lectura de bytes
    static void write(uint8_t x);
    static uint8_t read();	    // devuelve el byte leido

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
//    static constexpr uint8_t A_us =  6;    // Arduino toma 10 us
    // La app note 126 tiene un valor de 6 us; los de Arduino toman 10 us.
    // La datasheet del DS18B20 indica que para escribir un 1 como mucho tiene
    // que estar 15 us low. Al medir (ver test) el valor de A_us = 3 me genera
    // un pulso LOW un pelín menos de 15 us (recordar que el reloj del atmega
    // es 8MHz +- 10%)
    static constexpr uint8_t A_us =  3;    // Arduino toma 10 us
    static constexpr uint8_t B_us =  64;   //    "         55 us
    static constexpr uint8_t C_us =  60;   //    "         65 us
    static constexpr uint8_t D_us =  10;   //    "          5 us
    // Los tiempos recomendados no funcionan para el read_bit.
    // Ver los comentarios de la función read_bit (o pag 16 datasheet DS18B20)
    static constexpr uint8_t E_us =  4;	    // recomendado 9 us
    static constexpr uint8_t F_us =  55;
    static constexpr uint8_t G_us =  0;
    static constexpr uint16_t H_us =  480;
    static constexpr uint8_t I_us =  70;
    static constexpr uint16_t J_us =  410;



// HELPER FUNCTIONS
    static void pin_write_zero();
    static void pin_release_the_bus(); // = pin_write_one()
    static bool pin_is_one();
    static bool pin_is_zero() {return !pin_is_one();}

};

template <typename Cfg>
inline void One_wire<Cfg>::pin_release_the_bus()
{
    using Pin = typename Cfg::Pin;

    Pin::as_input_without_pullup(); // release the bus
}


template <typename Cfg>
inline void One_wire<Cfg>::pin_write_zero()
{
    using Pin = typename Cfg::Pin;

    Pin::write_zero();
    Pin::as_output();
}

template <typename Cfg>
inline bool One_wire<Cfg>::pin_is_one()
{
    using Pin = typename Cfg::Pin;

    return Pin::is_one();
}



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
//
// DUDA: los de Arduino están continuamente activando/desactivando las
// interrupciones. Por ejemplo, cuando tienen que esperar los 480 us los
// espeeran con las interrupciones desactivadas. Tiene la ventaja de que si
// salta una interrupción se le atiende inmediatamente. ¿Mejor hacerlo así?
// Como es la primera implementación voy a lo más básico y me olvido de esto.
template <typename C>
bool One_wire<C>::reset()
{
    Disable_interrupts<Micro> lock;

    Micro::wait_us(G_us);
    pin_write_zero();
    Micro::wait_us(H_us);

    pin_release_the_bus();

    Micro::wait_us(I_us);
    bool res = pin_is_zero();

    Micro::wait_us(J_us); 

    return res;

}


template <typename C>
void One_wire<C>::match_rom(const uint8_t ROM[8])
{
    write(0x55);

    for (uint8_t i = 0; i < 8; ++i)
	write(ROM[i]);
}


// Copia de la app note 126
// precondition: Pin como de salida.
template <typename C>
void One_wire<C>::write_bit_one()
{
    Disable_interrupts<Micro> lock;

    pin_write_zero();
    Micro::wait_us(A_us);

    pin_release_the_bus();
    // DUDA: aqui los de arduino activan las interrupciones
    Micro::wait_us(B_us);
}



// Copia de la app note 126
// precondition: Pin como de salida.
template <typename C>
void One_wire<C>::write_bit_zero()
{
    Disable_interrupts<Micro> lock;

    pin_write_zero();
    Micro::wait_us(C_us);

    pin_release_the_bus();
    // DUDA: aqui los de arduino activan las interrupciones
    Micro::wait_us(D_us);
}

template <typename C>
inline void One_wire<C>::write_bit(uint8_t x)
{
    if (x) 
	write_bit_one();

    else 
	write_bit_zero();
}



// Copia de la app note 126
// Según la app note del DS18B20, pag 16, al hablar de los "read time slots"
// dice:
//  "Output data from the DS18B20 is valid for 15us after the falling edge
//  that initiated the read time slot. Therefore
//  the master must release the bus and then sample the bus state WITHIN 15us
//  from the start of the slot"
//
//  Los tiempos recomendados por la app note 126 dan: A + E = 6 + 9 = 15 us
//  ==> nos pasamos de los 15 us cuando medimos el read!!! No debería de
//  funcionar!!!
template <typename C>
uint8_t One_wire<C>::read_bit()
{
    Disable_interrupts<Micro> lock;

    pin_write_zero();
    
    Micro::wait_us(A_us);

    pin_release_the_bus();
    
    Micro::wait_us(E_us);

    uint8_t res = pin_is_one();

    Micro::wait_us(F_us);

    if (res != 0) return 0x01;
    else          return 0x00;
    
}


// Copia de la app note 126. 
// Observar que se envía primero el LSb.
//
// NOTA: los de Arduino comentan que dependiendo de si se 
// conectan los dispositivos en parasite power mode, no liberar el bus.
// Pero en ese modo, según la datasheet del DS1820 hay que controlar más
// tiempos y es más complicado. Por ello yo libero el bus.
template <typename C>
void One_wire<C>::write(uint8_t x)
{
    for (uint8_t i = 0; i < 8; ++i){
	write_bit(x & 0x01);
	x >>= 1;
    }
}


// Copia de la app note 126. 
// Observar que se envía primero el LSb.
template <typename C>
uint8_t One_wire<C>::read()
{
    uint8_t res = 0;

    for (uint8_t i = 0; i < 8; ++i){
	res >>= 1;

	if (read_bit())
	    res |= 0x80; // atd::write_bit<7>::to<1>::in(res);
    }

    return res;

}


// One_wire_search
// ---------------
// TODO: clase temporal para probar el search. Convertirla en iteradores
// normales: 
//	for (auto p = One_wire::begin_search(); p != end_search(); ++p)
//	    ROM[i] = *p;

// (RRR) ¿por qué no implementarla como una vulgar función One_wire::search()?
//	 1. Para poder descomponer el algoritmo en partes.
//	 2. Tiene estado, y el estado son más de 8 bytes. Esta función se
//	    llama de año en año: esos bytes de estado solo deben de quedar
//	    reservados mientras se ejecuta search, después se tiene que
//	    liberar la memoria.
//
// Baso la implementación en el algoritmo dado en la app note 187 (es la misma
// implementación pero reestructurada)
//
// IMPORTANTE: la app note usa como índices de ROM el intervalo [1, 64].
// Observar que no empieza en 0. Eso lo hace para poder definir
// i_last_discrepancy = 0 para indicar que no hay discrepancias y que todo el
// algoritmo funcione. Revisar código o app note para entender lo que digo.
template <typename Cfg>
class One_wire_search{
public:
    using One_wire = dev::One_wire<Cfg>;
    static constexpr uint8_t ROM_size = One_wire::ROM_size;

    // Find the first devices on the 1-wire bus
    bool begin();
    bool next() {return search();}

    const uint8_t* value() const {return &ROM[0];}

private:


    // valor de la ROM del dispositivo que encontramos en la última llamada a
    // `search`
    uint8_t ROM[ROM_size]; 
	    
    // Indica la posición en que encontramos la última discrepancia igual a 0.
    // Cuando recorremos el diagrama de árbol cuando encontremos una
    // discrepancia (un nodo del árbol) vamos a seguir primero el camino
    // marcado por el 0. Si hemos seguido ese camino quiere decir que todavía
    // tenemos pendiente de seguir el otro camino: el marcado por el 1. Este
    // índice nos marca la posición en la que encontramos la última
    // discrepancia marcada como 0
    // CUIDADO: i_last_discrepancy = 0 ==> no hay discrepancias.
    uint8_t i_last_discrepancy;
    
    // indica si hemos llegado al último dispositivo (ya no hay más
    // dispositivos conectados al 1-wire)
    bool last_device;

// HELPER data
    uint8_t i_last_zero_;

// HELPER FUNCTIONS
    bool search();
    bool search_device();
    bool read_rom();
    bool read_bit_rom(uint8_t i, uint8_t i_rom, uint8_t rom_mask);
    uint8_t read_two_bits();
    uint8_t calculate_next_bit(uint8_t i, uint8_t i_rom, uint8_t rom_mask);
    uint8_t ROM_bit(uint8_t i, uint8_t mask); 
    void ROM_write(uint8_t i, uint8_t mask, uint8_t bit);

    
    void reset_state();
};


template <typename C>
void One_wire_search<C>::reset_state()
{
    i_last_discrepancy = 0;
    last_device = false;

    for (uint8_t i = 0; i < ROM_size; ++i)
	ROM[i] = 0;
}

template <typename C>
bool One_wire_search<C>::begin()
{
    reset_state();

    return search();
}


template <typename C>
bool One_wire_search<C>::search()
{
    bool search_result = false;	// se encontro device?
    i_last_zero_ = 0;
				
    if (!last_device)
	search_result = search_device();

    if (search_result){ // actualizamos el state
	i_last_discrepancy = i_last_zero_;

	if (i_last_discrepancy == 0)
	    last_device = true;
    }

    if (!search_result or !ROM[0]){ // ¿por qué miran también ROM[0]???
	reset_state();
	search_result = false;
    }


    return search_result;
}


template <typename C>
bool One_wire_search<C>::search_device()
{
    if (!One_wire::reset()){
	reset_state();
	return false; 
    }

    One_wire::write(0xF0);	// search command

    return read_rom();
}

// Tenemos que iterar por los 64 bits de la ROM.
// Dos posibles formas de hacerlo:
//  1. Definir ROM como uint64_t e iterar desde i = 0..63
//     El problema de usar el índice `i` es que para acceder al bit
//     correspondiente de la ROM hay que hacer (0x01 << i)
//  2. Definir ROM como array de 8 bytes e iterar usando dos parámetros:
//          i_rom     = nos dice el byte de la ROM en el que nos encontramos
//          rom_mask  = nos dice el bit dentro de ese byte.
//     Para iterar de esta forma hacemos el siguiente bucle:
//	    for (i_rom = 0 ... 7)
//	       for (rom_mask = 0x01; mask != 0; rom_mask <<= 1)
//		    Accedemos al bit dado por la mask de ROM[n].
//
//  Resulta que el algoritmo dado en la app note usa los dos métodos!!! 
//  La app note llama:
//	    id_bit_number = i	(de 1 a 64) <-- CUIDADO, empieza en 1 ya
//					que reserva el 0 para indicar que no
//					hay discrepancia.
//
//	    rom_byte_number = i_rom
//	    rom_byte_mask   = rom_mask
//
template <typename C>
bool One_wire_search<C>::read_rom()
{
    uint8_t i = 0;
    for (uint8_t i_rom = 0; i_rom < ROM_size; ++i_rom)
	for (uint8_t rom_mask = 0x01; rom_mask != 0; rom_mask <<= 1){
	    ++i;
	    if (!read_bit_rom(i, i_rom, rom_mask))
		return false;
	}
	
    return true;
}

template <typename C>
bool One_wire_search<C>::read_bit_rom(uint8_t i, 
				      uint8_t i_rom, uint8_t rom_mask)
{
    uint8_t discrepancy_type = read_two_bits();

    uint8_t next_bit{};

    switch(discrepancy_type){
	break; case 0x00: next_bit = calculate_next_bit(i, i_rom, rom_mask);
	break; case 0x01: next_bit = 0;
	break; case 0x10: next_bit = 1;
	break; case 0x11: return false;
    }

    ROM_write(i_rom, rom_mask, next_bit);
    One_wire::write_bit(next_bit);

    return true;
}


// Truco: para que coincida el valor devuelto con la Table 1. de la app note
// 187, devuelvo un uint8_t tal que su valor en hexadecimal coincida con la
// tabla.
template <typename C>
uint8_t One_wire_search<C>::read_two_bits()
{
    uint8_t b = One_wire::read_bit();
    uint8_t c = One_wire::read_bit();

    if (b == 0){
	if (c == 0)  return 0x00;
	else         return 0x01;
    }

    if (c == 0)  return 0x10;
    else         return 0x11;
}


// Se ha producido una discrepancia. Calculemos el siguiente bit a escribir en
// la ROM (y el camino por el que vamos a seguir en el diagrama de árbol)
template <typename C>
uint8_t One_wire_search<C>::calculate_next_bit(uint8_t i, 
				      uint8_t i_rom, uint8_t rom_mask)
{
    uint8_t next_bit{};

    if (i < i_last_discrepancy){
	next_bit = ROM_bit(i_rom, rom_mask);
    }

    else if (i == i_last_discrepancy)
	next_bit = 1;

    else // i > i_last_discrepancy
	next_bit = 0;

    if (next_bit == 0)
	i_last_zero_ = i;   

    return next_bit;
}


// Devuelve el bit ROM[i, mask]
template <typename C>
inline uint8_t One_wire_search<C>::ROM_bit(uint8_t i, uint8_t mask)
{
    if (ROM[i] & mask)
	return 1;

    return 0;
}

// ROM[i, mask] = bit
template <typename C>
inline void One_wire_search<C>::ROM_write(uint8_t i, uint8_t mask,
					  uint8_t bit)
{
    if (bit)
	ROM[i] |= mask;

    else 
	ROM[i] &=~ mask;
}


}// namespace


#endif


