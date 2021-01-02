// Copyright (C) 2019-2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

#ifndef __USER_CHOOSE_STRING_H__
#define __USER_CHOOSE_STRING_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para mostrar un menu en un LCD.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	02/11/2019 v0.0
 *	02/01/2020 Reescrito.
 *
 ****************************************************************************/
#include <avr_time.h>

#include "dev_keyboard.h"

namespace dev{

/*!
 *  \brief  Muestra un menu en el LCD y permite seleccionar una opción via 
 *  \brief  teclado.
 *
 *  El teclado tiene 3 teclas: 
 *	up, down: equivalentes a decrementar, incrementar
 *	enter	: equivalente a enter o fin.
 *
 *  Ver el ejemplo del test para ver cómo usarlo.
 */
template <typename LCD, typename Keyboard3, typename Array>
class User_choose_string{
public:
    using size_type = typename Array::size_type;

    /// Mostramos el número en el LCD e interaccionamos con el usuario via
    /// el teclado indicado.
    User_choose_string(LCD& lcd,
                       Keyboard3,
                       const Array& str);

    // Dejamos el LCD en el estado en que se encontraba.
    ~User_choose_string();

    /// Número de filas que mostramos en el display.
    /// precondition: n <= lcd.rows();
    User_choose_string& rows(uint8_t n);

    /// Número de filas del menú que mostramos en el LCD.
    uint8_t rows()  const {return rows_;}

    /// Número de columnas que mostramos en el display.
    /// precondition: n <= lcd.cols();
    User_choose_string& cols(uint8_t n);

    /// Número de columnas del menú que mostramos en el LCD.
    uint8_t cols()  const {return cols_;}

    /// Posición dentro del LCD en el que empieza el menú.
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
    LCD& lcd_;	    

    // keyboard
    static constexpr auto enter_key()
    { return Keyboard3::template key<Basic_keyboard_code::enter>(); }

    static constexpr auto up_key()
    { return Keyboard3::template key<Basic_keyboard_code::up>(); }

    static constexpr auto down_key()
    { return Keyboard3::template key<Basic_keyboard_code::down>(); }


    // Menu y opciones seleccionadas.
    const Array str_;

    uint8_t y0_;    // Posición de la primera opción del  menu que mostramos 
		    // en pantalla.
		    // Ejemplo: si la primera opción es la 3, entonces y0_ = 3.

    // Posición del menu: (xm_, ym_)
    // -----------------------------
    // Posición dentro del LCD de la esquina superior izda donde
    // empezamos a dibujar el menu.
    uint8_t xm_;    // Posición x del cursor. Nos dice a partir de qué posición 
		    // vamos a escribir las opciones.
    uint8_t ym_;    // Fila donde empezamos a dibujar el menu

    // Posición del cursor: (xm_, ym_ + yr_)
    // -------------------------------------
    uint8_t yr_;    // Posición relativa a ym_ del cursor.

    // Dimensiones del menú en el LCD (son las dimensiones de la ventana_menu)
    // -----------------------------------------------------------------------
    // número de filas que mostramos en el LCD <= lcd_.rows()
    // Mostraremos las filas ym_, ym_ + 1, ..., ym_ + rows_ - 1
    uint8_t rows_;	
    uint8_t cols_;	


    // Estado del LCD antes
    bool lcd_cursor_on_;
    bool lcd_wrap_on_;

    // Configuración
    // -------------
    // Frecuencia de muestreo. Vamos a mirar cada 150 ms el estado
    // del teclado.
    constexpr static uint8_t T_clock = 150; // 150 ms



// Funciones de ayuda
// ------------------
    // Memoriza el estado del lcd que vamos a cambiar, para que al destruir el
    // objeto dejar el lcd en el estado que estaba antes.
    void init_LCD() 
    {
        lcd_wrap_on_   = lcd_.nowrap();
        lcd_cursor_on_ = lcd_.cursor_on();
    }

    // Muestra en el LCD el menu [p0, '\0'), colocando el cursor en (xm_, yr_)
    void show_option(size_type i0);

    // Muestra por primera vez el menu en el LCD. Se encarga de inicializar
    // todas las variables de este objeto.
    void show_str_first_time(uint8_t first_option);


    enum class Redraw {no, cursor, all};

    // Redibuja el LCD de acuerdo a redraw: puede no redraw nada, o solo
    // recolocar el cursor, o redrawlo todo.
    void redibuja(Redraw redraw);

    // Si el usuario pulsa up, mueve el lcd hacia arriba, y si pulsa down, lo
    // mueve hacia abajo.
    void lcd_move();

    // Mueve el cursor hacia abajo. Si está en la última fila del lcd,
    // entonces mueve todo el lcd hacia abajo.
    Redraw lcd_down();

    // Mueve el cursor hacia arriba. Si está en la primera fila del lcd,
    // entonces mueve todo el lcd hacia arriba.
    Redraw lcd_up();
    

    // Los arrays de caracteres y los de cadenas los imprimimos 
    // de forma diferente. La función print fusiona el interfaz.
    void print(const char* p, uint8_t n);
    void print(char p, uint8_t);

};

template <typename LCD, typename Keyboard3, typename Array>
inline User_choose_string<LCD, Keyboard3, Array>::User_choose_string(
    LCD& lcd, Keyboard3, const Array& menu)
    : lcd_{lcd}, str_{menu}, 
      y0_{0}, 
      xm_{lcd.cursor_pos_x()}, ym_{0}, yr_{0}
{
    rows_ = std::min<uint8_t>(lcd.rows(), menu.size());
    cols_ = lcd.cols();

    init_LCD();
}


template <typename L, typename K, typename A>
User_choose_string<L,K, A>::~User_choose_string()
{
    if (!lcd_cursor_on_)
	lcd_.cursor_off();

    if (!lcd_wrap_on_)
	lcd_.nowrap();
}



template <typename L, typename K, typename A>
inline User_choose_string<L, K, A>& User_choose_string<L,K, A>::rows(uint8_t n)
{
    rows_ = n;
    return *this;
}


template <typename L, typename K, typename A>
inline User_choose_string<L, K, A>& User_choose_string<L,K, A>::cols(uint8_t n)
{
    cols_ = n;
    return *this;
}


template <typename L, typename K, typename A>
inline User_choose_string<L, K, A>& User_choose_string<L,K, A>::pos(uint8_t x, uint8_t y)
{
    xm_ = x;
    ym_ = y;
    return *this;
}



template <typename L, typename K, typename A>
void User_choose_string<L,K, A>::show_str_first_time(uint8_t first_option)
{
    yr_ = 0;
    y0_ = first_option;

    show_option(y0_);
}


template <typename L, typename K, typename A>
void User_choose_string<L,K, A>::lcd_move()
{
    Redraw redraw = Redraw::no;

    if (up_key().is_pressed())
	redraw = lcd_up();

    else if (down_key().is_pressed())
	redraw = lcd_down();

    redibuja(redraw);
}



template <typename L, typename K, typename A>
uint8_t User_choose_string<L,K, A>::show(uint8_t first_option)
{
    wait_ms(T_clock);	// Le damos tiempo al usuario a que suelte enter.

    show_str_first_time(first_option);

    while (enter_key().is_not_pressed()){
	lcd_move();

	wait_ms(T_clock);
    }
    
    return y0_ + yr_; // = seleccion
}


template <typename L, typename K, typename A>
void User_choose_string<L,K, A>::redibuja(Redraw redraw)
{
    if (redraw == Redraw::all)
	show_option(y0_);

    else if (redraw == Redraw::cursor)
	lcd_.cursor_pos(xm_, ym_ + yr_);
}

template <typename L, typename K, typename A>
inline void User_choose_string<L,K, A>::print(const char* p, uint8_t n)
{ lcd_.print_line_nowrap(p, cols()); }

template <typename L, typename K, typename A>
inline void User_choose_string<L,K, A>::print(char c, uint8_t)
{ lcd_.print(c); }


template <typename L, typename K, typename A>
void User_choose_string<L,K, A>::show_option(size_type i0)
{
    for (size_type i = 0; i < rows() and (i + i0 < str_.size()); ++i){
	lcd_.cursor_pos(xm_, ym_ + i);
	print(str_[i + i0], cols());
    }

    lcd_.cursor_pos(xm_, ym_ + yr_);
}


template <typename L, typename K, typename A>
typename User_choose_string<L, K, A>::Redraw
				    User_choose_string<L, K, A>::lcd_up()
{
    if (yr_ < rows() - 1){
	++yr_;
	return Redraw::cursor;
    }

    if (y0_ + yr_ < str_.size() - 1){
	++y0_;
	return Redraw::all;
    }

    return Redraw::no;

}


// Devuelve true si hay que redraw el lcd.
template <typename L, typename K, typename A>
typename User_choose_string<L, K, A>::Redraw
					User_choose_string<L, K, A>::lcd_down()
{
    if (yr_ > 0){
	--yr_;
	return Redraw::cursor;
    }

    if (y0_ > 0){
	--y0_;
	return Redraw::all;
    }

    return Redraw::no;

}



// syntactic sugar
template <typename LCD, typename Keyboard3, typename A>
User_choose_string<LCD, Keyboard3, A> user_choose_string(
    LCD& lcd, Keyboard3 k, const A& str)

{
    return User_choose_string<LCD, Keyboard3, A>{lcd, k, str};
}

}// namespace

#endif


