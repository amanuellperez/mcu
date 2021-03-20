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

#ifndef  __USER_CHOOSE_NUMBER_H__
#define  __USER_CHOOSE_NUMBER_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz para elegir un número en un LCD vía un teclado.
 *
 *  - COMENTARIOS: 
 *	Observar que todo el código de aquí no depende para nada de que sea un
 *  avr, un pic u otro micro.
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *	30/08/2019 v0.0
 *	30/12/2020      Dos tipos: lineal y circular.
 *	07/03/2021      * Parametrizamos por el tipo de número que queremos
 *			leer. De esa forma podemos leer Magnitude y
 *			Magnitude_ENG_notation (para leer frecuencias).
 *
 *			* Mejoro la lectura. Si el usuario quiere pasar de 
 *			  1 Hz a 1 MHz no puede ir de uno en uno. Introduzco
 *			  Counter.
 *			  FRACASO!!!
 *			  Pretendía sumar primero 1 al número, luego 10, y por
 *			  últimmo 100 al número según mantuviese el usuario
 *			  pulsada la tecla UP/DOWN. El problema es que para
 *			  ello necesito poder hacer:
 *				    x_ += incr_;
 *			  donde incr_ será 1, 10 ó 100. Pero ¿cómo definir 
 *			  ENG_magnitude{1}??? ¿Qué significa esto? Lo que
 *			  quiero es sumarle 1 a x_. Si x_ son kHz quiero
 *			  entonces incr_ = 1kHz, pero si x_ está en MHz
 *			  entonces incr_ = 1MHz. 
 *			  No basta con definir:
 *				    incr = Rep{1}; ¿qué unidades?
 *			  Podría definir por defecto 1 en la unidad base de
 *			  ENG_magnitude (si son herztios 1 Hz), pero si x_ esta
 *			  en MHz al sumarle 1 Hz no se va a ver en pantalla el
 *			  cambio. No serviría para nada.
 *			  De momento lo dejo así. Si se necesita implementar
 *			  de verdad el caso de ENG_magnitude , el tiempo ya lo
 *			  dirá. (una forma sería dar una implementación
 *			  user_choose_number_type_lineal particular para
 *			  el caso ENG_magnitude; otra intentar ver cómo definir
 *			  Rep{1} con las mismas unidades de x_:
 *			  to_integer(1, x_)??? En este último caso habria que
 *			  definir esta función para todos los ints (se
 *			  limitaría a hacer un return Rep{1})).
 *
 ****************************************************************************/
#include <avr_time.h>
#include <iomanip>
#include <limits>
#include <array>

#include <atd_metronome.h>

#include "dev_keyboard.h"

namespace dev{


// Tipos 
constexpr int user_choose_number_type_lineal   = 0;
constexpr int user_choose_number_type_circular = 1;


/*!
 *  \brief  Permite elegir al usuario un número de n cifras.
 *
 *  Gestiona la elección: al principio, los números van subiendo
 *  lentamente, pero si se mantiene pulsada la tecla de forma seguida
 *  empiezan a aumentar más rápido.
 *
 *  El teclado tiene 3 teclas: 
 *	up, down: equivalentes a decrementar, incrementar
 *	enter	: equivalente a enter o fin.
 *
 *  Dos formas diferentes de poder elegir el número:
 *	1. type = lineal:
 *	   El usuario al pulsar up, down se mueve desde [min(), max()]. Al
 *	   llegar a min() por mucho que pulse down no baja. Al llegar a max()
 *	   la tecla up no hace nada.
 *
 *	2. type = circular:
 *	   Al llegar a min() y pulsar down se pasa a max(), y viceversa, al
 *	   llegar a max() y pulsar up se pasa a min().
 *	   Esta sería la forma de operar al configurar la hora: 
 *	   Si queremos poner en hora el reloj y son las 10:50, para escribir
 *	   los 50 minutos pulsamos down yendo desde 00 -> 59 -> 58 ... -> 55.
 *	   En el modo lineal habría que ir desde 00 -> 01 -> 02 ->... ->55 lo
 *	   cual es más incómodo.
 */
template <typename LCD, typename Keyboard3, int type0, typename Rep0>
class User_choose_number{
public:
    static_assert(type0 == user_choose_number_type_lineal or 
		  type0 == user_choose_number_type_circular, 
		  "incorrect user_choose_number_type");

    using Rep = Rep0;	// tipo de número que mostramos

    static constexpr int type = type0; // lineal o circular

    // Configuración
    // -------------
    /// Mostramos el número en el LCD e interaccionamos con el usuario via
    /// el teclado indicado.
    User_choose_number(LCD& lcd, Keyboard3)
		    :lcd_{lcd}{}

    /// Posición (col, row) del LCD donde mostramos el número a elegir.
    User_choose_number& pos(uint8_t col, uint8_t row);
    
    /// Valor mínimo que puede tomar el número a elegir.
    User_choose_number& min(Rep m);

    /// Valor máximo que puede tomar el número a elegir.
    User_choose_number& max(Rep M);

    /// Queremos que el número elegido x esté entre m <= x <= M.
    User_choose_number& between(Rep m, Rep M);

    // Funciones para elegir
    // ---------------------
    /// Elegimos un número de 2 cifras.
    /// Returns: valor elegido.
    Rep choose2(Rep x0);

    /// Elegimos un número de 4 cifras.
    /// Returns: valor elegido.
    Rep choose4(Rep x0);

private:
// Hardware
    LCD& lcd_;

    // keyboard
    static constexpr auto enter_key()
    { return Keyboard3::template key<Basic_keyboard_code::enter>(); }

    static constexpr auto up_key()
    { return Keyboard3::template key<Basic_keyboard_code::up>(); }

    static constexpr auto down_key()
    { return Keyboard3::template key<Basic_keyboard_code::down>(); }


// Counter
    // Al mantener pulsada la tecla UP, se quiere que al principio los números
    // cambien lentamente pero luego más rápidamente. 
    // El Metronomo se encarga de esto.
    // La idea es muestrear el teclado cada T_clock. Vamos contando cuántas veces
    // se ha pulsado UP al mirar T_clock y generamos un trigger cada cierto
    // tiempo. 
    using d1_max = atd::static_array<uint8_t, 2,9>;
    using d0_max = atd::static_array<uint8_t, 2,0>;
    atd::Metronome<d1_max, d0_max> counter_;


// Configuración
    uint8_t col_, row_;	    // posición donde están los 2 números.
    Rep min_ = std::numeric_limits<Rep>::min();
    Rep max_ = std::numeric_limits<Rep>::max();

// Datos
// -----
    Rep x_;	    // Dato que mostramos en pantalla


// Configuración estática
// ----------------------
    // Frecuencia de muestreo. Vamos a mirar cada 100 ms el estado
    // del teclado.
    static constexpr uint8_t T_clock = 100; // 100 ms

    enum class Tecla_pulsada{ up, down, ok, ninguna };

    // ultima tecla que ha pulsado el usuario
    Tecla_pulsada ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    

    // Funciones de ayuda
    // ------------------
    void print2(Rep x);
    void print4(Rep x);

    // Mira a ver si el usuario quiere actualizar x0, y en caso de ser 
    // así, lo actualiza de acuerdo a las pulsaciones del usuario.
    //
    //	Garantiza que x0 esté en el rango [0, max_].
    //
    void update();
    void update_ninguna();

    // La última tecla pulsada fue down. Si la tecla sigue pulsada
    // vamos decrementando el número cada vez más rápidamente.
    void update_down();
    void update_up();


};

template <typename L, typename T, int t0, typename R>
inline User_choose_number<L, T, t0, R>&
User_choose_number<L, T, t0, R>::pos(uint8_t col, uint8_t row)
{
    col_ = col;
    row_ = row;

    return *this;
}

template <typename L, typename T, int t0, typename R>
inline User_choose_number<L, T, t0, R>&
User_choose_number<L, T, t0, R>::min(Rep m)
{
    min_= m;
    return *this;
}

template <typename L, typename T, int t0, typename R>
inline User_choose_number<L, T, t0, R>&
User_choose_number<L, T, t0, R>::max(Rep M)
{
    max_ = M;
    return *this;
}

template <typename L, typename T, int t0, typename R>
inline User_choose_number<L, T, t0,R>&
User_choose_number<L, T, t0,R>::between(Rep m, Rep M)
{
    min(m);
    max(M);

    return *this;
}



// El cliente elige un número de 2 cifras
template <typename L, typename T, int t0, typename Rep>
Rep User_choose_number<L, T, t0,Rep>::choose2(Rep x0)
{ 
    char fill_char = lcd_.fill('0');

    x_ = x0;

    lcd_.cursor_on();

    ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    counter_.reset();

    print2(x_);
   
    wait_ms(300);// Hay que garantizar que el usuario haya soltado la tecla ok

    while(enter_key().is_not_pressed()){
	if (ultima_tecla_pulsada_ != Tecla_pulsada::ninguna)
	    print2(x_);

	update();

	wait_ms(T_clock);
    }


    lcd_.cursor_off();
    lcd_.fill(fill_char);

    return x_;
}

template <typename L, typename T, int t0, typename Rep>
Rep User_choose_number<L, T, t0, Rep>::choose4(Rep x0)
{ 

    x_ = x0;
    lcd_.cursor_on();

    ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    counter_.reset();

    print4(x_);
    // Hay que garantizar que el usuario haya soltado la tecla ok
    wait_ms(300);  

    while(enter_key().is_not_pressed()){
	if (ultima_tecla_pulsada_ != Tecla_pulsada::ninguna)
	    print4(x_);

	update();

	wait_ms(T_clock);
    }


    lcd_.cursor_off();

    return x_;
}




template <typename L, typename T, int t0, typename R>
void User_choose_number<L, T, t0,R>::update()
{
    switch(ultima_tecla_pulsada_){
	case Tecla_pulsada::down:
	    update_down();
	    break;

	case Tecla_pulsada::up:
	    update_up();
	    break;

	default:
	    update_ninguna();
	    break;
    }
}


template <typename L, typename T, int t0, typename R>
void User_choose_number<L, T, t0,R>::update_ninguna()
{

    if (down_key().is_pressed()){
	counter_.reset();
	ultima_tecla_pulsada_ = Tecla_pulsada::down;

	if (x_ > min_)
	    --x_;
    }

    else if (up_key().is_pressed()){
	counter_.reset();
	ultima_tecla_pulsada_ = Tecla_pulsada::up;

	if (x_ < max_)
	    ++x_;
    }
    else
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}


// La última tecla pulsada fue down. Si la tecla sigue pulsada
// vamos decrementando el número cada vez más rápidamente.
template <typename L, typename T, int t0, typename R>
void User_choose_number<L, T, t0,R>::update_down()
{
    if (down_key().is_pressed()){

	counter_.tick();

	if (x_ > min_){
	    if (counter_.trigger())
		--x_;

	}
	else{
	    if constexpr (type == user_choose_number_type_circular){
		x_ = max_;
	    }
	}

    }
    else 
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}


template <typename L, typename T, int t0, typename R>
void User_choose_number<L, T, t0,R>::update_up()
{
    if (up_key().is_pressed()){

	counter_.tick();

	if (x_ < max_){
	    if (counter_.trigger())
		++x_;
	}
	else{
	    if constexpr (type == user_choose_number_type_circular){
		x_ = min_;
	    }
	}
    }
    else 
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}

template <typename L, typename T, int t0, typename Rep>
void User_choose_number<L, T, t0, Rep>::print2(Rep x)
{
    lcd_.cursor_pos(col_, row_);
    lcd_ << std::setw(2) << x; 
    lcd_.cursor_pos(col_ + 1, row_);
}

template <typename L, typename T, int t0, typename Rep>
void User_choose_number<L, T, t0, Rep>::print4(Rep x)
{
    lcd_.cursor_pos(col_, row_);
    lcd_ << std::setw(4) << x;
    lcd_.cursor_pos(col_ + 3, row_);
}

// syntactic sugar
template <typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_lineal, R>
user_choose_number_lineal(L& lcd,  K k)
{ return User_choose_number<L, K, user_choose_number_type_lineal,R>{lcd, k}; }

template <typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_circular, R>
user_choose_number_circular(L& lcd,  K k)
{ return User_choose_number<L, K, user_choose_number_type_circular,R>{lcd, k}; }


}// namespace

#endif
