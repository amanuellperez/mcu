// Copyright (C) 2021 A.Manuel L.Perez 
//           mail: <amanuel.lperez@gmail.com>
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
 *    A.Manuel L.Perez
 *    17/07/2021 v0.0
 *
 ****************************************************************************/

#include "buffer.h"


// meto Interface dentro de calc, para poder definir Interface en dev.h
namespace calc{

// Keycode: 
//	El usuario de la calculador aal pulsar una tecla busca:
//	1. imprimir un número o caracter (+, -, x, /)
//	2. ejecutar instrucción de edicion (mover el cursor, borrar
//	   pantalla...)
//	3. pulsa una abreviatura: tecla 'sin' imprime 'sin(', ...
//	4. ejecutar instrucción en la calculadora (tecla mode)
//	Al pulsar una tecla almacenamos lo que quiere hacer el usuario en un
//	byte cuyo significado lo marca Keycode. Es similar al código ASCII
//	pero hay un error de concepción en ASCII: se habla de caracteres de
//	control y caracteres imprimibles lo cual es un absurdo. Por eso no los
//	llamo caracteres sino códigos (sería mejor Bytecode? A fin de cuentas
//	es lo que es, el Key es para recordar que es una codificación
//	sugerida por al teclado).
template <typename LCD_t, typename Keyboard_t, 
	 typename Keycode,
	    size_t N, // tamaño del buffer
	    uint8_t key_return0 = '\n',
	    uint8_t debouncing_time0 = 200 /* ms */>
class Interface{
public:
    using Code     = Keycode;

    using LCD	   = LCD_t;
    using Keyboard = Keyboard_t;
    using Buffer   = Linear_array<N>;

    Interface(LCD& lcd, Keyboard& keyboard)
	:lcd_{lcd}, keyboard_{keyboard}, 
	 buffer_{nullptr}
    {}


    /// Pantalla inicial que mostramos al encender la calculadora.
    void initial_screen();  // TODO: esta es del main!!! Moverla al main!
    void getline(Buffer& buf);

private:
// Data
    LCD& lcd_;
    Keyboard& keyboard_;

    Buffer* buffer_;

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

    void write_buffer(char c);
    void write_buffer(const char* p);

    void redraw_lcd();
    void redraw_lcd_from(typename Buffer::iterator p);

    void read(); // implementacion de getline

    // Traducimos c en el simbolo a mostrar en pantalla
    void print_lcd(char c);
    using symbol = dev::HD44780_charset_A00;

// screen: es la pantalla donde escribimos. 
//         Estas son las funciones que conocen las dimensiones de la ventana
//         del LCD donde escribimos.
    void clear_screen() {lcd_.screen().clear_row(0);}
    void cursor_on(uint8_t x, uint8_t y);

// Data
    static constexpr uint8_t key_return = key_return0;
    
// Frecuencia (realmente periodo) de muestreo del teclado
    static constexpr uint8_t Tclock_keyboard = debouncing_time0; // ms
};

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::cursor_on(uint8_t x, uint8_t y)
{
    lcd_.cursor_on();
    lcd_.cursor_pos(x, y);
}


template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::print_lcd(char c)
{
    if (c == 's')
	lcd_.screen().print(symbol::of("√"));
    else
	lcd_ << c;

}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::redraw_lcd_from(
    typename Buffer::iterator p)
{
    lcd_.cursor_pos(0,0);

    uint8_t i = 0;
    for (; p != buffer_->end() and i < LCD::cols(); ++p, ++i)
	print_lcd(*p);
//	lcd_ << *p;

    for (; i < LCD::cols(); ++i)
	lcd_ << ' ';  // lcd_clear_row(1).from(i);


    redraw_cursor();
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::redraw_lcd()
{
    if (lcd_p0_ > buffer_p_)
	lcd_p0_ = buffer_p_;

//    else if (lcd_p0_ + LCD::cols() > buffer_p_) { }

    else if (lcd_p0_ + LCD::cols() <= buffer_p_)
	lcd_p0_ = buffer_p_ - (LCD::cols() - 1);

    redraw_lcd_from(lcd_p0_);
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::buffer_init(Buffer& buffer0)
{
    buffer_ = &buffer0;
    buffer_init();
}


template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::buffer_init()
{
    buffer_->clear();

    lcd_p0_   = buffer_->begin();
    buffer_p_ = buffer_->begin();
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::write(char c)
{
    if (buffer_p_ != buffer_->me()){
	write_buffer(c);
	redraw_lcd();
    }
}


// TODO: ¿mejorarla? De momento, por rapidez, reutilizo const char*
template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::write_buffer(char c)
{
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    write_buffer(tmp);
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::write_buffer(const char* str)
{
    if (buffer_p_ == buffer_->end()){
	uint8_t n = push_back(*buffer_, str);
	buffer_p_ += n;
    }

    else{
	buffer_p_ = insert(*buffer_, buffer_p_, str);
    }

}


template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::DEL_command()
{
    if (buffer_p_ != buffer_->begin()){
	if (buffer_p_ == buffer_->end())
	    --buffer_p_;

	buffer_->remove(buffer_p_);
	redraw_lcd();
    }

}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::AC_command()
{
    buffer_init();
    lcd_.clear();
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::to_the_right_command()
{
    if (buffer_p_ != buffer_->end()){
	++buffer_p_;

	if (cursor_x() < LCD::cols())
	    redraw_cursor();

	else
	    redraw_lcd();
    }
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::to_the_left_command()
{
    if (buffer_p_ != buffer_->begin()){
	--buffer_p_;

	if (buffer_p_ > lcd_p0_)
	    redraw_cursor();

	else
	    redraw_lcd();

    }
}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::initial_screen()
{
    lcd_.clear();
    lcd_.cursor_pos(lcd_.cols() - 1, 1);
    lcd_ << '0';
    lcd_.cursor_pos(0,0);
    lcd_.cursor_on();

}

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::lcd_init()
{
    lcd_.cursor_on();
    lcd_.cursor_pos(0,0);
}


template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::getline(Buffer& buffer0)
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


template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::buffer_end()
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

template <typename L, typename K, typename KC, size_t N, uint8_t kr, uint8_t dt>
void Interface<L, K, KC, N, kr, dt>::read()
{
    while (1) {
	uint8_t c = keyboard_.getchar();

	switch(c){
	    break; case Code::null: break;
	    break; case key_return: return;
	    break; case Code::del : DEL_command(); 
            break; case Code::ac  : AC_command();
            break; case Code::left: to_the_left_command(); 
	    break; case Code::right: to_the_right_command();
	    // case Code::ANS: TODO ANS_command();
	    break; default: write(c);
        }

	wait_ms(Tclock_keyboard);
    }
}

}// namespace

#endif

