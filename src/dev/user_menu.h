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

#ifndef __USER_MENU_H__
#define __USER_MENU_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Funciones para mostrar un menu en un LCD.
 *
 *  - COMENTARIOS: 
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	02/11/2019 v0.0
 *
 ****************************************************************************/
#include <stdint.h>

#include <algorithm>
#include <iterator>

#include <atd_algorithm.h>  // find_c
#include <atd_cstring.h>    // line_count
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
template <typename LCD, typename Keyboard3>
class User_menu{
public:
    /// Mostramos el número en el LCD e interaccionamos con el usuario via
    /// el teclado indicado.
    /// El menu es una cadena de C, donde cada opción de menú viene separada
    /// por '\n' (una línea = una opción).
    /// Ejemplo: const char menu[] = "1. Uno\n2. Dos\n3. Tres\n4. Atrás"
    User_menu(LCD& lcd, Keyboard3, const char* menu);

    // Dejamos el LCD en el estado en que se encontraba.
    ~User_menu();

    /// Número de filas que mostramos en el display.
    /// precondition: n <= lcd.rows();
    User_menu& num_rows(uint8_t n);

    /// Número de columnas que mostramos en el display.
    /// precondition: n <= lcd.cols();
    User_menu& num_cols(uint8_t n);

    /// Posición dentro del LCD en el que empieza el menú.
    /// (x,y) = esquina superior izquierda donde empieza el menú.
    User_menu& pos(uint8_t x, uint8_t y);


    /// Muestra el menu, mostrando como primera opción la indicada por
    /// 'opcion_inicial'. 
    /// Returns: la opción de menu elegida por el usuario (las opciones
    /// empiezan en la primera y no en cero).
    uint8_t show(uint8_t opcion_inicial = 1);

private:
    // Datos
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
    const char* menu_;	// cadena con el menu: "1.opcion\n2.opcion\n3.opcion"

    // [op0, ope): opciones que mostramos en el LCD.
    const char* op0_;   
    const char* ope_; 

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
    // Mostraremos las filas ym_, ym_ + 1, ..., ym_ + num_rows_ - 1
    uint8_t num_rows_;	
    uint8_t num_cols_;	


    // Estado del LCD antes
    bool lcd_cursor_on_;
    bool lcd_wrap_on_;

    // Configuración
    // -------------
    // Frecuencia de muestreo. Vamos a mirar cada 150 ms el estado
    // del teclado.
    constexpr static uint8_t T_clock = 150; // 150 ms


    // Número de filas del menú que mostramos en el LCD.
    uint8_t rows()  const {return num_rows_;}

    // Número de columnas del menú que mostramos en el LCD.
    uint8_t cols()  const {return num_cols_;}

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
    void show(const char* p0);

    // Muestra por primera vez el menu en el LCD. Se encarga de inicializar
    // todas las variables de este objeto.
    void show_menu_first_time(uint8_t opcion_inicial);


    enum class Redibujar {nada, cursor, todo};

    // Redibuja el LCD de acuerdo a redibujar: puede no redibujar nada, o solo
    // recolocar el cursor, o redibujarlo todo.
    void redibuja(Redibujar redibujar);

    // Si el usuario pulsa up, mueve el lcd hacia arriba, y si pulsa down, lo
    // mueve hacia abajo.
    void lcd_move();

    // Mueve el cursor hacia abajo. Si está en la última fila del lcd,
    // entonces mueve todo el lcd hacia abajo.
    Redibujar lcd_down();

    // Mueve el cursor hacia arriba. Si está en la primera fila del lcd,
    // entonces mueve todo el lcd hacia arriba.
    Redibujar lcd_up();


    // Funciones de ayuda genéricas
    // ----------------------------
    static const char* reverse_find(const char* op, const char* menu, char x);
};


template <typename LCD, typename Keyboard3>
inline User_menu<LCD, Keyboard3>::User_menu(LCD& lcd, Keyboard3,
	const char* menu)
    : lcd_{lcd}, menu_{menu}, y0_{1}, xm_{lcd.cursor_pos_x()},
      ym_{0}, yr_{0}
{
    num_rows_ = std::min<uint8_t>(lcd.rows(), atd::line_count(menu));
    num_cols_ = lcd.cols();

    init_LCD();
}


template <typename L, typename T>
User_menu<L,T>::~User_menu()
{
    if (!lcd_cursor_on_)
	lcd_.cursor_off();

    if (!lcd_wrap_on_)
	lcd_.nowrap();
}



template <typename L, typename T>
inline User_menu<L, T>& User_menu<L,T>::num_rows(uint8_t n)
{
    num_rows_ = n;
    return *this;
}


template <typename L, typename T>
inline User_menu<L, T>& User_menu<L,T>::num_cols(uint8_t n)
{
    num_cols_ = n;
    return *this;
}


template <typename L, typename T>
inline User_menu<L, T>& User_menu<L,T>::pos(uint8_t x, uint8_t y)
{
    xm_ = x;
    ym_ = y;
    return *this;
}



template <typename L, typename T>
void User_menu<L,T>::show_menu_first_time(uint8_t opcion_inicial)
{
    // opcion que mostramos
    yr_ = 0;
    y0_ = opcion_inicial;

    // Opciones que mostramos en el LCD: [opc0_, opce_)
    op0_ = atd::find_c(menu_, '\n', opcion_inicial - 1u);
    if (*op0_ == '\0'){
	op0_ = menu_;
	y0_ = 1;
    }
    else
	++op0_;

    ope_ = atd::find_c(op0_, '\n', rows());
    if (*ope_ != '\0')	++ope_;


    show(op0_);
}


template <typename L, typename T>
void User_menu<L,T>::lcd_move()
{
    Redibujar redibujar = Redibujar::nada;

    if (up_key().is_pressed())
	redibujar = lcd_up();

    else if (down_key().is_pressed())
	redibujar = lcd_down();

    redibuja(redibujar);
}



template <typename L, typename T>
uint8_t User_menu<L,T>::show(uint8_t opcion_inicial)
{
    wait_ms(T_clock);	// Le damos tiempo al usuario a que suelte enter.

    show_menu_first_time(opcion_inicial);

    while (enter_key().is_not_pressed()){
	lcd_move();

	wait_ms(T_clock);
    }
    
    return y0_ + yr_; // = seleccion
}


template <typename L, typename T>
void User_menu<L,T>::redibuja(Redibujar redibujar)
{
    if (redibujar == Redibujar::todo)
	show(op0_);

    else if (redibujar == Redibujar::cursor)
	lcd_.cursor_pos(xm_, ym_ + yr_);
}


template <typename L, typename T>
void User_menu<L,T>::show(const char* p)
{
    for (uint8_t y = 0; y < rows() and *p != '\0'; ++y){
	lcd_.cursor_pos(xm_, ym_ + y);
	p = lcd_.print_line_nowrap(p, cols());
    }

    lcd_.cursor_pos(xm_, ym_ + yr_);
}

template <typename L, typename T>
typename User_menu<L,T>::Redibujar User_menu<L,T>::lcd_down()
{
    if (yr_ < rows() - 1){
	++yr_;
	return Redibujar::cursor;
    }

    if (*ope_ == '\0')
	return Redibujar::nada;
    else{
	op0_ = atd::find_c(op0_, '\n');
	ope_ = atd::find_c(ope_, '\n');

	if (*op0_ != '\0') ++op0_;
	if (*ope_ != '\0') ++ope_;

	++y0_;
    }

    return Redibujar::todo;
}

// Devuelve true si hay que redibujar el lcd.
template <typename L, typename T>
typename User_menu<L,T>::Redibujar User_menu<L,T>::lcd_up()
{
    if (yr_ > 0){
	--yr_;
	return Redibujar::cursor;
    }

    if (op0_ == menu_)
	return Redibujar::nada;

    else{
	if (op0_ != menu_)
	    op0_ = reverse_find(op0_, menu_, '\n');

	if (ope_ != menu_)
	    ope_ = reverse_find(ope_, menu_, '\n');

	--y0_;
    }

    return Redibujar::todo;
}

// precondicion: op != menu
template <typename L, typename T>
const char* User_menu<L,T>::reverse_find(const char* op, const char* menu, char x)
{
    --op;   // *op == '\n'
    --op;   // *op == anterior a '\n'

    auto pe = std::reverse_iterator{menu};

    auto res =
        std::find(std::reverse_iterator{op}, pe, x);

    if (res == pe)
	return menu;

    op = res.base(); // el base es el siguiente a '\n', que es lo que buscamos

    return op;
}

}// namespace

#endif


