// Copyright (C) 2019-2020 Manuel Perez 
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

// Es más eficiente y más sencillo de implementar usando los puertos
// (ver ejemplo de port/POV_toy). Sin embargo, es más fácil soldar con estas
// conexiones.
#include <avr_atmega.h>

namespace mcu = atmega;

class Main_app{
public:
    void run();

private:
    atmega::Output_pin<15> p1_;
    atmega::Output_pin<16> p2_;
    atmega::Output_pin<17> p3_;
    atmega::Output_pin<18> p4_;
    atmega::Output_pin<19> p5_;
    atmega::Output_pin<23> p6_;
    atmega::Output_pin<24> p7_;
    atmega::Output_pin<25> p8_;

    void POVDisplay(uint8_t fila_glyph);
    void clear();
};

void Main_app::POVDisplay(uint8_t fila_glyph)
{
    p1_.write(fila_glyph & 0b00000001);
    p2_.write(fila_glyph & 0b00000010);
    p3_.write(fila_glyph & 0b00000100);
    p4_.write(fila_glyph & 0b00001000);
    p5_.write(fila_glyph & 0b00010000);
    p6_.write(fila_glyph & 0b00100000);
    p7_.write(fila_glyph & 0b01000000);
    p8_.write(fila_glyph & 0b10000000);

    mcu::Micro::wait_ms(2);
}


void Main_app::clear()
{
    p1_.write_zero();
    p2_.write_zero();
    p3_.write_zero();
    p4_.write_zero();
    p5_.write_zero();
    p6_.write_zero();
    p7_.write_zero();
    p8_.write_zero();
}



void Main_app::run()
{
    while(1){
	POVDisplay(0b00001110);
	POVDisplay(0b00011000);
	POVDisplay(0b10111101);
	POVDisplay(0b01110110);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b00111100);
	POVDisplay(0b01110110);
	POVDisplay(0b10111101);
	POVDisplay(0b00011000);
	POVDisplay(0b00001110);

	clear();
	mcu::Micro::wait_ms(10);
    }
}


int main()
{
    Main_app app;

    app.run();

}



