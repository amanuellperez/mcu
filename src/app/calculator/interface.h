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
 *
 *  - HISTORIA:
 *    A.Manuel L.Perez
 *    17/07/2021 v0.0
 *
 ****************************************************************************/

#include "types.h"


// meto Interface dentro de calc, para poder definir Interface en dev.h
namespace calc{

template <typename LCD_t, typename Keyboard_t>
class Interface{
public:
    using LCD = LCD_t;
    using Keyboard = Keyboard_t;
    using Code = typename Keyboard_t::Code;

    Interface(LCD& lcd, Keyboard& keyboard)
	:lcd_{lcd}, keyboard_{keyboard}, 
	 buffer_{nullptr}
    {}


    /// Pantalla inicial que mostramos al encender la calculadora.
    void initial_screen(); 

    void getline(Buffer& buf, bool error);

private:
// Data
    LCD& lcd_;
    Keyboard& keyboard_;

    Buffer* buffer_;

// cursor: (???) Hacer clase Cursor?
    Buffer::iterator buffer_p_; // caracter que apunta el cursor
    Buffer::iterator lcd_p0_;	// primer caracter que aparece en el LCD

    uint8_t cursor_x() {return static_cast<uint8_t>(buffer_p_ - lcd_p0_);}

    void redraw_cursor() {lcd_.cursor_pos(cursor_x(), 0);}

// comandos de edición
    void DEL_command();
    void AC_command();
    void to_the_right_command();
    void to_the_left_command();

    void reset();
    void write(char c);
    void write(const char* p);
    void redraw_lcd();
    void redraw_lcd_from(Buffer::iterator p);

    void read(); // implementacion de getline

    // Traducimos c en el simbolo a mostrar en pantalla
    void print_lcd(char c);
    using symbol = dev::HD44780_charset_A00;

// Data
    static constexpr uint8_t key_return = '=';
    
    static constexpr uint8_t debouncing_time = 200; // ms
};


template <typename L, typename K>
void Interface<L, K>::print_lcd(char c)
{
    if (c == 's')
	lcd_.screen().print(symbol::of("√"));
    else
	lcd_ << c;

}

template <typename L, typename K>
void Interface<L, K>::redraw_lcd_from(Buffer::iterator p)
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

template <typename L, typename K>
void Interface<L, K>::redraw_lcd()
{
    if (lcd_p0_ > buffer_p_)
	lcd_p0_ = buffer_p_;

//    else if (lcd_p0_ + LCD::cols() > buffer_p_) { }

    else if (lcd_p0_ + LCD::cols() <= buffer_p_)
	lcd_p0_ = buffer_p_ - (LCD::cols() - 1);

    redraw_lcd_from(lcd_p0_);
}

template <typename L, typename K>
void Interface<L, K>::reset()
{
    buffer_->clear();

    lcd_p0_   = buffer_->begin();
    buffer_p_ = buffer_->begin();
}


// TODO: ¿mejorarla? De momento, por rapidez, reutilizo const char*
template <typename L, typename K>
void Interface<L, K>::write(char c)
{
    char tmp[2];
    tmp[0] = c;
    tmp[1] = '\0';
    write(tmp);
}

template <typename L, typename K>
void Interface<L, K>::write(const char* str)
{
    if (buffer_p_ == buffer_->end()){
	uint8_t n = push_back(*buffer_, str);
	buffer_p_ += n;
    }

    else{
	buffer_p_ = insert(*buffer_, buffer_p_, str);
    }

    redraw_lcd();
}


template <typename L, typename K>
void Interface<L, K>::DEL_command()
{
    if (buffer_p_ != buffer_->begin()){
	if (buffer_p_ == buffer_->end())
	    --buffer_p_;

	buffer_->remove(buffer_p_);
	redraw_lcd();
    }

}

template <typename L, typename K>
void Interface<L, K>::AC_command()
{
    reset();
    lcd_.clear();
}

template <typename L, typename K>
void Interface<L, K>::to_the_right_command()
{
    if (buffer_p_ != buffer_->end()){
	++buffer_p_;

	if (cursor_x() < LCD::cols())
	    redraw_cursor();

	else
	    redraw_lcd();
    }
}

template <typename L, typename K>
void Interface<L, K>::to_the_left_command()
{
    if (buffer_p_ != buffer_->begin()){
	--buffer_p_;

	if (buffer_p_ > lcd_p0_)
	    redraw_cursor();

	else
	    redraw_lcd();

    }
}

template <typename L, typename K>
void Interface<L, K>::initial_screen()
{
    lcd_.clear();
    lcd_.cursor_pos(lcd_.cols() - 1, 1);
    lcd_ << '0';
    lcd_.cursor_pos(0,0);
    lcd_.cursor_on();

}

// Hay 2 pantallas: la pantalla normal y la pantalla de error.
// El parámetro 'error' nos indica en qué pantalla estamos.
template <typename L, typename K>
void Interface<L, K>::getline(Buffer& buffer0, bool error)
{
    buffer_ = &buffer0;

    reset();

    // Esperamos a que se pulse una tecla ignorando el bouncing de '\n'
    // (última tecla que pulsamos)
    uint8_t key{};
    while ((key = keyboard_.getchar()) == key_return)
    { }

    if (error)
	lcd_.screen().clear_row(1);
    
    lcd_.cursor_on();

    read();
    buffer_->push_back('='); // caracter de terminación

    lcd_.cursor_off();
}


template <typename L, typename K>
void Interface<L, K>::read()
{
    while (1) {
	uint8_t c = keyboard_.getchar();

	switch(c){
	    case key_return: return;
	    case Code::del: DEL_command(); break;
            case Code::AC: AC_command(); break;
            case Code::left: to_the_left_command(); break;
	    case Code::right: to_the_right_command(); break;
	    // case Code::ANS: TODO ANS_command();
	    default: 
		if (buffer_p_ != buffer_->me())
		    write(c);
		break;

        }

	wait_ms(debouncing_time);
    }
}

}// namespace

#endif

