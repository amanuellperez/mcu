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
 *	1) Como matriz de LEDS (MAX7219_matrix)
 *	2) Como digitos de un display tradicional BCD (MAX7219_digits)
 *
 *
 *	Las matrices de led, a su vez, las podemos conectar:
 *	1) Una fila de MAX7219 conectados en serie (daisy chained).
 *
 *	2) Conectar varios de los dispositivos anteriores a SPI via diferentes
 *	   chip selects, de tal manera que se obtiene una matriz de leds (bits
 *	   agrupados por columnas). 
 *       
 *
 * HISTORIA
 *    Manuel Perez
 *    22/08/2024 Escrito como traductor. El primer intento es del 2018.
 *		 MAX7219_matrix
 *
 *    02/10/2024 MAX7219_digits
 *
 ****************************************************************************/
#include <cstdint>
#include <span>
//#include <iterator> // forward_iterator

#include <mcu_SPI.h>
#include <atd_bit_matrix.h>
#include <atd_basic_types.h>	// View_of_int
				
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
//
// Este es el traductor del MAX7219
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
    using SelectoR   = Cfg::SPI_selector;
    using SPI_select = mcu::SPI_selector_with_deselect_delay<SelectoR, 1>;

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
 *				MAX7219_matrix
 ***************************************************************************/
// ¿Cómo conectar varios MAX7219 para formar un display rectangular?
//
// Venden 4 MAX7219 juntos con los displays correspondientes. Tal como los
// venden una forma sencilla de formar (con esos displays) una matriz es
// dividir la matriz verticalmente en strips, cada strip siendo uno de los
// conjuntos de los 4 módulos MAX7219:
//     0       1       2       3     <- número de strip
//     ||      ||      ||      ||
//     \/      \/      \/      \/
// i+------++------++------++------+
// 0|      ||      ||      ||      |
// 1|      ||      ||      ||      |
//  +------++------++------++------+
// 2|      ||      ||      ||      |
// 3|      ||      ||      ||      |
//  +------++------++------++------+
// 4|      ||      ||      ||      |
// 5|      ||      ||      ||      |
//  +------++------++------++------+
// 6|      ||      ||      ||      |
// 7|      ||      ||      ||      |
//  +------++------++------++------+
//  01234567012345670123456701234567 <- columnas relativas a la strip
//j:0123456789,10,11, ...	     <- columnas absolutas al display = j
//            
//  Al montarlo de esta forma estoy conectando cada página con conexiones
//  un chip_select DISTINTO, necesitando un SPI_selector que me permita iterar
//  por las 4 páginas.
//
//  Las caracteristicas de la matriz son: 
//	1) número de strips (tantas como quieras)
//	2) número de MAX7219 por strip = numero de modulos por strip
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
//  COORDENADAS
//	Observar que hay varios tipos de coordenadas:
//	1) Absolutas al display : (i,j) = (row, col) <- referencia bits
//	2) Relativas a la página: (strip, digit) con digit = [0..7]
//
//	Las absolutas no son prácticas ya que tenemos que escribir bytes
//	enteros.
//
//  ¿POR QUÉ USO DOS TIPOS DE Bitmatrix?
//  Cuando el MAX7219 lo conectamos en columnas, los bytes del bitmatrix
//  tienen que estar en columnas para ser copiados rápidamente.
//  Pero si los conectamos en filas, los bytes del bitmatrix hay que
//  copiarlos en filas.
//  De ahí la diferencia.
namespace MAX7219_matrix_impl_{

enum class Matrix_type{ by_columns, by_rows, unknown };

// Dependiendo de cómo esté conectado por hardware usaremos un tipo de
// Bitmatrix
// typename Bitmatrix(typename Cfg)
// {
//    if (Cfg::by_columns)
//	return atd::Bitmatrix_col_1bit<rows, cols>;
//
//    else if (Cfg::by_rows)
//	return atd::Bitmatrix_row_1bit<rows, cols>;
// }
//
template <typename Cfg>
inline constexpr Matrix_type matrix_type()
{
    if constexpr (requires {Cfg::by_columns;})
	{ return Matrix_type::by_columns;}

    else if constexpr (requires {Cfg::by_rows;})
	{ return Matrix_type::by_rows;}

    else 
	return Matrix_type::unknown;
}


template <typename Cfg, uint8_t rows, uint8_t cols, 
	  Matrix_type type = matrix_type<Cfg>()> // este hace el switch
struct Bitmatrix_type{};

template <typename Cfg, uint8_t rows, uint8_t cols>
struct Bitmatrix_type<Cfg, rows, cols, Matrix_type::by_columns> 
{ using type = atd::Bitmatrix_col_1bit<rows, cols>;};

template <typename Cfg, uint8_t rows, uint8_t cols>
struct Bitmatrix_type<Cfg, rows, cols, Matrix_type::by_rows>
{ using type = atd::Bitmatrix_row_1bit<rows, cols>;};

template <typename Cfg, uint8_t rows, uint8_t cols>
using Bitmatrix_t = typename Bitmatrix_type<Cfg, rows, cols>::type;

// rows and cols
// -------------
// TODO: viendo que estoy calculando mucha cosa, se puede calcular todo dentro
// de una clase y usar el resultado en MAX7219_matrix
template <typename Cfg, uint8_t nstrips, uint8_t modules_per_strip>
inline 
constexpr uint8_t rows()
{
    constexpr uint8_t minidisplay_rows = 8; // DUDA: generalizarlo?
    constexpr uint8_t minidisplay_cols = 8;

    if constexpr (matrix_type<Cfg>() == Matrix_type::by_columns)
	return modules_per_strip * minidisplay_rows;

    else if constexpr (matrix_type<Cfg>() == Matrix_type::by_rows)
	return nstrips * minidisplay_cols;

    else
	return 0; // error
}

template <typename Cfg, uint8_t nstrips, uint8_t modules_per_strip>
inline
constexpr uint8_t cols()
{
    constexpr uint8_t minidisplay_rows = 8; // DUDA: generalizarlo?
    constexpr uint8_t minidisplay_cols = 8;

    if constexpr (matrix_type<Cfg>() == Matrix_type::by_columns)
	return nstrips * minidisplay_cols;

    else if constexpr (matrix_type<Cfg>() == Matrix_type::by_rows)
	return modules_per_strip * minidisplay_rows;

    else
	return 0; // error
}
} // namespace
  

template <typename Cfg, uint8_t nstrips0, uint8_t nmodules_per_strip0>
class MAX7219_matrix {
public:
// Cfg
    static constexpr uint8_t nstrips = nstrips0; 
    static constexpr uint8_t modules_per_strip = nmodules_per_strip0; 

    static constexpr uint8_t strip_cols = 8;

    // dimensiones de los pequeños displays que forman el display entero
    // minidisplay = modulo (pero hoy me suena mejor minidisplay)
    static constexpr uint8_t minidisplay_rows = 8;
    static constexpr uint8_t minidisplay_cols = 8;

// Concebido como matriz de bits
    // Dimensiones en bits del display 
    static constexpr uint8_t rows 
	    = MAX7219_matrix_impl_::rows<Cfg, nstrips, modules_per_strip>();
    static constexpr uint8_t cols 
	    = MAX7219_matrix_impl_::cols<Cfg, nstrips, modules_per_strip>();

    using Bitmatrix = MAX7219_matrix_impl_::Bitmatrix_t<Cfg, rows, cols>;

// Constructor
    MAX7219_matrix() {}; // Text_display necesita un constructor
    static void init();

// Write
    // Escribe en la página nstrip, columna ndigit de la strip, el array x
    static void write(uint8_t nstrip, uint8_t ndigit,
				std::span<uint8_t, modules_per_strip> x);

    // Escribimos la matriz m en el display
    static void write(const Bitmatrix& m)
	requires (std::is_same_v<Bitmatrix, atd::Bitmatrix_col_1bit<rows, cols> >);

    static void write(const Bitmatrix& m)
	requires (std::is_same_v<Bitmatrix, atd::Bitmatrix_row_1bit<rows, cols> >);

    // Borra la fila row
    static void clear(uint8_t row);

    // Borra todo el array
    static void clear();

// comandos
    static void intensity(uint8_t nstrip, uint8_t I);
    static void intensity(uint8_t I);

    // turn_on/turn_off (me gustan más que normal_mode/shutdown
    static void turn_on (uint8_t nstrip);
    static void turn_on ();

    static void turn_off (uint8_t nstrip);
    static void turn_off ();


    static void display_test_on(uint8_t nstrip);
    static void display_test_on();

    static void display_test_off(uint8_t nstrip);
    static void display_test_off();

private:
// Types
    using SPI        = Cfg::SPI_master;
    using SelectoR   = Cfg::SPI_selector;
    using SPI_select = mcu::SPI_selector_with_deselect_delay<SelectoR, 1>;

    using MAX7219    = private_::MAX7219<SPI>;
    
// Helpers
    static void move_command_till_last(uint8_t nstrip);
    static void send_no_op(uint8_t nstrip, uint8_t n);

    static void SPI_cfg() {MAX7219::SPI_cfg();}

    //template <std::forward_iterator It> TODO: no compila, pero debiera
    template <typename  It>
    static void write_digit(uint8_t nstrip, uint8_t ndigit,
					It p0, const It pe);
//    static 
//	void digit(uint8_t nstrip, uint8_t nmodule, uint8_t ndigit, uint8_t x);
};

// Quiero inicializar todos los modulos de la página, por ello tengo que ir
// haciendo el select/deselect correspondiente. (en el MAX7219 no es necesario
// pero en el MAX7221 si)
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::init() 
{
    SPI_select::init();

    SPI_cfg();

    for (uint8_t nstrip = 0; nstrip < nstrips; ++nstrip){
	SPI_select::select(nstrip);
	MAX7219::disable_decode_mode(); // es un display, no 7-segments
	SPI_select::deselect(nstrip);

	SPI_select::select(nstrip);
	MAX7219::scan_all_digits();
	SPI_select::deselect(nstrip);

	move_command_till_last(nstrip);
    }
}

// precondition: spi está configurado, y el chip no está seleccionado
template <typename C, uint8_t np, uint8_t nm>
inline 
void MAX7219_matrix<C, np, nm>::move_command_till_last(uint8_t nstrip)
{ send_no_op(nstrip, modules_per_strip - 1); }

// precondition: spi está configurado, y el chip no está seleccionado
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::send_no_op(uint8_t nstrip, uint8_t n)
{
// SPI_cfg(); 
    for (uint8_t i = 0; i < n; ++i){
	SPI_select spi{nstrip};
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
//template <typename C, uint8_t np, uint8_t nm>
//void MAX7219_matrix<C, np, nm>::digit(uint8_t nstrip, 
//				uint8_t nmodule, uint8_t ndigit, uint8_t x)
//{
//    SPI_cfg();
//
//    SPI_select spi{nstrip};
//
//    for (uint8_t i = 0; i < modules_per_strip() - nmodule; ++i)
//	MAX7219::no_op();
//
//    MAX7219::digit(ndigit, x);
//
//    for (uint8_t i = 0; i < nmodule; ++i)
//	MAX7219::no_op();
//}

template <typename C, uint8_t np, uint8_t nm>
inline
void MAX7219_matrix<C, np, nm>::write(uint8_t nstrip, uint8_t ndigit,
				std::span<uint8_t, modules_per_strip> data)
{ write_digit(nstrip, ndigit, data.begin(), data.end()); }

// Observar que no hago SPI::select/deselect con cada envio de comando
// Hay que enviar todos los comandos para llenar los registros de cada MAX7219 
// conectado en serie, y luego al hacer el deselect se cargan todos los
// valores en el MAX7219 correspondiente 

template <typename C, uint8_t np, uint8_t nm>
    // template <std::forward_iterator It>
    template <typename It>
void MAX7219_matrix<C, np, nm>::write_digit(uint8_t nstrip, uint8_t ndigit,
					It p0, const It pe)
{
    SPI_select spi{nstrip};

    for (; p0 != pe; ++p0)
	MAX7219::digit(ndigit + 1, *p0);
}


// Dos sistemas de coordenadas:
//	(nstrip, ndigit) = estamos en la strip nstrip, en su digit ndigit
//	          j      = estamos en la columna j del display (esta es global)
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::write(const Bitmatrix& m)
    requires (std::is_same_v<Bitmatrix, atd::Bitmatrix_col_1bit<rows, cols> >)
{
    using index_type = typename Bitmatrix::index_type;
    for (uint8_t nstrip = 0; nstrip < nstrips; ++nstrip)
    {
	for (uint8_t ndigit = 0; ndigit < minidisplay_cols; ++ndigit){
	    index_type j = minidisplay_cols * nstrip + ndigit;
	    write_digit(nstrip, ndigit, m.rcol_begin(j), m.rcol_end(j));
	}
    }
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::write(const Bitmatrix& m)
    requires (std::is_same_v<Bitmatrix, atd::Bitmatrix_row_1bit<rows, cols> >)
{
    using index_type = typename Bitmatrix::index_type;
    for (uint8_t nstrip = 0; nstrip < nstrips; ++nstrip)
    {
	for (uint8_t ndigit = 0; ndigit < minidisplay_rows; ++ndigit){
	    index_type i = minidisplay_rows * nstrip + ndigit;
	    write_digit(nstrip, ndigit, m.row_begin(i), m.row_end(i));
	}
    }
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::clear(uint8_t nstrip)
{
    SPI_cfg();

    for (uint8_t row = 0; row < 8; ++row){
	SPI_select::select(nstrip); 
	MAX7219::digit(row + 1, 0x00);
	SPI_select::deselect(nstrip); 

	move_command_till_last(nstrip);
    }
}


// (*) No se debe de usar el destructor para liberar el CS porque entonces se
//     llamaria a move_command_till_last con el chip seleccionado, y se
//     deseleccionaria despues de move_command_till_last!!!
template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::intensity(uint8_t nstrip, uint8_t I)
{
    SPI_cfg();

    SPI_select::select(nstrip);// No usar SPI_select spi{nstrip} (*)
    MAX7219::intensity(I);
    SPI_select::deselect(nstrip);

    move_command_till_last(nstrip);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_on(uint8_t nstrip)
{
    SPI_cfg();

    SPI_select::select(nstrip);
    MAX7219::normal_mode();
    SPI_select::deselect(nstrip);

    move_command_till_last(nstrip);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_off(uint8_t nstrip)
{
    SPI_cfg();

    SPI_select::select(nstrip);
    MAX7219::shutdown();
    SPI_select::deselect(nstrip);

    move_command_till_last(nstrip);
}


template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_on(uint8_t nstrip)
{
    SPI_cfg();

    SPI_select::select(nstrip);
    MAX7219::display_test_on();
    SPI_select::deselect(nstrip);

    move_command_till_last(nstrip);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_off(uint8_t nstrip)
{
    SPI_cfg();

    SPI_select::select(nstrip);
    MAX7219::display_test_off();
    SPI_select::deselect(nstrip);

    move_command_till_last(nstrip);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::clear()
{
    for (uint8_t p = 0; p < nstrips; ++p)
	clear(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::intensity(uint8_t I)
{
    for (uint8_t p = 0; p < nstrips; ++p)
	intensity(p, I);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_on()
{
    for (uint8_t p = 0; p < nstrips; ++p)
	turn_on(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::turn_off()
{
    for (uint8_t p = 0; p < nstrips; ++p)
	turn_off(p);
}

template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_on()
{
    for (uint8_t p = 0; p < nstrips; ++p)
	display_test_on(p);
}


template <typename C, uint8_t np, uint8_t nm>
void MAX7219_matrix<C, np, nm>::display_test_off()
{
    for (uint8_t p = 0; p < nstrips; ++p)
	display_test_off(p);
}


/***************************************************************************
 *				MAX7219_digits
 ***************************************************************************/
// DUDA: de momento solo está implementado 1 MAX7219. ¿Merece la pena conectar
// en serie varios MAX7219 digits? La modificación es sencilla.
template <typename Cfg>
class MAX7219_digits : private MAX7219_basic<Cfg>{
public:
// Types
    using Base = MAX7219_basic<Cfg>;

// Cfg
    static constexpr uint8_t ndigits = 8;

// Constructor
    MAX7219_digits() = delete;
    static void init();

// Write
    // Escribimos el digit 'x' en la posición i. Observar que un digit es
    // un número de una cifra (de 0 a 9)
    // digit va de [0..ndigits) 
    static void digit(uint8_t i, uint8_t x)
    {Base::digit(i + 1, x);}

    static void digit_with_point(uint8_t i, uint8_t x)
    {
	atd::bit(7).of(x) = 1;
	digit(i, x);
    }

    static void write_point(uint8_t i)
    {digit(i, 0x8F);}

    // Escribe el uint8_t x en la posición i. En este caso el número va de
    // 0 a 255
    template <Type::Integer Int>
    static void write(uint8_t i, const Int& x);


    // Borra el digit escrito en la posición i
    static void clear(uint8_t i) {digit(i, 0x0F);}

    // Borra el display
    static void clear();

// comandos
    static void intensity(uint8_t I) {Base::intensity(I);}

    // turn_on/turn_off (me gustan más que normal_mode/shutdown
    static void turn_on ()  {Base::turn_on();}
    static void turn_off () {Base::turn_off();}


    static void display_test_on() {Base::display_test_on();}
    static void display_test_off(){Base::display_test_off();}


private:

};

template <typename C>
void MAX7219_digits<C>::init()
{
    Base::init();

    Base::enable_decode_mode();
    Base::scan_all_digits();
}

template <typename C>
void MAX7219_digits<C>::clear()
{
    for (uint8_t i = 0; i < ndigits; ++i)
	clear(i);
}

template <typename C>
    template <Type::Integer Int>
void MAX7219_digits<C>::write(uint8_t i, const Int& x)
{
    atd::View_of_int v{x};
    for (auto d = v.digit_begin();
	      d != v.digit_end() and i < ndigits;
	      ++d, ++i){
	digit(i, *d);
    }

    
}

}// namespace dev

#endif

