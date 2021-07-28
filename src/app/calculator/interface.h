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

#include "dev.h"
#include "types.h"

#include <array>


class Interface;

// Dejo las str como id para dar la posibilidad de meterlas en la EEPROM las
// cadenas (aunque sospecho que no merece la pena ya que lo más seguro es que
// tenga que tener continuamente cargadas en memoria las cadenas de teclado)
struct Key {
// Data
    uint8_t str_id;	
    using Cmd = void (Interface::*)();
    Cmd command;

// construction
    static constexpr Key str(uint8_t id){ return Key{id, null_command};}
    static constexpr Key cmd(Cmd cmd){ return Key{null_id, cmd};}

    bool is_command() const {return command != null_command;}
    void execute_command(Interface* app) const { (app->*command)(); }

    // null
    static constexpr uint8_t null_id  = 0xFF;
    static constexpr Cmd null_command = nullptr;
};


class Interface{
public:
    Interface(LCD& lcd, Keyboard& keyboard)
	:lcd_{lcd}, keyboard_{keyboard}, buffer_{nullptr} {}

    void getline(Buffer& buf);

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
    void write(const char* p);
    void redraw_lcd();
    void redraw_lcd_from(Buffer::iterator p);

    void read(); // implementacion de getline

// Data
    static constexpr uint8_t key_return = 4; // '='

    // TODO: parametrizarlo
    // los "?" son comandos.
    // TODO: sqrt( ---> por el símbolo de la raíz. 
    static constexpr const char* key_strings[] = 
	    {"0", ".", ".", "?", "=",
	     "1", "2", "3", "+", "-",
	     "4", "5", "6", "x", "/",
	     "7", "8", "9", "?", "?",
	     "sqrt(", "(", ")", "?", "?"}; // ultima temporal, para probar

    static constexpr std::array<Key, 25> key_commands = {
        Key::str(0) , Key::str(1) , Key::str(2),  Key::str(3),  Key::str(4),  
	Key::str(5) , Key::str(6) , Key::str(7),  Key::str(8),  Key::str(9),  
	Key::str(10), Key::str(11), Key::str(12), Key::str(13), Key::str(14), 
	Key::str(15), Key::str(16), Key::str(17), 
					    Key::cmd(&Interface::DEL_command), 
					    Key::cmd(&Interface::AC_command),
	Key::str(20), Key::str(21), Key::str(22), 
				    Key::cmd(&Interface::to_the_left_command),
				    Key::cmd(&Interface::to_the_right_command)
    };
    
    static constexpr uint8_t debouncing_time = 200; // ms
};



#endif

