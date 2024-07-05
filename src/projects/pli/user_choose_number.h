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

#ifndef  __DEV_USER_CHOOSE_NUMBER_H__
#define  __DEV_USER_CHOOSE_NUMBER_H__

/****************************************************************************
 *
 *  - DESCRIPCION: Interfaz para elegir un número en un Screen vía un teclado.
 *
 *  - COMENTARIOS: 
 *	Observar que todo el código de aquí no depende para nada de que sea un
 *  avr, un pic u otro micro.
 *
 *  - HISTORIA:
 *    Manuel Perez
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
 *	01/05/2021       * Doy soporte para poder ir de 1 a 100.000 de forma
 *			   rápida (la idea es sumar 1, luego 10, 100 y por
 *			   ultimo ir de 1000 en 1000).
 *
 *			   Tiene un pequeño error: al llegar al número máximo
 *			   si iba de 1000 en 1000 y llegas a 9100 ya no puede
 *			   sumar 1000, y empieza a sumarle de 1 en 1. Basta
 *			   con soltar y volver a pulsar para que empiece a
 *			   sumar de 1, 10 y 100.
 *
 *			 * Añado una callback: cuando se varia el número
 *			   se llama a esta función. (Ej: en el stroboscope al
 *			   pulsar el usuario modificando la frecuencia tiene
 *			   que modificarse la señal generada). En principio la
 *			   callback será del tipo Main::callback.
 *
 *	05/02/2022	* Generalizo choose2 y choose4 en choose(width).
 *	12/03/2022	* Funciona con diferentes tipos de fuentes (big digits)
 *
 ****************************************************************************/
#include <avr_time.h>
#include <iomanip>
#include <limits>
#include <array>

#include <atd_metronome.h>
// #include <atd_types.h> Empty_struct
#include <atd_names.h>

#include "dev_keyrow.h"
#include "dev_LCD_big_digits.h"	// Digit fonts

namespace dev{
struct Empty_struct {} ;

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
 *
 *  DUDAS:
 *	1. Ahora estoy almacenando dentro de esta clase (d1,d0)_max y
 *	   (seq_x,seq_y), que supone unos 12 bytes. Se podrían parametrizar y
 *	   pasarlos en el constructor. Dos formas de hacerlo:
 *	    I. Como template: esto seguramente sea más ineficiente porque para
 *	       ahorrar máximo 12 bytes podemos duplicar el código fuente de
 *	       esta clase (cada template se instancia y genera su código).
 *	    II. Pasarlo como array de C. Habría que reescribir
 *	       Sequence_of_steps, funcionaria, pero... ¿ganaríamos espacio?
 *	   No está claro que eso sea realmente una optimización y puede que
 *	   por ahorrar un par de bytes se genere bastante más código.
 */
template <typename Screen, typename Keyrow3, 
	  int type0, typename Rep0, 
	  typename Main = Empty_struct>	// DUDA: usar void?
class User_choose_number{
public:
    static_assert(type0 == user_choose_number_type_lineal or 
		  type0 == user_choose_number_type_circular, 
		  "incorrect user_choose_number_type");

    static constexpr int type = type0; // lineal o circular

    using Rep = Rep0;	// tipo de número que mostramos
    using Callback = void (Main::*)(Rep);


    // Configuración
    // -------------
    /// Mostramos el número en el Screen e interaccionamos con el usuario via
    /// el teclado indicado.
    User_choose_number(Main& app, Screen& lcd, Keyrow3)
		    :scr_{lcd}, app_{&app}{ }

    User_choose_number(Screen& lcd, Keyrow3)
		    :scr_{lcd}, app_{nullptr}{ }

    /// Posición (col, row) del Screen donde mostramos el número a elegir.
    User_choose_number& pos(uint8_t col, uint8_t row);
    
    /// Valor mínimo que puede tomar el número a elegir.
    User_choose_number& min(Rep m);

    /// Valor máximo que puede tomar el número a elegir.
    User_choose_number& max(Rep M);

    /// Queremos que el número elegido x esté entre m <= x <= M.
    User_choose_number& between(Rep m, Rep M);

    /// Cada vez que varia el número se llama a esta función f.
    User_choose_number& callback(Callback f);

    // Funciones para elegir
    // ---------------------
    /// Choose a number of width w
    /// Returns: number choosen
    template <typename Font = Font_digit_default>
    Rep choose(Rep x0, const nm::Width<int>& w);

    /// Elegimos un número de 2 cifras.
    /// Returns: valor elegido.
    template <typename Font = Font_digit_default>
    Rep choose2(Rep x0) {return choose<Font>(x0, 2);}

    /// Elegimos un número de 4 cifras.
    /// Returns: valor elegido.
    template <typename Font = Font_digit_default>
    Rep choose4(Rep x0) {return choose<Font>(x0, 4);}


private:
// Hardware
    Screen& scr_;

    // keyrow
    static constexpr auto enter_key()
    { return Keyrow3::template key<Keyboard_code_kascii::enter>(); }

    static constexpr auto up_key()
    { return Keyrow3::template key<Keyboard_code_kascii::up>(); }

    static constexpr auto down_key()
    { return Keyrow3::template key<Keyboard_code_kascii::down>(); }


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

    using seq_x = atd::static_array<uint8_t, 0, 10, 20, 30>;
    using seq_y = atd::static_array<Rep, Rep{1}, Rep{10}, Rep{100}, Rep{1000}>;
    atd::static_Sequence_of_steps<seq_x, seq_y> incr_;

// Configuration
    uint8_t col_, row_;	    // posición donde están los 2 números.
    Rep min_ = std::numeric_limits<Rep>::min();
    Rep max_ = std::numeric_limits<Rep>::max();

// Data
// ----
    Rep x_;	    // Dato que mostramos en pantalla
    Main* app_; // opcional. Solo lo usamos si se llama a callbacks.
    Callback callback_ = nullptr;


// Static configuration
// --------------------
    // Frecuencia de muestreo. Vamos a mirar cada 100 ms el estado
    // del teclado.
    static constexpr uint8_t T_clock = 100; // 100 ms

    enum class Key_pressed{ up, down, ok, none };

    // ultima tecla que ha pulsado el usuario
    Key_pressed last_key_pressed_ = Key_pressed::none;
    

    // Helpers
    // -------
    template <typename Font = Font_digit_default>
    void print(Rep x, const nm::Width<int>& w);

    // Mira a ver si el usuario quiere actualizar x0, y en caso de ser 
    // así, lo actualiza de acuerdo a las pulsaciones del usuario.
    //
    //	Garantiza que x0 esté en el rango [0, max_].
    //
    void update();
	// La última tecla pulsada fue down. Si la tecla sigue pulsada
	// vamos decrementando el número cada vez más rápidamente.
	void update_down();
	void update_up();
	void update_none();
	    void update_none_down_pressed();
	    void update_none_up_pressed();



// counter
    void counter_reset();

    void wait_till_enter_key_is_not_pressed()
    { while(enter_key().is_pressed()){;}}

    void incr_next();

};

template <typename L, typename T, int t0, typename R, typename M>
inline User_choose_number<L, T, t0, R, M>&
User_choose_number<L, T, t0, R, M>::pos(uint8_t col, uint8_t row)
{
    col_ = col;
    row_ = row;

    return *this;
}

template <typename L, typename T, int t0, typename R, typename M>
inline User_choose_number<L, T, t0, R, M>&
User_choose_number<L, T, t0, R, M>::min(Rep m)
{
    min_= m;
    return *this;
}

template <typename L, typename T, int t0, typename R, typename M>
inline User_choose_number<L, T, t0, R, M>&
User_choose_number<L, T, t0, R, M>::max(Rep max0)
{
    max_ = max0;
    return *this;
}

template <typename L, typename T, int t0, typename R, typename M>
inline User_choose_number<L, T, t0,R, M>&
User_choose_number<L, T, t0,R, M>::between(Rep m, Rep max0)
{
    min(m);
    max(max0);

    return *this;
}


template <typename L, typename T, int t0, typename R, typename M>
inline User_choose_number<L, T, t0,R, M>&
User_choose_number<L, T, t0,R, M>::callback(Callback f)
{
    callback_ = f;
    return *this;
}


// El cliente elige un número de 2 cifras
template <typename L, typename T, int t0, typename Rep, typename M>
template <typename Font>
Rep User_choose_number<L, T, t0, Rep, M>::choose(Rep x0,
                                                 const nm::Width<int>& w)
{ 
    x_ = x0;

    // better: if (has_cursor<Font>): 
    if (std::is_same_v<Font, Font_digit_default>)
	scr_.cursor_on(); 

    last_key_pressed_ = Key_pressed::none;
    counter_reset();

    print<Font>(x_, w);
   
    wait_till_enter_key_is_not_pressed();

    while(enter_key().is_not_pressed()){
	if (last_key_pressed_ != Key_pressed::none)
	    print<Font>(x_, w);

	update();

	not_generic::wait_ms(T_clock);
    }


    scr_.cursor_off();

    return x_;
}


template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update()
{
    switch(last_key_pressed_){
	case Key_pressed::down:
	    update_down();
	    break;

	case Key_pressed::up:
	    update_up();
	    break;

	default:
	    update_none();
	    break;
    }
}


template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update_none()
{

    if (down_key().is_pressed())
	update_none_down_pressed();

    else if (up_key().is_pressed())
	update_none_up_pressed();

    else
	last_key_pressed_ = Key_pressed::none;
}


template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update_none_down_pressed()
{
    counter_reset();
    last_key_pressed_ = Key_pressed::down;

    if (x_ > min_){
	if (x_ > min_ + incr_.value())
	    x_ -= incr_.value();
	else
	    --x_;

	incr_next();
    }
    else{
	if constexpr (type == user_choose_number_type_circular){
	    x_ = max_;
	}
    }
}


template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update_none_up_pressed()
{
    counter_reset();
    last_key_pressed_ = Key_pressed::up;

    if (x_ < max_){
	if (x_ + incr_.value() < max_)
	    x_ += incr_.value();
	else
	    ++x_;

	incr_next();
    }
    else {
	if constexpr (type == user_choose_number_type_circular){
	    x_ = min_;
	}
    }
}


// La última tecla pulsada fue down. Si la tecla sigue pulsada
// vamos decrementando el número cada vez más rápidamente.
template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update_down()
{
    if (down_key().is_pressed()){

	counter_.tick();

	if (x_ > min_){
	    if (counter_.trigger()){
		if (x_ > min_ + incr_.value())
		    x_ -= incr_.value();
		else
		    --x_;
		incr_next();
	    }

	}
	else{
	    if constexpr (type == user_choose_number_type_circular){
		x_ = max_;
	    }
	}

    }
    else 
	last_key_pressed_ = Key_pressed::none;
}


template <typename L, typename T, int t0, typename R, typename M>
void User_choose_number<L, T, t0,R, M>::update_up()
{
    if (up_key().is_pressed()){

	counter_.tick();

	if (x_ < max_){
	    if (counter_.trigger()){
		if (x_ + incr_.value() < max_)
		    x_ += incr_.value();
		else
		    ++x_;
		incr_next();
	    }
	}
	else{
	    if constexpr (type == user_choose_number_type_circular){
		x_ = min_;
	    }
	}
    }
    else 
	last_key_pressed_ = Key_pressed::none;
}


template <typename L, typename T, int t0, typename R, typename M>
inline void User_choose_number<L, T, t0,R, M>::counter_reset()
{
    counter_.reset();
    incr_.reset();
}


template <typename L, typename T, int t0, typename R, typename M>
inline void User_choose_number<L, T, t0,R, M>::incr_next()
{
    if (callback_ != nullptr)
	(app_->*callback_)(x_);

    incr_.next();
}

// De momento solo admito diferentes Fonts para uint8_t y uint16_t
// No admito int8_t y int16_t porque no tengo implementado el signo - en
// grande.
template <typename L, typename T, int t0, typename Rep, typename M>
template <typename Font>
void User_choose_number<L, T, t0, Rep, M>::print(Rep x,
                                                 const nm::Width<int>& w)
{
    scr_.cursor_pos(col_, row_);
    if constexpr (std::is_same_v<Rep, uint8_t> or std::is_same_v<Rep, uint16_t>)
    { scr_.template print<Font>(x, w); }


    else
	atd::print(scr_, x, w);

    scr_.cursor_pos(col_ + w - 1, row_);
}



// syntactic sugar
// Para callbacks:
template <typename M, typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_lineal, R, M>
user_choose_number_lineal(M& app, L& lcd,  K k)
{
    return 
    User_choose_number<L, K, user_choose_number_type_lineal, R, M>{app, lcd, k};
}

template <typename M, typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_circular, R, M>
user_choose_number_circular(M& app, L& lcd,  K k)
{ 
  return 
  User_choose_number<L, K, user_choose_number_type_circular,R, M>{app, lcd, k}; 
}

// Si no se van a usar callbacks
template <typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_lineal, R>
user_choose_number_lineal(L& lcd,  K k)
{
    return User_choose_number<L, K, user_choose_number_type_lineal, R>{lcd, k};
}

template <typename L, typename K, typename R = uint16_t>
User_choose_number<L, K, user_choose_number_type_circular, R>
user_choose_number_circular(L& lcd,  K k)
{ return User_choose_number<L, K, user_choose_number_type_circular,R>{lcd, k}; }




}// namespace

#endif
