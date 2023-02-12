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

#ifndef __DEV_EEPROM_LINEAL_H__
#define __DEV_EEPROM_LINEAL_H__
/****************************************************************************
 *
 * - DESCRIPCION: EEPROM abstracta.
 *
 *     Cuando se tenga un nuevo chip de EEPROM, lo primero que hay que hacer 
 *     es crear el traductor correspondiente (no es más que leer la datasheet
 *     e ir creando las funciones que correspondan). Esto da lugar a una clase
 *     particular de EEPROM. Todas las EEPROM particulares tienen que tener 
 *     el mismo conjunto de operaciones básicas. Sobre esta axiomática es
 *     sobre la que se basa esta clase EEPROM abstracta. (¿Qué concepto
 *     representa EEPROM? = ¿¿¿Memoria_paginada???)
 *
 *     Esta clase se encarga de convertir la memoria paginada de las EEPROM 
 *     particulares en memoria lineal, más sencilla de manejar.
 *  
 * - TODO:
 *	+ la función write no tiene por qué escribir todo lo que se le pide.
 *	Esta clase la vamos a usar desde un eeprom_iostream que contiene un
 *	buffer. La idea es volcar parte de ese buffer haciendo hueco para que
 *	el programa pueda continuar escribiendo. Si write intenta volcar todo
 *	el buffer se va a quedar esperando a guardar todas las páginas que
 *	tenga (tarda hasta 5ms). Es mejor que cuando se llama a write escriba
 *	la página en la que está y devuelva el control. De esa forma hace
 *	hueco en el buffer. Cuando eeprom_iostream vuelva a llamar a write lo
 *	mas seguro es que ya hayan pasado esos 5 ms de escritura con lo que
 *	podrá hacer otra petición de escritura de otra pagina. (y si no han
 *	pasado los 5 ms habrá pasado algo de tiempo con lo que el programa
 *	tendrá que esperar menos a que la eeprom escriba la pagina. Más
 *	eficiente!!!) Cambiar write e iostreams para que funcione así.
 *
 * - HISTORIA:
 *   Manuel Perez:
 *    16/07/2019 v0.1: Linearizo la EEPROM.
 *    19/09/2019 v0.2: Añado el buffer interno de escritura.		    
 *    14/10/2019 v0.3: Descompongo la clase en EEPROM_lineal y
 *		       EEPROM_iostream.
 *
 ****************************************************************************/
#include <algorithm>
#include <cstdint>


namespace dev{

enum class _EEPROM_lineal_state{
    goodbit = 0,	// No se ha producido ningún error de acceso a la eeprom
    end_memorybit = 1, // Se ha intentado escribir más alla del maximo address
    no_responsebit = 2	// No responde la memoria
//  eof		// al leer se ha llegado al final del fichero
};


inline constexpr _EEPROM_lineal_state operator&(_EEPROM_lineal_state a,
                                                _EEPROM_lineal_state b)
{
    return static_cast<_EEPROM_lineal_state>(static_cast<int>(a) &
                                             static_cast<int>(b));
}

inline constexpr _EEPROM_lineal_state operator|(_EEPROM_lineal_state a,
                                                _EEPROM_lineal_state b)
{
    return static_cast<_EEPROM_lineal_state>(static_cast<int>(a) |
                                             static_cast<int>(b));
}

inline constexpr _EEPROM_lineal_state operator^(_EEPROM_lineal_state a,
                                                _EEPROM_lineal_state b)
{
    return static_cast<_EEPROM_lineal_state>(static_cast<int>(a) ^
                                             static_cast<int>(b));
}

inline constexpr _EEPROM_lineal_state operator~(_EEPROM_lineal_state a)
{
    return static_cast<_EEPROM_lineal_state>(~static_cast<int>(a));
}

inline constexpr _EEPROM_lineal_state& operator&=(_EEPROM_lineal_state& a,
                                                  _EEPROM_lineal_state b)
{ return a = a & b; }

inline constexpr _EEPROM_lineal_state& operator|=(_EEPROM_lineal_state& a,
                                                  _EEPROM_lineal_state b)
{ return a = a | b; }

inline constexpr _EEPROM_lineal_state& operator^=(_EEPROM_lineal_state& a,
                                                  _EEPROM_lineal_state b)
{ return a = a ^ b; }

inline constexpr bool operator==(_EEPROM_lineal_state a, int b)
{ return static_cast<int>(a) == b; }

inline constexpr bool operator!=(_EEPROM_lineal_state a, int b)
{ return !(a == b); }



/*!
 *  \brief  EEPROM lineal con buffer interno.
 *
 */
template <typename EEPROM>
class EEPROM_lineal{
public:
    using Address = typename EEPROM::Address;
    using size_type = typename EEPROM::size_type;
    
    using State = _EEPROM_lineal_state;

    /// Inicializamos el buffer
    EEPROM_lineal() : state_{State::goodbit} { }


    /// Lee como máximo n bytes de la dirección de memoria address, 
    /// guardando el resultado en buf. 
    /// Devuelve el número de bytes leídos.
    size_type read(const Address& address, uint8_t* buf, size_type n);


    /// Intenta escribir los n bytes del buffer buf, a partir de la dirección 
    /// de memoria indicada address. En caso de llegar al final, deja de
    /// escribir. Si el número de bytes a escribir es grande, esta función
    /// puede tardar bastante, ya que tiene que ir escribiendo página a página
    /// la EEPROM (y puede llegar a tardar 5ms)
    /// Devuelve el número de bytes escritos. En caso de llegar al final de la
    /// memoria, marca el estado como end_memory() (state != good)
    //
    // Decisión de diseño: 
    // Supongamos que al intentar escribir se envían más
    // datos de los que quedan en la eeprom. Por ejemplo, se quieren escribir
    // 10 bytes pero estamos al final de la memoria y solo quedan 6 bytes
    // disponibles. ¿Qué hacer?
    //	1.- No escribir nada. Devuelve 0.
    //	2.- Escribir 6 bytes. Devuelve 6. Quedan 4 bytes sin escribir.
    // Opto por elegir la segunda opción. A fin de cuentas, si el usuario
    // quiere escribir los datos parece mejor opción escribir todo lo que se
    // pueda ya que de perder datos así se perderían los menos posible.
    size_type write(Address address, uint8_t* buf, size_type n);


    /// Hace eeprom[address... address + n) = value.
    /// Esta es la versión unbuffered de fill_n: no usa un buffer intermedio
    /// donde escribir value, sino que escribe directamente en la eeprom.
    size_type fill_n(const Address& address, size_type n, uint8_t value);


    // Acceso al State
    // ---------------
    /// Borramos el estado de la EEPROM.
    void clear() {state_ = State::goodbit;}

    /// La EEPROM ¿está en buen estado? ¿Se puede leer y escribir de ella?
    bool good() const {return state_ == 0;}

    /// En el último intento de escritura ¿se llegó al final de la EEPROM?
    bool end_memory() const {return (state_ & State::end_memorybit) != 0;}

    /// La memoria no responde.
    bool no_response() const {return (state_ & State::no_responsebit) != 0;}

    // Datos de la memoria
    // -------------------
    /// Dirección de la última posición de memoria.
    constexpr static size_type max_address() 
    {return EEPROM::max_address();}

private:
    // Dispositivo real en el que almacenamos los datos
    EEPROM eeprom_;
    State state_;
    
    // Tamaño máximo de los buffers que usaremos.
    constexpr static uint16_t SIZE_BUF_MAX = 64;

    // Funciones de ayuda
    // ------------------
    /// Configuramos el SPI. Antes de leer o escribir en la EEPROM
    /// hay que configurar el SPI.
    void cfg() const {eeprom_.cfg_SPI();}

    // Posición dentro de la página. 
    // Una dirección la podemos representar con un número (address)
    // o con 2 números (page_size, pos_page), que dan el número de página y la
    // posición dentro de esa página. Las eeproms paginan la memoria a la hora
    // de escribir.
    // address = page_size*num_page + pos_page
    size_type pos_page(Address address)
    {
	return address % eeprom_.page_size();
    }


    // Escribe el bufer buf a partir del address como mucho n bytes. 
    // Precondición: [address, address + n) están dentro de una misma página.
    // Devuelve true si todo va bien, false en caso de que la memoria no
    // responde. En este caso también marca el bit no_responsebit del state_.
    bool write_page(Address address, uint8_t* buf, size_type n);

    // Primer intento de controlar si la memoria responde o no. Básicamente
    // espero un número de ciclos determinado como mucho. Si no responde en
    // ese tiempo, marcaremos el estado de la memoria como erroneo.
    // La memoria puede tardar hasta 5ms en una operación de escritura, 
    // el bucle while donde miro NUM_CICLOS_MAX_EXPERA tardará unas 
    // 5 instrucciones (???) luego como mucho esperaremos 
    // 20000*5 = 100 ms > 5 ms!!!
    constexpr static uint16_t NUM_CICLOS_MAX_EXPERA = 20000;

    // Devuelve true si todo va bien, false si la memoria no responde.
    bool wait_until_write_in_process_end()
    {	
	uint16_t n = 0;
	while(eeprom_.write_in_process(eeprom_.read_status_register()))
	{
	    ++n;
	    if (n >= NUM_CICLOS_MAX_EXPERA)
		return false;
	}

	return true;
    }

    size_type write_unguarded(Address address, uint8_t* buf, size_type n);

    void setstate(State state) {state_ |= state;}
};



template <typename E>
typename EEPROM_lineal<E>::size_type 
EEPROM_lineal<E>::read(const Address& address, uint8_t* buf, size_type n)
{
    if (!good())
	return 0;

    cfg();

    if (!wait_until_write_in_process_end()){
	setstate(State::no_responsebit);
	return 0;
    }

    size_type leidos = std::min(n, eeprom_.max_address() - address + 1);
    eeprom_.read(address, buf, leidos);

    return leidos;
}


template <typename E>
bool EEPROM_lineal<E>::write_page(Address address, uint8_t* buf, size_type n)
{
    if (!wait_until_write_in_process_end()){
	setstate(State::no_responsebit);
	return false;
    }

    eeprom_.write_enable();
    eeprom_.write(address, buf, n);

    return true;
}


template <typename E>
typename EEPROM_lineal<E>::size_type 
EEPROM_lineal<E>::write(Address address, uint8_t* buf0, size_type N)
{
    if (!good())
	return 0;

    cfg();

    // Garantizamos no escribir fuera de la memoria
    if (address > eeprom_.max_address()){
	setstate(State::end_memorybit);
	return 0;
    }

    // N = std::min(N, eeprom_.max_address() + 1 - address);
    if (N > eeprom_.max_address() + 1 - address){
	N = eeprom_.max_address() + 1 - address;
	setstate(State::end_memorybit);
    }

    return write_unguarded(address, buf0, N);
}


// precondition: eeprom configurada y en bien conectada.
// precondition: address + N <= eeprom_.max_address() + 1
template <typename E>
typename EEPROM_lineal<E>::size_type EEPROM_lineal<E>::write_unguarded(
    Address address, uint8_t* buf0, size_type N)
{

    // 1.- Escribimos lo que queda en la pagina actual
    uint8_t* buf = buf0;
    size_type n = std::min(N, eeprom_.page_size() - pos_page(address));
    
    if (!write_page(address, buf, n))
	return 0;

    address += n;
    buf += n;
    N -= n;
    

    // 2.- Escribimos las páginas completas que haya
    while (N >= eeprom_.page_size()){
	if (!write_page(address, buf, eeprom_.page_size()))
	    return buf - buf0;

	address += eeprom_.page_size();
	buf += eeprom_.page_size();
	N -= eeprom_.page_size();
    }

    // 3.- Escribimos lo que queda (que no llega a una página entera)
    if (N > 0){
	if (!write_page(address, buf, N))
	    return buf - buf0;

	buf += N;
    }

    return (buf - buf0);
}



template <typename EEPROM>
typename EEPROM_lineal<EEPROM>::size_type 
EEPROM_lineal<EEPROM>::
	fill_n(const Address& address, size_type n, uint8_t value)
{
    if (!good())
	return 0;

// TODO: ¿qué pasa si n > buf_size??? overflow!!! Corregir!!!
    constexpr uint8_t buf_size = std::min(EEPROM::page_size(), SIZE_BUF_MAX);
    uint8_t buf[buf_size];
    std::fill_n(buf, n, value);
    return write(address, buf, n);
}


}// namespace


#endif


