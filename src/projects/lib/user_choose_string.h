// Copyright (C) 2019-2022 Manuel Perez 
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

#ifndef __DEV_USER_CHOOSE_STRING_H__
#define __DEV_USER_CHOOSE_STRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para mostrar un menu en un Screen.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    Manuel Perez
 *	02/11/2019 v0.0
 *	02/01/2020 v0.1 Reescrito.
 *	07/03/2022      Screen en vez de LCD.
 *
 ****************************************************************************/
#include <avr_time.h>

#include <atd_cstring.h>

#include "dev_keyrow.h"
#include <not_generic.h>

namespace dev{


// Tipos 
constexpr int user_choose_string_type_lineal   = 0;
constexpr int user_choose_string_type_circular = 1;

// En los Screen se suele dar cols x rows (al contrario que en las matrices).
// Seguiré con esa convención aquí.
template <typename Screen_t,
          typename Keyrow3_t,
          uint8_t ncols, uint8_t nrows>
struct User_choose_string_interface {
    using Screen                     = Screen_t;
    using Keyrow3                 = Keyrow3_t;
    static constexpr uint8_t rows = nrows;
    static constexpr uint8_t cols = ncols;
};


/*!
 *  \brief  Muestra un menu en el Screen y permite seleccionar una opción via 
 *  \brief  teclado.
 *
 *  La idea es mostrar un combobox de un tamaño fijo, por ejemplo, se muestran
 *  3 opciones de 20 caracteres máximo. La ventana del combobox es de tamaño
 *  fijo (ej: 3 filas x 20 columnas).
 *
 *  El teclado tiene 3 teclas: 
 *	up, down: equivalentes a decrementar, incrementar
 *	enter	: equivalente a enter o fin.
 *
 *  Ver el ejemplo del test para ver cómo usarlo.
 *
 *  Observaciones:
 *	1. Al principio definía dinámicamente rows x cols. Sin embargo, al
 *	   observar que estas dimensiones se tienen que conocer en tiempo de
 *	   compilación reescribí la clase para poder pasarlas como parámetro
 *	   de template. El test inicial, donde definía las rows x cols
 *	   dinámicamente ocupaba 6954 bytes. Al definirlo en tiempo de
 *	   compilación el mismo test paso a 6208 bytes, ahorrando 700 bytes.
 *	   Da la impresión de que sí merece la pena.
 */
template <typename Interface, typename Array, int type0>
class User_choose_string{
public:
    static_assert(type0 == user_choose_string_type_lineal or 
		  type0 == user_choose_string_type_circular, 
		  "incorrect user_choose_string_type");

    static constexpr int type = type0; // lineal o circular

    using Screen    = Interface::Screen;
    using Keyrow3   = Interface::Keyrow3;

    using size_type = typename Array::size_type;

    /// Mostramos el número en el Screen e interaccionamos con el usuario via
    /// el teclado indicado.
    User_choose_string(Screen& lcd,
                       Keyrow3,
                       const Array& str);

    // Dejamos el Screen en el estado en que se encontraba.
    ~User_choose_string();

    /// Número de filas del menú que mostramos en el Screen.
    constexpr uint8_t rows()  const {return rows_;}

    /// Número de columnas del menú que mostramos en el Screen.
    constexpr uint8_t cols()  const {return cols_;}

    /// Posición dentro del Screen en el que empieza el menú.
    /// (x,y) = esquina superior izquierda donde empieza el menú.
    User_choose_string& pos(uint8_t x, uint8_t y);


    /// Muestra el menu, mostrando como primera opción la indicada por
    /// 'first_option'. 
    /// Returns: la opción de menu elegida por el usuario (las opciones
    /// empiezan en la primera y no en cero).
    uint8_t show(uint8_t first_option = 1);

private:
// Data
// -----
    // Dispositivos a los que conectamos este menu
    Screen& lcd_;	    

    // keyrow
    static constexpr auto enter_key()
    { return Keyrow3::template key<Keyboard_code_kascii::enter>(); }

    static constexpr auto up_key()
    { return Keyrow3::template key<Keyboard_code_kascii::up>(); }

    static constexpr auto down_key()
    { return Keyrow3::template key<Keyboard_code_kascii::down>(); }


    // Menu y opciones seleccionadas.
    const Array str_;

    uint8_t y0_;    // Posición de la primera opción del  menu que mostramos 
		    // en pantalla.
		    // Ejemplo: si la primera opción es la 3, entonces y0_ = 3.

    // Posición del menu: (xm_, ym_)
    // -----------------------------
    // El menu lo mostramos en una ventana del Screen. ¿Merece la pena definir
    // Screen_window? Quedaría más limpio la implementación de esta clase.
    // Posición dentro del Screen de la esquina superior izda donde
    // empezamos a dibujar el menu.
    uint8_t xm_;    // Posición x del cursor. Nos dice a partir de qué posición 
		    // vamos a escribir las opciones.
    uint8_t ym_;    // Fila donde empezamos a dibujar el menu

    // Posición del cursor: (xm_, ym_ + yr_)
    // -------------------------------------
    uint8_t yr_;    // Posición relativa a ym_ del cursor.

    // Dimensiones del menú en el Screen (son las dimensiones de la ventana_menu)
    // -----------------------------------------------------------------------
    // número de filas que mostramos en el Screen <= lcd_.rows()
    // Mostraremos las filas ym_, ym_ + 1, ..., ym_ + rows_ - 1
    static constexpr uint8_t rows_ = Interface::rows;
    static constexpr uint8_t cols_ = Interface::cols;


    // Estado del Screen antes
    bool lcd_cursor_on_;

    // Configuración
    // -------------
    // Frecuencia de muestreo. Vamos a mirar cada 150 ms el estado
    // del teclado.
    constexpr static uint8_t T_clock = 150; // 150 ms



// Funciones de ayuda
// ------------------
    // Memoriza el estado del lcd que vamos a cambiar, para que al destruir el
    // objeto dejar el lcd en el estado que estaba antes.
    void init_Screen() 
    {
        lcd_cursor_on_ = lcd_.cursor_on();
    }

    // Muestra en el Screen el menu [p0, '\0'), colocando el cursor en (xm_, yr_)
    void show_option(size_type i0);

    // Muestra por primera vez el menu en el Screen. Se encarga de inicializar
    // todas las variables de este objeto.
    void show_str_first_time(uint8_t first_option);


    enum class Redraw {no, cursor, all};

    // Redibuja el Screen de acuerdo a redraw: puede no redraw nada, o solo
    // recolocar el cursor, o redrawlo todo.
    void redraw(Redraw type);

    // ¿Qué hacer cuando el usuario pulsa up/down/ok? lcd_move decide.
    void lcd_move();
    void lcd_move_foreward();
    void lcd_move_backwards();

    // Mueve el cursor hacia abajo. Si está en la última fila del lcd,
    // entonces mueve todo el lcd hacia abajo.
    Redraw lcd_down();

    // Mueve el cursor hacia arriba. Si está en la primera fila del lcd,
    // entonces mueve todo el lcd hacia arriba.
    Redraw lcd_up();
    

    // Los arrays de caracteres y los de cadenas los imprimimos 
    // de forma diferente. La función print fusiona el interfaz.
    void print(uint8_t x, uint8_t y, const char* p, uint8_t n);
    void print(uint8_t x, uint8_t y, char p, uint8_t);
    void print(uint8_t x, uint8_t y, const atd::const_nstring&, uint8_t);

    // Borramos la ventana
    void clear();
};

template <typename I, typename Array, int t>
inline User_choose_string<I, Array, t>::User_choose_string(
    Screen& lcd, Keyrow3, const Array& str0)
    : lcd_{lcd}, str_{str0}, 
      y0_{0}, 
      xm_{lcd.cursor_pos_x()}, ym_{0}, yr_{0}
{

    init_Screen();
}


template <typename I, typename A, int t>
User_choose_string<I, A, t>::~User_choose_string()
{
    if (!lcd_cursor_on_)
	lcd_.cursor_off();
}




template <typename I, typename A, int t>
inline User_choose_string<I, A, t>& User_choose_string<I, A, t>::pos(uint8_t x, uint8_t y)
{
    xm_ = x;
    ym_ = y;
    return *this;
}



template <typename I, typename A, int t>
void User_choose_string<I, A, t>::show_str_first_time(uint8_t first_option)
{
    yr_ = 0;
    y0_ = first_option;

    show_option(y0_);
}


// (RRR) Si dibujamos varias líneas, como en un menú, para seleccionar una
//       al pulsar down quiero mover el cursor hacia abajo, esto es,
//       incrementar el índice de la selección. Pero si solo muestro 1 línea
//       al pulsar down lo que quiero es ir hacia atrás.
template <typename I, typename A, int t>
void User_choose_string<I, A, t>::lcd_move()
{
    if constexpr (rows_ == 1)
	lcd_move_foreward();
    else
	lcd_move_backwards();
}


template <typename I, typename A, int t>
void User_choose_string<I, A, t>::lcd_move_foreward()
{
    Redraw rdw = Redraw::no;

    if (up_key().is_pressed())
	rdw = lcd_up();

    else if (down_key().is_pressed())
	rdw = lcd_down();

    redraw(rdw);
}


template <typename I, typename A, int t>
void User_choose_string<I, A, t>::lcd_move_backwards()
{
    Redraw rdw = Redraw::no;

    if (up_key().is_pressed())
	rdw = lcd_down();

    else if (down_key().is_pressed())
	rdw = lcd_up();

    redraw(rdw);
}




template <typename I, typename A, int t>
uint8_t User_choose_string<I, A, t>::show(uint8_t first_option)
{
// TODO: Micro::wait_ms(T_clock);
    not_generic::wait_ms(T_clock);	// Le damos tiempo al usuario a que suelte enter.

    clear();

    show_str_first_time(first_option);

    while (enter_key().is_not_pressed()){
	lcd_move();

// TODO: Micro::wait_ms(T_clock);
	not_generic::wait_ms(T_clock);
    }
    
    return y0_ + yr_; // = seleccion
}


template <typename I, typename A, int t>
void User_choose_string<I, A, t>::redraw(Redraw type)
{
    if (type == Redraw::all)
	show_option(y0_);

    else if (type == Redraw::cursor)
	lcd_.cursor_pos(xm_, ym_ + yr_);
}

template <typename I, typename A, int t>
inline void User_choose_string<I, A, t>::print(uint8_t x, uint8_t y,
					       const char* p, uint8_t n)
{
    lcd_.print(nm::Row<int>{y},
               nm::From<int>{x},
               nm::Size<int>{static_cast<int>(cols())},
               p);
}

template <typename I, typename A, int t>
inline void User_choose_string<I, A, t>::print(uint8_t x, uint8_t y, 
					       char c, uint8_t)
{
    lcd_.cursor_pos(x, y);
    lcd_.print(c);
}

template <typename I, typename A, int t>
inline void User_choose_string<I, A, t>::print(
			    uint8_t x, uint8_t y,
			    const atd::const_nstring& str,
                                               uint8_t)
{
    lcd_.print(nm::Row<int>{y},
               nm::From<int>{x},
               nm::Size<int>{static_cast<int>(str.size())},
               str.data());
}

template <typename I, typename A, int t>
void User_choose_string<I, A, t>::show_option(size_type i0)
{
    for (size_type i = 0; i < rows() and (i + i0 < str_.size()); ++i){
	print(xm_, ym_ + i, str_[i + i0], cols());
    }

    lcd_.cursor_pos(xm_, ym_ + yr_);
}

template <typename I, typename A, int t>
typename User_choose_string<I, A, t>::Redraw
					User_choose_string<I, A, t>::lcd_up()
{
    if (yr_ < rows() - 1){
	++yr_;
	return Redraw::cursor;
    }

    if (y0_ + yr_ < str_.size() - 1){
	++y0_;
	return Redraw::all;
    }

    if constexpr (type == user_choose_string_type_circular){
	if (y0_ + yr_ == str_.size() - 1){
	    y0_ = 0;
	    yr_ = 0;
	    return Redraw::all;
	}
    }

    return Redraw::no;

}


// Devuelve true si hay que redraw el lcd.
template <typename I, typename A, int t>
typename User_choose_string<I, A, t>::Redraw
					User_choose_string<I, A, t>::lcd_down()
{
    if (yr_ > 0){
	--yr_;
	return Redraw::cursor;
    }

    if (y0_ > 0){
	--y0_;
	return Redraw::all;
    }

    if constexpr (type == user_choose_string_type_circular){
	if (y0_ == 0){
	    y0_ = str_.size() - rows();
	    yr_ = rows() - 1;
	    return Redraw::all;
	}
    }

    return Redraw::no;

}


template <typename I, typename A, int t>
void User_choose_string<I, A, t>::clear()
{
    for (uint8_t r = 0; r < rows(); ++r){
	lcd_.cursor_pos(xm_, ym_ + r);

	for (uint8_t c = 0; c < cols(); ++c)
	    lcd_.print(' ');
    }

}


// syntactic sugar
template <uint8_t ncols, uint8_t nrows = 1, 
	 typename Screen, typename Keyrow3, 
	 typename Array>
User_choose_string<User_choose_string_interface<Screen, Keyrow3, ncols, nrows>, 
		   Array, user_choose_string_type_lineal>
user_choose_string_lineal(Screen& lcd, Keyrow3 k, const Array& str)

{
    using Interface = User_choose_string_interface<Screen, Keyrow3,
						   ncols, nrows>;
    return 
	User_choose_string<Interface, Array, user_choose_string_type_lineal>
								{lcd, k, str};
}


template <uint8_t ncols, uint8_t nrows = 1, 
	 typename Screen, typename Keyrow3, 
	 typename Array>
User_choose_string<User_choose_string_interface<Screen, Keyrow3, ncols, nrows>, 
		   Array, user_choose_string_type_circular>
user_choose_string_circular(Screen& lcd, Keyrow3 k, const Array& str)

{
    using Interface = User_choose_string_interface<Screen, Keyrow3,
						   ncols, nrows>;
    return User_choose_string<Interface,
                              Array,
                              user_choose_string_type_circular>{lcd, k, str};
}



}// namespace

#endif


