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

#ifndef __MCU_SPI_H__
#define __MCU_SPI_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Configuración genérica de SPI	
 *
 * HISTORIA
 *    Manuel Perez
 *    22/08/2024 SPI_pin_selector
 *    31/08/2024 SPI_selector_with_deselect_delay
 *	         SPI_pin_array_selector
 *
 ****************************************************************************/
#include <cstdint>
#include <atd_static.h>

namespace mcu {

// Este es un selector básico que no hay por qué usar. Cada aplicación podrá
// (y deberá) de escribir sus propios selectores
//
// Por defecto el pin es no_CS, esto es, para seleccionarlo escribimos 0, y
// para deseleccionarla un 1.
template <typename Micro0, uint8_t npin>
class SPI_pin_selector{
public:
    using Micro = Micro0;
    using Pin   = typename Micro::Pin<npin>;

    // Fundamental dejar deseleccionado el spi device.
    static void init() 
    {
	Pin::as_output();
	deselect();
    }

    static void select()
    { Pin::write_zero(); }

    static void deselect()
    { Pin::write_one(); }

    SPI_pin_selector() 
    { select(); }

    ~SPI_pin_selector() 
    { deselect(); }

};

/***************************************************************************
 *			  SPI_PIN_ARRAY_SELECTOR
 ***************************************************************************/
template <typename Micro0, uint8_t... npins>
class SPI_pin_array_selector{
public:
// Types
    using Micro = Micro0;
    using npin  = atd::static_array<uint8_t, npins...>;

    static_assert(npin::size <= 4, "To use more than 4 pin, modify select/deselect");
    template <uint8_t pin>
    using Pin   = typename Micro::Pin<pin>;

    // CUIDADO: i va de [0..npin::size) 
    // Quiero que el usuario desconozca los pines reales a los que se
    // ha conectado el dispositivo. Por eso la numeración es 0, 1, 2,...
    template <uint8_t i> 
    using SPI_pin_selector = 
			mcu::SPI_pin_selector<Micro, npin::template at<i>>;

// Functors
    template <uint8_t i>
    struct Functor{
	static void init(){
	    Pin<i>::as_output();
	    Pin<i>::write_one(); // deselect<i>();
	}
    };

    // Init se limita a convertir Init_functor<i>::init() en functor
    template <uint8_t i>
    using Init = atd::Function_as_functor_template<i, Functor<i>::init>;


// Constructors
    SPI_pin_array_selector() = delete;

    // Fundamental dejar deseleccionado el spi device.
    static void init() 
    { atd::for_each<npin, Init>(); } 


// static interface
    template <uint8_t i>
    static void select()   { SPI_pin_selector<i>::select(); }

    template <uint8_t i>
    static void deselect()   { SPI_pin_selector<i>::deselect(); }

// dynamic interface
    static void select(uint8_t i)
    { spi_select(i); }

    static void deselect(uint8_t i)
    { spi_deselect(i); }

// Constructor/destructor inteface
    SPI_pin_array_selector(uint8_t i) : npin_selected_{i}
    { spi_select(npin_selected_); }

    ~SPI_pin_array_selector() 
    { spi_deselect(npin_selected_); }


private:
// Data
// (RRR) Quiero un interfaz dinámico, que se pueda seleccionar la conexión que
//       se quiera:
//	    SPI_array spi{3}; // selecciona la 3
//	 Pero además, quiero mantener que Pin<3> sea static.
//	 Estas dos constraints me llevan a implementarlo de esta forma (a
//	 falta de una mejor).
//	    
    uint8_t npin_selected_; // Lo necesita el destructor

    // TODO: ¿cómo se puede implementar esto mas corto? (es sencillo, pero
    // largo, y no es genérico, no vale para cualquier npin::size)
    static void spi_select(uint8_t n)
    {
	if constexpr (npin::size == 1){
	    SPI_pin_selector<0>::select();
	}
	else if constexpr (npin::size == 2){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::select();
		break; case 1: SPI_pin_selector<1>::select();
	    }
	}
	else if constexpr (npin::size == 3){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::select();
		break; case 1: SPI_pin_selector<1>::select();
		break; case 2: SPI_pin_selector<2>::select();
	    }
	}
	else if constexpr (npin::size == 4){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::select();
		break; case 1: SPI_pin_selector<1>::select();
		break; case 2: SPI_pin_selector<2>::select();
		break; case 3: SPI_pin_selector<3>::select();
	    }
	}
    }

    static void spi_deselect(uint8_t n)
    {
	if constexpr (npin::size == 1){
	    SPI_pin_selector<0>::deselect();
	}
	else if constexpr (npin::size == 2){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::deselect();
		break; case 1: SPI_pin_selector<1>::deselect();
	    }
	}
	else if constexpr (npin::size == 3){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::deselect();
		break; case 1: SPI_pin_selector<1>::deselect();
		break; case 2: SPI_pin_selector<2>::deselect();
	    }
	}
	else if constexpr (npin::size == 4){
	    switch (n){
		break; case 0: SPI_pin_selector<0>::deselect();
		break; case 1: SPI_pin_selector<1>::deselect();
		break; case 2: SPI_pin_selector<2>::deselect();
		break; case 3: SPI_pin_selector<3>::deselect();
	    }
	}
    }
};


/***************************************************************************
 *			SPI_SELECTOR_WITH_DESELECT_DELAY
 ***************************************************************************/
// time_in_us = tiempo mínimo que necesita el chip select estar en HIGH para
// que se haga el LOAD de los bytes enviados
// Es un wrapper sobre SPI_selector que le añade un tiempo de espera despues
// de cada deselect para garantizar que se carga correctamente todo.
template <typename SPI_selector, uint32_t deselect_delay_in_us>
class SPI_selector_with_deselect_delay : public SPI_selector {
public:
// Types
    using Micro = typename SPI_selector::Micro;

// static interface
    // Fundamental dejar deseleccionado el spi device.
    static void init() 
    { SPI_selector::init(); }

// static interface para SPI_pin_selector
    static void select()
	requires requires {SPI_selector::select();}
    { SPI_selector::select(); }


    static void deselect()
	requires requires {SPI_selector::deselect();}
    { 
	SPI_selector::deselect(); 
	Micro::wait_us(deselect_delay_in_us);// <- esta es la diferencia con SPI_selector
    }

// static interface para SPI_pin_array_selector
    static void select(uint8_t i)
	requires requires(uint8_t j) {SPI_selector::select(j);}
    { SPI_selector::select(i); }

    static void deselect(uint8_t i)
	requires requires(uint8_t j) {SPI_selector::deselect(j);}
    { 
	SPI_selector::deselect(i); 
	Micro::wait_us(deselect_delay_in_us);// <- esta es la diferencia con SPI_selector
    }

// dynamic interface
    SPI_selector_with_deselect_delay()
	requires requires {SPI_selector{};}
	: SPI_selector{}
    {}

    SPI_selector_with_deselect_delay(uint8_t i)
	requires requires(uint8_t j) {SPI_selector{j};}
	: SPI_selector{i}
    {}

    ~SPI_selector_with_deselect_delay() 
    {
	Micro::wait_us(deselect_delay_in_us);// <- esta es la diferencia con SPI_selector
    }

};


} // namespace mcu

#endif


