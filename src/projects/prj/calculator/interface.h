// Copyright (C) 2021-2022 Manuel Perez 
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

#ifndef __INTERFACE_H__
#define __INTERFACE_H__
/****************************************************************************
 *
 *  - DESCRIPCION: Gestionamos la interacción con el usuario.
 *	En esta primera versión de la calculadora, la pantalla la voy a
 *	dividir en dos (filas):
 *	    1. muestra la entrada del usuario.
 *	    2. muestra la salida.
 *
 *	Interface es responsable de la entrada de las operaciones que quiere
 *	hacer el usuario (sumar, restar...), no de la salida,
 *	luego es propietaria de la pantalla de entrada. No debe de tocar la de
 *	salida.
 *
 *  - HISTORIA:
 *    Manuel Perez
 *    17/07/2021 v0.0 Escrito
 *    29/12/2021      Reestructurado y mejorado.
 *
 ****************************************************************************/

#include "buffer.h"
#include <mega.h>


// meto Interface dentro de calc, para poder definir Interface en dev.h
namespace calc{

template <typename LCD_t, typename Keyboard_t, 
	 typename Code_t,
	    size_t N, // tamaño del buffer
	    typename Abb2str_t,
	    uint8_t key_return0 = '\n',
	    uint8_t debouncing_time0 = 200 /* ms */>
struct Interface_cfg{
    using Code              = Code_t;

    using LCD               = LCD_t;
    using Keyboard          = Keyboard_t;
    using Buffer            = Linear_array<N>;
    using Abb2str	    = Abb2str_t;

    static constexpr uint8_t key_return      = key_return0;
    static constexpr uint8_t debouncing_time = debouncing_time0;
};


// Keycode: 
//	El usuario de la calculadora al pulsar una tecla busca:
//	1. imprimir un número o caracter (+, -, x, /)
//	2. ejecutar instrucción de edicion (mover el cursor, borrar
//	   pantalla...)
//	3. pulsa una abreviatura: tecla 'sin' imprime 'sin(', ...
//	4. ejecutar instrucción en la calculadora (tecla mode)
//	Al pulsar una tecla almacenamos lo que quiere hacer el usuario en un
//	byte cuyo significado lo marca Code. Es similar al código ASCII.
//	Sin embargo, hay un error de concepción en ASCII: se habla de caracteres 
//	de control y caracteres imprimibles lo cual es absurdo. Por eso no 
//	los llamo caracteres sino códigos.
//
//  abb2str: contiene un array con la expansión de las abreviaturas ("ANS",
//  "sin", "cos", ...). En principio está pensado para que este array de
//  cadenas esté en memoria flash. 
template < typename Cfg>
class Interface{
public:
    using Code     = Cfg::Code;

    using LCD	   = Cfg::LCD;
    using Keyboard = Cfg::Keyboard;
    using Buffer   = Cfg::Buffer;
    using Abb2str = Cfg::Abb2str;

    Interface(LCD& lcd, Keyboard& keyboard, Abb2str abb2str)
	:lcd_{lcd}, keyboard_{keyboard}, 
	 buffer_{nullptr}, abb2str_{abb2str}
    {}


    /// Pantalla inicial que mostramos al encender la calculadora.
    void initial_screen();  // TODO: esta es del main!!! Moverla al main!
    void getline(Buffer& buf);

private:
// Data
    LCD& lcd_;
    Keyboard& keyboard_;

    Buffer* buffer_;
    Abb2str abb2str_;

// cursor: (???) Hacer clase Cursor?
    Buffer::iterator buffer_p_; // caracter que apunta el cursor dentro del buffer
    Buffer::iterator lcd_p0_;	// primer caracter que aparece en el LCD

    uint8_t cursor_x() {return static_cast<uint8_t>(buffer_p_ - lcd_p0_);}

    void redraw_cursor() {lcd_.cursor_pos(cursor_x(), 0);}

// comandos de edición
    void DEL_command();
    void AC_command();
    void to_the_right_command();
    void to_the_left_command();

// construcción/destrucción en getline
    void buffer_init(Buffer& buffer);
    void buffer_init();
    void buffer_end();

    void lcd_init();
    void lcd_end(){ lcd_.cursor_off();}

// escritura
    void write(char c);
    void write_abbrevation(char key) 
    {
	char tmp[abb2str_.max_size()]; 
	atmega::Micro::strlcpy(tmp, abb2str_[key - Code::first_abb], abb2str_.max_size());
	write(tmp);
    }

    void write(const char* str);

    void write_buffer(char c);
    void write_buffer(const char* p);

    void redraw_lcd();
    void redraw_lcd_from(typename Buffer::iterator p);

    void read(); // implementacion de getline


// screen: es la pantalla donde escribimos. 
//         Estas son las funciones que conocen las dimensiones de la ventana
//         del LCD donde escribimos.
    void clear_screen() {lcd_.terminal().clear_row(0);}
    void cursor_on(uint8_t x, uint8_t y);

// Data
    static constexpr uint8_t key_return = Cfg::key_return;
    
// Frecuencia (realmente periodo) de muestreo del teclado
    static constexpr uint8_t Tclock_keyboard = Cfg::debouncing_time; // ms
};


template <typename Cfg>
void Interface<Cfg>::cursor_on(uint8_t x, uint8_t y)
{
    lcd_.cursor_on();
    lcd_.cursor_pos(x, y);
}



template <typename Cfg>
void Interface<Cfg>::redraw_lcd_from(
    typename Buffer::iterator p)
{
    lcd_.cursor_pos(0,0);

    uint8_t i = 0;
    for (; p != buffer_->end() and i < LCD::cols(); ++p, ++i)
	lcd_.print(*p);
//	lcd_ << *p;

    for (; i < LCD::cols(); ++i)
	lcd_ << ' ';  // lcd_clear_row(0).from(i);


    redraw_cursor();
}

template <typename Cfg>
void Interface<Cfg>::redraw_lcd()
{
    if (lcd_p0_ > buffer_p_)
	lcd_p0_ = buffer_p_;

//    else if (lcd_p0_ + LCD::cols() > buffer_p_) { }

    else if (lcd_p0_ + LCD::cols() <= buffer_p_)
	lcd_p0_ = buffer_p_ - (LCD::cols() - 1);

    redraw_lcd_from(lcd_p0_);
}

template <typename Cfg>
void Interface<Cfg>::buffer_init(Buffer& buffer0)
{
    buffer_ = &buffer0;
    buffer_init();
}


template <typename Cfg>
void Interface<Cfg>::buffer_init()
{
    buffer_->clear();

    lcd_p0_   = buffer_->begin();
    buffer_p_ = buffer_->begin();
}

template <typename Cfg>
void Interface<Cfg>::write(char c)
{
    if (buffer_p_ != buffer_->me())
	write_buffer(c);
}


template <typename Cfg>
void Interface<Cfg>::write(const char* str)
{
    if (buffer_p_ != buffer_->me())
	write_buffer(str);
}


// TODO: ¿mejorarla? De momento, por rapidez, reutilizo const char*
template <typename Cfg>
void Interface<Cfg>::write_buffer(char c)
{
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    write_buffer(tmp);
}

template <typename Cfg>
void Interface<Cfg>::write_buffer(const char* str)
{
    if (buffer_p_ == buffer_->end()){
	uint8_t n = push_back(*buffer_, str);
	buffer_p_ += n;
    }

    else{
	buffer_p_ = insert(*buffer_, buffer_p_, str);
    }

}


template <typename Cfg>
void Interface<Cfg>::DEL_command()
{
    if (buffer_p_ != buffer_->begin()){
	if (buffer_p_ == buffer_->end())
	    --buffer_p_;

	buffer_->remove(buffer_p_);
    }

}

template <typename Cfg>
void Interface<Cfg>::AC_command()
{
    buffer_init();
    lcd_.clear();
}

template <typename Cfg>
void Interface<Cfg>::to_the_right_command()
{
    if (buffer_p_ != buffer_->end())
	++buffer_p_;
}

template <typename Cfg>
void Interface<Cfg>::to_the_left_command()
{
    if (buffer_p_ != buffer_->begin())
	--buffer_p_;
}

template <typename Cfg>
void Interface<Cfg>::initial_screen()
{
    lcd_.clear();
    lcd_.cursor_pos(lcd_.cols() - 1, 1);
    lcd_ << '0';
    lcd_.cursor_pos(0,0);
    lcd_.cursor_on();

}

template <typename Cfg>
void Interface<Cfg>::lcd_init()
{
    lcd_.cursor_on();
    lcd_.cursor_pos(0,0);
}


template <typename Cfg>
void Interface<Cfg>::getline(Buffer& buffer0)
{
    // Esperamos a que se pulse una tecla ignorando el bouncing de '\n'
    // (última tecla que pulsamos)
    uint8_t key{};
    while ((key = keyboard_.getchar()) == key_return)
    { }

    buffer_init(buffer0);
    lcd_init();

    read();

    buffer_end();
    lcd_end();
}


template <typename Cfg>
void Interface<Cfg>::buffer_end()
{
    if (buffer_->size() + 2 > buffer_->max_size()){
	buffer_->clear();
	push_back(*buffer_, "E01: error");  // TODO: numerar errores ???
    }
    else{
	buffer_->push_back(key_return); // caracter de terminación
	buffer_->push_back('\0');	// yparse usa cadenas de C!!! Fundamental!!!
    }
}

template <typename Cfg>
void Interface<Cfg>::read()
{
    while (1) {
	uint8_t key = keyboard_.getchar();

// abreviaturas (pulsas una tecla, pero escribes "ANS" ó "sin(" ...)
	if (Code::first_abb <= key and key <= Code::last_abb)
	    write_abbrevation(key);

	else {
	    switch(key){
		break; case Code::null: break;
		break; case key_return: return;

// comandos de edición
		break; case Code::del : DEL_command(); 
		break; case Code::ac  : AC_command();
		break; case Code::left: to_the_left_command(); 
		break; case Code::right: to_the_right_command();


// caracteres
		break; default: write(key);
	    }
	}

	redraw_lcd();
	atmega::Micro::wait_ms(Tclock_keyboard);
    }
}

}// namespace

#endif

