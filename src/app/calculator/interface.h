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


// ¿Qué hacer cuando se pulsa una tecla?
// 1. Mostrar una cadena en pantalla y añadirla al buffer.
// 2. Ejecutar algún comando de edición (AC, DEL, <-, -> ...)
//
// 1. Dejo las str como id para dar la posibilidad de meterlas en la EEPROM las
// cadenas (aunque sospecho que no merece la pena ya que lo más seguro es que
// tenga que tener continuamente cargadas en memoria las cadenas de teclado)
//
// 2. Quiero poder parametrizar el interfaz cambiando el significado de las
// teclas. Por ello uso identificadores de comando. El interfaz es quien los
// suministra (Interface::Command::DEL, o AC,...). De esta forma se podrá
// cambiar por completo la configuración del teclado sin tener que tocar la
// implementación de Interface.
// 
struct Key {
// Data
    enum class Command {null = 0, DEL, AC, to_the_right, to_the_left};

    uint8_t str_id; 
    Command command;


// construction
    /// Define el indice dentro de un array donde localizar la cadena 
    /// a mostrar al pulsar esta tecla
    static constexpr Key id(uint8_t id0){ return Key{id0, Command::null};}
    static constexpr Key cmd(Command cmd){ return Key{null_id, cmd};}

    bool is_command() const {return command != Command::null;}

    // null
    static constexpr uint8_t null_id  = 0xFF;
};


// meto Interface dentro de calc, para poder definir Interface en dev.h
namespace calc{

template <typename LCD_t, typename Keyboard_t>
class Interface{
public:
    using LCD = LCD_t;
    using Keyboard = Keyboard_t;

    Interface(LCD& lcd, Keyboard& keyboard, const char* const* key_strings,
	    const Key* key_commands)
	:lcd_{lcd}, keyboard_{keyboard}, 
	 buffer_{nullptr}, 
	 key_strings_{key_strings},
	 key_commands_{key_commands}
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
    void execute_command(Key::Command cmd);

    void reset();
    void write(const char* p);
    void redraw_lcd();
    void redraw_lcd_from(Buffer::iterator p);

    void read(); // implementacion de getline

    // Traducimos c en el simbolo a mostrar en pantalla
    void print_lcd(char c);
    using symbol = dev::HD44780_charset_A00;

// Data
    static constexpr uint8_t key_return = 4; // '='

    const char* const* key_strings_;
    const Key* key_commands_;
    //static constexpr std::array<Key, 25> key_commands = {
//    static constexpr Key key_commands[25] = {
//        Key::id(0) , Key::id(1) , Key::id(2),  Key::id(3),  Key::id(4),  
//	Key::id(5) , Key::id(6) , Key::id(7),  Key::id(8),  Key::id(9),  
//	Key::id(10), Key::id(11), Key::id(12), Key::id(13), Key::id(14), 
//	Key::id(15), Key::id(16), Key::id(17), Key::cmd(Key::Command::DEL), 
//					       Key::cmd(Key::Command::AC),
//	Key::id(20), Key::id(21), Key::id(22), Key::cmd(Key::Command::to_the_left),
//					       Key::cmd(Key::Command::to_the_right)
//    };
    
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
    while ((key = keyboard_.getkey()) == key_return)
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
	uint8_t key = keyboard_.getkey();

	if (key == key_return)
	    return;

	if (key_commands_[key].is_command())
	    execute_command(key_commands_[key].command);

	else {
	    if (buffer_p_ != buffer_->me()){
		const char* p = key_strings_[key_commands_[key].str_id];
		write(p);
	    }

        }

	wait_ms(debouncing_time);
    }
}

template <typename L, typename K>
void Interface<L, K>::execute_command(Key::Command cmd)
{
    switch(cmd){
	case Key::Command::DEL: DEL_command(); break;
	case Key::Command::AC: AC_command(); break;
	case Key::Command::to_the_left: to_the_left_command(); break;
	case Key::Command::to_the_right: to_the_right_command(); break;
	case Key::Command::null: break;
    }

}
}// namespace

#endif

