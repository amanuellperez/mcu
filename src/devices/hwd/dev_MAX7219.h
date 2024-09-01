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

#ifndef __DEV_MAX7219_H__
#define __DEV_MAX7219_H__
/****************************************************************************
 *
 * DESCRIPCION
 *	Traductor del MAX7219
 *
 *	De momento hay 2 formas de conectar el dispositivo:
 *
 *	1) Una fila de MAX7219 conectados en serie (daisy chained).
 *	   Esto es MAX7219_array. 
 *
 *	2) Conectar varios de los dispositivos anteriores a SPI via diferentes
 *	   chip selects, de tal manera que se obtiene una matriz de leds (bits
 *	   agrupados por columnas). Esto es MAX7219_matrix
 *       
 *
 * HISTORIA
 *    Manuel Perez
 *    22/08/2024 Escrito como traductor. El primer intento es del 2018.
 *		 MAX7219_array, MAX7219_matrix
 *
 ****************************************************************************/
#include <cstdint>
#include <span>

#include <mcu_SPI.h>

namespace dev{

/***************************************************************************
 *				MAX7219
 ***************************************************************************/
namespace private_{
// Esta clase no es responsable de inicializar el SPI, sino solo de enviar los
// packets. 
// Esto es para poder tener diferentes implementacionnes de MAX7219: si se
// conectan en array la inicialización del SPI es de una manera mientras que
// si los conectamos en matriz es de otra. Por eso hay que desacoplar el envio
// del packet de la inicialización.
template <typename SPI0>
class  MAX7219{
public:
// Types
    using SPI = SPI0;

// Constructor
    MAX7219() = delete;

// SPI connection
    static void SPI_cfg();

// digits values
  // ndigit = 1..8
    // Escribimos el valor del digit correspondiente
    static void digit(uint8_t ndigit, uint8_t x) {send_packet(ndigit, x); }

// comandos
    static void no_op() {send_packet(0x00, 0x00); }

    static void enable_decode_mode() {send_packet(0x09, 0xFF);}

    static void disable_decode_mode() {send_packet(0x09, 0x00);}

    // Ver table 4 de la datasheet para posibles valores
    static void decode_mode(uint8_t mode) {send_packet(0x09, mode);}

    // Ver Table 7 para posibles valores
    static void intensity(uint8_t I) {send_packet (0x0A, I); }

    // Ver Table 8, aunque n indica el número de digit a mostrar
    static void scan_limit(uint8_t n) {send_packet (0x0B, n); }
    static void scan_all_digits() {send_packet (0x0B, 0x07); }

    // turn_on/turn_off
    static void normal_mode() {send_packet(0x0C, 0x01); }
    static void shutdown()    {send_packet(0x0C, 0x00); }

    // Sinónimos más comunes
    static void turn_on () { normal_mode(); }
    static void turn_off () { shutdown(); }

    
    static void display_test_on() {send_packet(0x0F, 0x01); }
    static void display_test_off() {send_packet(0x0F, 0x00); }

private:
    static void send_packet(uint8_t address, uint8_t data);
};


template <typename C>
void MAX7219<C>::SPI_cfg() 
{
    SPI::spi_mode(0, 0); // CPOL = 0, CPHA = 0
    SPI::data_order_MSB();
}

template <typename C>
void MAX7219<C>::send_packet(uint8_t address, uint8_t data)
{
    SPI::write(address);
    // TODO: no funciona wait_transmission_complete!!!
//    SPI::wait_transmission_complete();
    SPI::write(data);
}

} // namespace private_
  

/***************************************************************************
 *				MAX7219_basic
 ***************************************************************************/
// struct MAX7219_cfg {
//	using SPI_master   = myu::SPI_master;
//	using SPI_selector = mcu::SPI_selector<myu::Micro, 
//						    SPI_master::CS_pin_number>;
// };

// PRECONDICIONES
//	+ El cliente es el responsable de fijar la frecuencia del reloj.
//	  Voy a suponer una comunicación SPI donde la frecuecia del relojo es
//	  la misma para todos los dispositivos SPI
//	  
template <typename Cfg>
class  MAX7219_basic{
public:
// Constructor
    MAX7219_basic() = delete;
    static void init() { SPI_select::init(); }

// digits values
  // ndigit = 1..8
    // Escribimos el valor del digit correspondiente
    static void digit(uint8_t ndigit, uint8_t x);

// comandos
    static void no_op();

    static void enable_decode_mode();

    static void disable_decode_mode();

    // Ver table 4 de la datasheet para posibles valores
    static void decode_mode(uint8_t mode);

    // Ver Table 7 para posibles valores
    static void intensity(uint8_t I);

    // Ver Table 8, aunque n indica el número de digit a mostrar
    static void scan_limit(uint8_t n);
    static void scan_all_digits();

    // turn_on/turn_off
    static void normal_mode();
    static void shutdown();

    // Sinónimos más comunes
    static void turn_on () { normal_mode(); }
    static void turn_off () { shutdown(); }
    
    static void display_test_on();
    static void display_test_off();

private:
// Types
    using SPI		= typename Cfg::SPI_master;
    using SPI_select	= typename Cfg::SPI_selector;

    using MAX7219 = private_::MAX7219<SPI>;

};


template <typename C>
void MAX7219_basic<C>::digit(uint8_t ndigit, uint8_t x)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::digit(ndigit, x);
}


template <typename C>
void MAX7219_basic<C>::no_op()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::no_op();
}

template <typename C>
void MAX7219_basic<C>::enable_decode_mode()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::enable_decode_mode();
}

template <typename C>
void MAX7219_basic<C>::disable_decode_mode()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::disable_decode_mode();
}

template <typename C>
void MAX7219_basic<C>::decode_mode(uint8_t mode)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::decode_mode(mode);
}

template <typename C>
void MAX7219_basic<C>::intensity(uint8_t I)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::intensity(I);
}

template <typename C>
void MAX7219_basic<C>::scan_limit(uint8_t n)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::scan_limit(n);
}

template <typename C>
void MAX7219_basic<C>::scan_all_digits()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::scan_all_digits();
}

template <typename C>
void MAX7219_basic<C>::normal_mode()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::normal_mode();
}

template <typename C>
void MAX7219_basic<C>::shutdown()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::shutdown();
}

template <typename C>
void MAX7219_basic<C>::display_test_on()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::display_test_on();
}

template <typename C>
void MAX7219_basic<C>::display_test_off()
{
    MAX7219::SPI_cfg();
    SPI_select select;

    MAX7219::display_test_off();
}



/***************************************************************************
 *				MAX7219_array
 ***************************************************************************/
// Conectamos varios MAX7219 en fila
//
//  +------+------+------+------+
//  |      |      |      |      |
//  |      |      |      |      |
//  +------+------+------+------+
// 
// Hay 2 formas de conectar los displays:
//  1) por filas
//	+------+------+------+------+
//	|      |      |      |      |
//	+------+------+------+------+
//	|      |      |      |      |
//	+------+------+------+------+
//	
//	Si se escriben en el digit(1) 'ab' 4 veces (suponiendo que
//	tenemos 4 MAX7219) obtenemos:
//
//	+------+------+------+------+
//	|  ab  |  ab  |  ab  |  ab  |
//	+------+------+------+------+
//	|      |      |      |      |
//	+------+------+------+------+
//
//  2) por columnas
//       1  2 3 1 2 3  1 2  3 1 2  3 
//	+------+------+------+------+
//	| |  | | |  | | |  | | |  | |
//	| |  | | |  | | |  | | |  | |
//	+------+------+------+------+
//
//	Si se escriben en el digit(1) 'ab' 4 veces (suponiendo que
//	tenemos 4 MAX7219) obtenemos:
//
//       1  2 3 1 2 3  1 2  3 1 2  3 
//	+------+------+------+------+
//	|a|  | |a|  | |a|  | |a|  | |
//	|b|  | |b|  | |b|  | |b|  | |
//	+------+------+------+------+
//
//	Aunque a simple vista da la impresión de que este es peor que 1) en
//	este se podrían escribir las letras por columnas, pudiendo hacer
//	scrolling facilmente. Además para rellenarlo bastaria con copiar una
//	matriz y copiarla rellenandola en el orden:
//	    write(4, 3); write(4, 2); write(4, 1); <--- rellenado ultimo
//	    write(3, 3); write(3, 2); write(3, 1); <--- rellenado penultimo
//	    ...
//
//  Como los displays que tengo son del tipo 1), las pruebas estan hechas con
//  ese tipo no teniendo que funcionar con el tipo 2). (llamar a la clase
//  MAX7219_array_by_rows (???))
//
//  Version prueba: hagamos un array de MAX7219. Serán N MAX7219.
template <typename Cfg, uint8_t N>
class MAX7219_array {
public:
// Cfg
    static constexpr uint8_t size() { return N; } // = nmodules() (???)
    static constexpr uint8_t rows() {return 8;}	  // filas que tiene cada display
    
// Constructor
    MAX7219_array() = delete;
    static void init();

// Write
    // Escribimos en la fila row el array x[0...N)
    // Las filas van de [0...8) (lo normal, no como van en el MAX7219 que van
    // de [1...9) lo cual es muy confuso para un programador de C++ @_@
    static void row(uint8_t row, std::span<uint8_t, N> x);

    // Escribe en el display ndisplay, en el digit ndigit el valor x
    static void write(uint8_t ndisplay, uint8_t ndigit, uint8_t x);

    // Borra la fila row
    static void clear(uint8_t row);

    // Borra todo el array
    static void clear();

// comandos
    static void intensity(uint8_t I);

    // turn_on/turn_off (me gustan más que normal_mode/shutdown
    static void turn_on ();
    static void turn_off ();


    static void display_test_on();
    static void display_test_off();

    
private:
// Types
    using SPI        = typename Cfg::SPI_master;
    using SelectoR   = typename Cfg::SPI_selector;
    using SPI_select = mcu::SPI_selector_with_deselect_delay<SelectoR, 1>;

    using MAX7219    = private_::MAX7219<SPI>;
    
// Cfg
    static void move_command_till_last();
};

template <typename C, uint8_t N>
void MAX7219_array<C, N>::init() 
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::disable_decode_mode(); // es un display, no 7-segments
    SPI_select::deselect();

    SPI_select::select();
    MAX7219::scan_all_digits();
    SPI_select::deselect();

    move_command_till_last();
}

// Mueve el comando hasta el final
template <typename C, uint8_t N>
void MAX7219_array<C, N>::move_command_till_last()
{
    if constexpr (size() > 1) {
	for (uint8_t i = 0; i < size() - 1; ++i){
	    SPI_select spi;
	    MAX7219::no_op();

	}
    }
}


template <typename C, uint8_t N>
void MAX7219_array<C, N>::row(uint8_t row, std::span<uint8_t, N> value)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    for (auto x: value)
	MAX7219::digit(row + 1, x);
}

// Esta función genera parpadeo!!! (es culpa de la forma de funcionar del
// MAX7219?)
template <typename C, uint8_t N>
void MAX7219_array<C, N>::write(uint8_t ndisplay, uint8_t ndigit, uint8_t x)
{
    MAX7219::SPI_cfg();
    SPI_select select;

    for (uint8_t i = 0; i < size() - ndisplay; ++i)
	MAX7219::no_op();

    MAX7219::digit(ndigit, x);

    for (uint8_t i = 0; i < ndisplay; ++i)
	MAX7219::no_op();
}


template <typename C, uint8_t N>
inline void MAX7219_array<C, N>::clear(uint8_t row)
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::digit(row + 1, 0x00);
    SPI_select::deselect();

    move_command_till_last();
}

template <typename C, uint8_t N>
inline void MAX7219_array<C, N>::clear()
{
    for (uint8_t i = 0; i < rows(); ++i)
	clear(i);
}

template <typename C, uint8_t N>
void MAX7219_array<C, N>::intensity(uint8_t I) 
{
    MAX7219::SPI_cfg();


    SPI_select::select();
    MAX7219::intensity(I);
    SPI_select::deselect();

    move_command_till_last();
}

template <typename C, uint8_t N>
void MAX7219_array<C, N>::turn_on() 
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::turn_on();
    SPI_select::deselect();

    move_command_till_last();
}

template <typename C, uint8_t N>
void MAX7219_array<C, N>::turn_off() 
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::turn_off();
    SPI_select::deselect();

    move_command_till_last();
}


template <typename C, uint8_t N>
void MAX7219_array<C, N>::display_test_on()
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::display_test_on();
    SPI_select::deselect();

    move_command_till_last();
}

template <typename C, uint8_t N>
void MAX7219_array<C, N>::display_test_off()
{
    MAX7219::SPI_cfg();

    SPI_select::select();
    MAX7219::display_test_off();
    SPI_select::deselect();

    move_command_till_last();
}


/***************************************************************************
 *				MAX7219_matrix
 ***************************************************************************/
// ¿Cómo conectar varios MAX7219 para formar un display rectangular?
//
// Venden 4 MAX7219 juntos con los displays correspondientes. Tal como los
// venden una forma sencilla de formar (con esos displays) una matriz es
// dividir la matriz verticalmente en páginas, cada página siendo uno de los
// conjuntos de los 4 displays:
//
//     ||      ||      ||      ||
//     \/      \/      \/      \/
//  +------++------++------++------+
//  |      ||      ||      ||      |
//  |      ||      ||      ||      |
//  +------++------++------++------+
//  |      ||      ||      ||      |
//  |      ||      ||      ||      |
//  +------++------++------++------+
//  |      ||      ||      ||      |
//  |      ||      ||      ||      |
//  +------++------++------++------+
//  |      ||      ||      ||      |
//  |      ||      ||      ||      |
//  +------++------++------++------+
//
//  Al montarlo de esta forma estoy conectando cada página con conexiones
//  un chip_select DISTINTO, necesitando un SPI_selector que me permita iterar
//  por las 4 páginas.
//
//  Las caracteristicas de la matriz son: 
//	1) número de páginas (tantas como quieras)
//	2) número de MAX7219 por página = numero de moduls por página
//
//  Como el MAX7219 solo puede gestionar matrices de led 8x8, cada display
//  será de 8x8. 
//
//  Desconozco si existe un nombre estandar para llamar a cada display 8x8. En
//  alguno sitios los llamas LED modules. Usemos esa forma de hablar:
//
//	Una matriz de LEDs (= display) está formado a partir de LED modules 
//	(= minidisplays)
//
//
//
template <typename Cfg, uint8_t npages0, uint8_t nmodules_per_page0>
class MAX7219_matrix {
public:
// Cfg
    static constexpr uint8_t npages() { return npages0; }
    static constexpr uint8_t modules_per_page() { return nmodules_per_page0; }

// Constructor
    MAX7219_matrix() = delete;
    static void init();

// Write
    // Escribe en la página npage, columna col, el array x
    static void write(uint8_t npage, uint8_t col,
				std::span<uint8_t, modules_per_page()> x);

    // Borra la fila row
    static void clear(uint8_t row);

    // Borra todo el array
    static void clear();

// comandos
    static void intensity(uint8_t npage, uint8_t I);
    static void intensity(uint8_t I);

    // turn_on/turn_off (me gustan más que normal_mode/shutdown
    static void turn_on (uint8_t npage);
    static void turn_on ();

    static void turn_off (uint8_t npage);
    static void turn_off ();


    static void display_test_on(uint8_t npage);
    static void display_test_on();

    static void display_test_off(uint8_t npage);
    static void display_test_off();

private:
// Types
    using SPI        = Cfg::SPI_master;
    using SelectoR   = Cfg::SPI_selector;
    using SPI_select = mcu::SPI_selector_with_deselect_delay<SelectoR, 1>;

    using MAX7219    = private_::MAX7219<SPI>;
    
// Helpers
    static void move_command_till_last(uint8_t npage);
    static void send_no_op(uint8_t npage, uint8_t n);

    static void SPI_cfg() {MAX7219::SPI_cfg();}

    static 
	void digit(uint8_t npage, uint8_t nmodule, uint8_t ndigit, uint8_t x);
};

// Quiero inicializar todos los modulos de la página, por ello tengo que ir
// haciendo el select/deselect correspondiente. (en el MAX7219 no es necesario
// pero en el MAX7221 si)
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::init() 
{
    SPI_select::init();

    SPI_cfg();

    for (uint8_t npage = 0; npage < npages(); ++npage){
	SPI_select::select(npage);
	MAX7219::disable_decode_mode(); // es un display, no 7-segments
	SPI_select::deselect(npage);

	SPI_select::select(npage);
	MAX7219::scan_all_digits();
	SPI_select::deselect(npage);

	move_command_till_last(npage);
    }
}

// precondition: spi está configurado, y el chip no está seleccionado
template <typename C, uint8_t np, uint8_t nm>
inline 
void MAX7219_matrix<C, np, nm>::move_command_till_last(uint8_t npage)
{ send_no_op(npage, modules_per_page() - 1); }

// precondition: spi está configurado, y el chip no está seleccionado
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::send_no_op(uint8_t npage, uint8_t n)
{
// SPI_cfg(); 
    for (uint8_t i = 0; i < n; ++i){
	SPI_select spi{npage};
	MAX7219::no_op();
    }
}

// Datasheet: For the MAX7219, serial data at DIN, sent in 16-bit
//	      packets, is shifted into the internal 16-bit shift register
//	      with each rising edge of CLK regardless of the state of
//	      LOAD.
//
// Esto genera el problema de que al escribir en diferentes módulos de una
// misma página, luego se propagará lo escrito a los demás. Por ello, me veo
// obligado a usar tantos no-op
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::digit(uint8_t npage, 
				uint8_t nmodule, uint8_t ndigit, uint8_t x)
{
    SPI_cfg();

    SPI_select spi{npage};

    for (uint8_t i = 0; i < modules_per_page() - nmodule; ++i)
	MAX7219::no_op();

    MAX7219::digit(ndigit, x);

    for (uint8_t i = 0; i < nmodule; ++i)
	MAX7219::no_op();
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::write(uint8_t npage, uint8_t col,
				std::span<uint8_t, modules_per_page()> data)
{
    for (uint8_t i = 0; i < data.size(); ++i)
	digit(npage, i, col + 1, data[i]);
}


template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::clear(uint8_t npage)
{
    SPI_cfg();

    for (uint8_t row = 0; row < 8; ++row){
	SPI_select::select(npage); 
	MAX7219::digit(row + 1, 0x00);
	SPI_select::deselect(npage); 

	move_command_till_last(npage);
    }
}


// (*) No se debe de usar el destructor para liberar el CS porque entonces se
//     llamaria a move_command_till_last con el chip seleccionado, y se
//     deseleccionaria despues de move_command_till_last!!!
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::intensity(uint8_t npage, uint8_t I)
{
    SPI_cfg();

    SPI_select::select(npage);// No usar SPI_select spi{npage} (*)
    MAX7219::intensity(I);
    SPI_select::deselect(npage);

    move_command_till_last(npage);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_on(uint8_t npage)
{
    SPI_cfg();

    SPI_select::select(npage);
    MAX7219::normal_mode();
    SPI_select::deselect(npage);

    move_command_till_last(npage);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_off(uint8_t npage)
{
    SPI_cfg();

    SPI_select::select(npage);
    MAX7219::shutdown();
    SPI_select::deselect(npage);

    move_command_till_last(npage);
}


template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_on(uint8_t npage)
{
    SPI_cfg();

    SPI_select::select(npage);
    MAX7219::display_test_on();
    SPI_select::deselect(npage);

    move_command_till_last(npage);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_off(uint8_t npage)
{
    SPI_cfg();

    SPI_select::select(npage);
    MAX7219::display_test_off();
    SPI_select::deselect(npage);

    move_command_till_last(npage);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::clear()
{
    for (uint8_t p = 0; p < npages(); ++p)
	clear(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::intensity(uint8_t I)
{
    for (uint8_t p = 0; p < npages(); ++p)
	intensity(p, I);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_on()
{
    for (uint8_t p = 0; p < npages(); ++p)
	turn_on(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_off()
{
    for (uint8_t p = 0; p < npages(); ++p)
	turn_off(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_on()
{
    for (uint8_t p = 0; p < npages(); ++p)
	display_test_on(p);
}


template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_off()
{
    for (uint8_t p = 0; p < npages(); ++p)
	display_test_off(p);
}


}// namespace dev

#endif

