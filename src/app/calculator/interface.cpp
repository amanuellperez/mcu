// Copyright (C) 2021 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// Conectar un teclado de 5 x 5
// El teclado es una matriz de pulsadores. En las filas conectamos diodos para
// evitar cortocircuitos, y en las columnas al conectar los pines los
// configuramos con pullup resistor.
// Sacamos la salida por un LCD
#include "interface.h"
#include "buffer.h"

void Interface::redraw_lcd_from(Buffer::iterator p)
{
    lcd_.cursor_pos(0,0);

    uint8_t i = 0;
    for (; p != buffer_->end() and i < LCD::cols(); ++p, ++i)
	lcd_ << *p;

    for (; i < LCD::cols(); ++i)
	lcd_ << ' ';  // lcd_clear_row(1).from(i);


    redraw_cursor();
}


void Interface::redraw_lcd()
{
    if (lcd_p0_ > buffer_p_)
	lcd_p0_ = buffer_p_;

//    else if (lcd_p0_ + LCD::cols() > buffer_p_) { }

    else if (lcd_p0_ + LCD::cols() < buffer_p_)
	lcd_p0_ = buffer_p_ - LCD::cols();

    redraw_lcd_from(lcd_p0_);
}

void Interface::reset()
{
    lcd_.clear();
    lcd_p0_ = buffer_->begin();

    buffer_->clear();
    buffer_p_ = buffer_->begin();
}

void Interface::write(const char* p)
{
    uint8_t n = push_back(*buffer_, p);

    buffer_p_ += n;

    if (cursor_x() < LCD::cols())
	lcd_ << p;

    else
	redraw_lcd();
}


void Interface::DEL_command()
{
    if (buffer_p_ != buffer_->begin()){
	--buffer_p_;
	buffer_->remove(buffer_p_);
	redraw_lcd();
    }

}

void Interface::AC_command()
{
    reset();
}

void Interface::to_the_right_command()
{
    if (buffer_p_ != buffer_->end()){
	++buffer_p_;

	if (cursor_x() < LCD::cols())
	    redraw_cursor();

	else
	    redraw_lcd();
    }
}

void Interface::to_the_left_command()
{
    if (buffer_p_ != buffer_->begin()){
	--buffer_p_;

	if (buffer_p_ > lcd_p0_)
	    redraw_cursor();

	else
	    redraw_lcd();

    }
}

void Interface::getline(Buffer& buffer0)
{
    buffer_ = &buffer0;

// init
    reset();
    lcd_.cursor_on();

    // Esperamos a que se pulse una tecla ignorando el bouncing de '\n'
    // (última tecla que pulsamos)
    uint8_t key{};
    while ((key = keyboard_.getkey()) == key_return)
    { }


// read
    while (buffer_->size() != buffer_->max_size()){
	if (key == key_return)
	    break;

	if (key_commands[key].is_command())
	    key_commands[key].execute_command(this);

	else {
	    const char* p = key_strings[key_commands[key].str_id];
	    write(p);

        }

	wait_ms(debouncing_time);

	key = keyboard_.getkey();
    }

    lcd_.cursor_off();
    lcd_.clear();
}



