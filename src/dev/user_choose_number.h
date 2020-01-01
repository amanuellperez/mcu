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
 *
 ****************************************************************************/
#include <avr_time.h>
#include <iomanip>


namespace dev{



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
 */
template <typename LCD, typename Teclado>
class User_choose_number{
public:
    // Configuración
    // -------------
    /// Mostramos el número en el LCD e interaccionamos con el usuario via
    /// el teclado indicado.
    User_choose_number(LCD& lcd, Teclado& key)
		    :lcd_{lcd}, key_{key}{}

    /// Posición (col, row) del LCD donde mostramos el número a elegir.
    User_choose_number& pos(uint8_t col, uint8_t row);
    
    /// Valor mínimo que puede tomar el número a elegir.
    User_choose_number& min(uint16_t m);

    /// Valor máximo que puede tomar el número a elegir.
    User_choose_number& max(uint16_t M);

    /// Queremos que el número elegido x esté entre m <= x <= M.
    User_choose_number& between(uint16_t m, uint16_t M);

    // Funciones para elegir
    // ---------------------
    /// Elegimos un número de 2 cifras.a
    /// Returns: valor elegido.
    uint8_t choose2(uint8_t x0);

    /// Elegimos un número de 4 cifras.
    /// Returns: valor elegido.
    uint16_t choose4(uint16_t x0);

private:
    // pantalla y teclado usados
    LCD& lcd_;
    Teclado& key_;

    // Configuración
    uint8_t col_, row_;	    // posición donde están los 2 números.
    uint16_t min_ = 0;	    // valor mínimo que puede tener el número.
    uint16_t max_ = 65535;  // valor máximo que puede tener el número.

    // Datos
    // -----
    uint16_t x_;	    // Dato que mostramos en pantalla



    // Configuración estática
    // ----------------------
    // Frecuencia de muestreo. Vamos a mirar cada 100 ms el estado
    // del teclado.
    constexpr static uint8_t T_clock = 100; // 100 ms

    // Tiempo que vamos a esperar antes de interactuar con el usuario
    // para darle tiempo a que haya soltado la tecla 'ok'.
//    constexpr static uint8_t t_wait  = 300; // ms

    // Al principio incrementamos el número cada 4 ticks de reloj
    constexpr static uint8_t num_ticks_incr_uno = 5;

    // Cuando el usuario lleva pulsado el pulsador durante 12 pulsos de reloj
    // lo incrementamos cada pulso de reloj (yendo más rápido)
    constexpr static uint8_t num_ticks_incr_continuo_ = 10;


    enum class Tecla_pulsada{ up, down, ok, ninguna };
    // ultima tecla que ha pulsado el usuario
    Tecla_pulsada ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    
    uint8_t num_ticks_pulsada_ = 0; // número de ticks de reloj que lleva la 
				    // tecla pulsada



    // Funciones de ayuda
    // ------------------
    void print2(uint8_t x);
    void print4(uint16_t x);

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



template <typename L, typename T>
inline User_choose_number<L,T>& User_choose_number<L,T>::pos(uint8_t col, uint8_t row)
{
    col_ = col;
    row_ = row;

    return *this;
}


template <typename L, typename T>
inline User_choose_number<L,T>& User_choose_number<L,T>::min(uint16_t m)
{
    min_= m;
    return *this;
}

template <typename L, typename T>
inline User_choose_number<L,T>& User_choose_number<L,T>::max(uint16_t M)
{
    max_ = M;
    return *this;
}

template <typename L, typename T>
inline User_choose_number<L,T>& User_choose_number<L,T>::between(uint16_t m, uint16_t M)
{
    min(m);
    max(M);

    return *this;
}



// El cliente elige un número de 2 cifras
template <typename L, typename T>
uint8_t User_choose_number<L, T>::choose2(uint8_t x0)
{ 
    char fill_char = lcd_.fill('0');


    x_ = x0;

    lcd_.cursor_on();

    ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    num_ticks_pulsada_ = 0;

    print2(x_);
   
    wait_ms(300);// Hay que garantizar que el usuario haya soltado la tecla ok

    while(key_.enter.no_esta_pulsado()){
	if (ultima_tecla_pulsada_ != Tecla_pulsada::ninguna)
	    print2(x_);

	update();

	wait_ms(T_clock);
    }


    lcd_.cursor_off();
    lcd_.fill(fill_char);

    return static_cast<uint8_t>(x_);
}


template <typename L, typename T>
uint16_t User_choose_number<L, T>::choose4(uint16_t x0)
{ 

    x_ = x0;

    lcd_.cursor_on();

// TODO: hacer local ultima_tecla_pulsada_, creo que solo lo necesita esta
// función (o la equivalente de choose2). Revisar. O mejor mirar la
// implementación de user_choose_list, he eliminado Tecla_pulsada.
    ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
    num_ticks_pulsada_ = 0;

    print4(x_);
    // Hay que garantizar que el usuario haya soltado la tecla ok
    wait_ms(300);  

    while(key_.enter.no_esta_pulsado()){
	if (ultima_tecla_pulsada_ != Tecla_pulsada::ninguna)
	    print4(x_);

	update();

	wait_ms(T_clock);
    }


    lcd_.cursor_off();

    return x_;
}




template <typename L, typename T>
void User_choose_number<L, T>::update()
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


template <typename L, typename T>
void User_choose_number<L, T>::update_ninguna()
{

    if (key_.down.esta_pulsado()){
	num_ticks_pulsada_    = 0;
	ultima_tecla_pulsada_ = Tecla_pulsada::down;

	if (x_ > min_)
	    --x_;
    }

    else if (key_.up.esta_pulsado()){
	num_ticks_pulsada_    = 0;
	ultima_tecla_pulsada_ = Tecla_pulsada::up;

	if (x_ < max_)
	    ++x_;
    }
    else
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}


// La última tecla pulsada fue down. Si la tecla sigue pulsada
// vamos decrementando el número cada vez más rápidamente.
template <typename L, typename T>
void User_choose_number<L, T>::update_down()
{
    if (key_.down.esta_pulsado()){

	++num_ticks_pulsada_;

	if (x_ > min_){
	    if (num_ticks_pulsada_ < num_ticks_incr_continuo_){
		if ((num_ticks_pulsada_ % num_ticks_incr_uno) == 0)
		    --x_;
	    }
	    else
		--x_;

	}
    }
    else 
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}


template <typename L, typename T>
void User_choose_number<L, T>::update_up()
{
    if (key_.up.esta_pulsado()){

	++num_ticks_pulsada_;

	if (x_ < max_){
	    if (num_ticks_pulsada_ < num_ticks_incr_continuo_){
		if ((num_ticks_pulsada_ % num_ticks_incr_uno) == 0)
		    ++x_;
	    }
	    else
		++x_;
	}
    }
    else 
	ultima_tecla_pulsada_ = Tecla_pulsada::ninguna;
}


template <typename L, typename T>
void User_choose_number<L, T>::print2(uint8_t x)
{
    lcd_.cursor_pos(col_, row_);
    lcd_ << std::setw(2) << static_cast<uint16_t>(x);
    lcd_.cursor_pos(col_ + 1, row_);
}

template <typename L, typename T>
void User_choose_number<L, T>::print4(uint16_t x)
{
    lcd_.cursor_pos(col_, row_);
    lcd_ << std::setw(4) << x;
    lcd_.cursor_pos(col_ + 3, row_);
}


}// namespace

#endif
