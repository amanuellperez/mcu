// Copyright (C) 2019-2020 A.Manuel L.Perez
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

#include "dev_LCD_HD44780_tr.h"

#include <atd_bit.h>
#include <avr_time.h>

// TODO: inicialmente el namespace de este fichero era avr.
// Al migrarlo a dev genera errores de compilación. Para eliminarlos 
// uso este using namespace. Quitarlo.
using namespace avr;

namespace dev{


// Escribe solo los bits d4-d7 de 'd' (los más significativos)
void LCD_HD44780::write_d4_d7( uint8_t rs
		, uint8_t rw
		, uint8_t d)
{
    d >>= 4;
    for(uint8_t i = 4; i < 8; ++i){
	D[i].write(d & 0x01);
	d >>= 1;
    }


    RS.write(rs);
    RW.write(rw);

    pulso_E();
}



void LCD_HD44780::write_d8(uint8_t rs
	    , uint8_t rw
	    , uint8_t d)
{
    RS.write(rs);
    RW.write(rw);

    for(uint8_t i = 0; i < 8; ++i){
	D[i].write(d & 0x01);
	d >>= 1;
    }


    pulso_E();
}


void LCD_HD44780::write_d4(uint8_t rs
	    , uint8_t rw
	    , uint8_t d)
{
    RS.write(rs);
    RW.write(rw);

    uint8_t x = d;
    x >>= 4;
    for(uint8_t i = 4; i < 8; ++i){
	D[i].write(x & 0x01);
	x >>= 1;
    }

    pulso_E();


    x = d;
    for(uint8_t i = 0; i < 4; ++i){
	D[i + 4].write(x & 0x01);
	x >>= 1;
    }


    pulso_E();

}


bool LCD_HD44780::esta_ocupado8()
{
    // marcamos el pin 7 de lectura
    D[7].de_entrada_sin_pullup();

    RS.write_zero();
    RW.write_one();
    _delay_us(1);

    bool res = false;

    E.write_one();
//    _delay_us(1); // La datasheet dice que esta operación es instantanea
		
    // Fundamental: leer D[7] cuando E = 1 (pag. 32, datasheet)
    if(D[7].is_one()) 
	res = true;
    E.write_zero();
    
    // Dejamos el pin como estaba: de salida
    D[7].de_salida();

    return res;
}
    

bool LCD_HD44780::esta_ocupado4()
{
    // Vamos a leer de los pines
    for (uint8_t i = 0; i < 8; ++i)
	D[i].de_entrada_sin_pullup();

    RS.write_zero();
    RW.write_one();
    _delay_us(1);

    bool res = false;

    E.write_one();
    _delay_us(1); 
		
    // Fundamental: leer D[7] cuando E = 1 (pag. 32, datasheet)
    if(D[7].is_one()) 
	res = true;
    E.write_zero();
    
    // Enviamos otro pulso a E para descartar el resto de los datos D0-D4
    // (que se envían en el segundo paquete)
    E.write_one();
    _delay_us(1);
    E.write_zero();

    // Dejamos los pines como estaban
    for (uint8_t i = 0; i < 8; ++i)
	D[i].de_salida();
    
    return res;
}


// Esta función está implementada de acuerdo a la figura 9 de la datasheet.
// Observar que cuando se lee es cuando el pin E está en 1, no hay que enviar
// un pulso como en el caso de escribir.
uint8_t LCD_HD44780::read_d4()
{
    // Vamos a leer de los pines
    for (uint8_t i = 0; i < 8; ++i)
	D[i].de_entrada_sin_pullup();

    RS.write_one();
    RW.write_one();

    uint8_t res = 0x00;

    E.write_one();
    _delay_us(1);

    if (D[7].valor()) atd::write_one_bit<7>(res);
    if (D[6].valor()) atd::write_one_bit<6>(res);
    if (D[5].valor()) atd::write_one_bit<5>(res);
    if (D[4].valor()) atd::write_one_bit<4>(res);

    E.write_zero();
    _delay_us(1);
    E.write_one();
    _delay_us(1);
    
    if (D[7].valor()) atd::write_one_bit<3>(res);
    if (D[6].valor()) atd::write_one_bit<2>(res);
    if (D[5].valor()) atd::write_one_bit<1>(res);
    if (D[4].valor()) atd::write_one_bit<0>(res);


    E.write_zero();

    // Dejamos los pines como estaban
    for (uint8_t i = 0; i < 8; ++i)
	D[i].de_salida();
    
    return res;
}




// Inicializamos el LCD con la interfaz de 8 bits.
// La inicializazción está hecha de acuerdo a la figura 23, de la página 45
// de la datasheet.
//
// OJO: no llamar a las funciones display_on() ... ya que todas estas
// funciones dan por supuesto que el display está inicializado!!! No
// funcionaría!!! (realmente sí funciona al cargar el display por primera vez,
// pero al desconectarlo de corriente y reconectarlo deja de funcionar, lo
// cual al principio despista mucho qué es lo que ha ocurrido).
void LCD_HD44780::init8()
{
    _delay_ms(50);   // wait for more than 40 ms

    // 8 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 23, pg 46
    // TODO: revisar esto. En este caso están conectados los 8 cables, no
    // tiene sentido usar write_d4_d7!!!
    write_d4_d7(0,0, 0x30);	// Function set: Interface is 8 bits long
    _delay_us(4500); // wait min 4.1ms

    write_d4_d7(0,0, 0x30); // Function set: Interface is 8 bits long
    _delay_us(150);  // wait for more than 100 us

    // Function set: Interface is 8 bits long
    write_d4_d7(0,0, 0x30);
    
    // definimos display 16 x 2 
    // function_set(true, true, true);
    write_d(0,0, 0x38);

    // display_on();
    write_d(0,0, 0x0C);

    // clear();
    write_d(0,0, 0x01);

    // entry mode set
    // entry_mode(incrementa_cursor_, shift_display_);
    write_d(0, 0, 0x06);
}


// Inicializamos el LCD con la interfaz de 4 bits.
//
// La inicializazción está hecha de acuerdo a la figura 24, de la página 46
// de la datasheet.
//
// Fundamental:
// 1.- Es fundamental medir bien los tiempos entre instrucciones. Por error
// use wait_ms y no funcionaba (ya que esta wait no espera exactamente ese
// tiempo sino más).
//
// 2.- no llamar a las funciones display_on() ... ya que todas estas
// funciones dan por supuesto que el display está inicializado!!! No
// funcionaría!!! (realmente sí funciona al cargar el display por primera vez,
// pero al desconectarlo de corriente y reconectarlo deja de funcionar, lo
// cual al principio despista mucho qué es lo que ha ocurrido).
void LCD_HD44780::init4()
{
    delay_ms(50);   // wait for more than 40 ms after Vcc rises to 2.7V

    // 8 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 23, pg 46
    write_d4_d7(0,0, 0x30);	// Function set: Interface is 8 bits long
    delay_us(4500);		// wait min 4.1ms

    write_d4_d7(0,0, 0x30); // Function set: Interface is 8 bits long
    delay_us(4500);	    // wait min 100us <--- TODO? solo 100us no 4500!!

    
    write_d4_d7(0,0, 0x30); // Function set: Interface is 8 bits long
    delay_us(150);	    // Aquí la datasheet no pone que esperes. (TODO?)
 
    // TODO: la datasheet pone que ya se puede mirar BF. Si no se mira
    // entonces the waiting time between instructions is longer than the
    // execution instruction time. Como todas las instrucciones tardan 37 us
    // de momento le añado un delay de 100 us.
    // Function set: Interface is 4 bits long
    write_d4_d7(0,0, 0x20);
    delay_us(100);

    // definimos display 16 x 2 
    // function_set(true, true, true);
    write_d4_d7(0,0, 0x20);
    write_d4_d7(0,0, 0x80);
    delay_us(100);

    // display_on();
    write_d4_d7(0,0, 0x00);
    write_d4_d7(0,0, 0xC0);
    delay_us(100);

    // clear();
    write_d4_d7(0,0, 0x00);
    write_d4_d7(0,0, 0x10);
    delay_us(100);

    // entry mode set
    // entry_mode(incrementa_cursor_, shift_display_);
    write_d4_d7(0,0, 0x00);
    write_d4_d7(0,0, 0x60);
    delay_us(100);
}


void LCD_HD44780::setup_pins()
{
    // Si es 8-bit interface, inicializamos los pins D0-D7
    // En caso contrario, los pines D4-D7
    int num_pin0 = (num_pins_d == 8? 0: 4);

    for(uint8_t i = num_pin0; i < 8; ++i)
	D[i].de_salida();

    RS.de_salida();
    RW.de_salida();
    E.de_salida();
}



// Definimos las conexiones hardware del display.
// Interface 4 bits.
 LCD_HD44780::LCD_HD44780( DPin_RS rs, DPin_RW rw, DPin_E e
			, const DPin_D4& d)
    :RS{rs.v}, RW{rw.v}, E{e.v}
    , D{0_pin,0_pin,0_pin,0_pin
	, Pin_number{d.D4}
	, Pin_number{d.D5}
	, Pin_number{d.D6}
	, Pin_number{d.D7}}
{
    num_pins_d = 4;

    setup(); 
}


// Para realizar una operación en el LCD necesitamos activar E
 void LCD_HD44780::pulso_E()
{
    E.write_one();
    delay_us(1);   // OJO: esto antes tenía que ponerlo a delay_ms(1)???
    E.write_zero();
}



 void LCD_HD44780::write_d(uint8_t rs
	    , uint8_t rw
	    , uint8_t d)
{
    if(es_interface8_bits())
	write_d8(rs, rw, d);
    else
	write_d4(rs, rw, d);
}


// Instrucciones del HD44780
 void LCD_HD44780::set_ddram_address(uint8_t addr)
{
    // addr tiene que tener DB7 = 1
    atd::write_one_bit<7>(addr);

    espera_estar_disponible();
    write_d(0,0, addr);
}


void 
LCD_HD44780::cursor_or_display_shift(bool display_no_cursor, bool to_the_right)
{
    uint8_t d = 0x10;
    if(display_no_cursor)	d |= 0x08;
    if(to_the_right)	d |= 0x04;

    espera_estar_disponible();
    write_d(0,0,d);
}


void 
LCD_HD44780::display_control(bool display_on, bool cursor_on, bool cursor_blink)
{
    uint8_t d = 0x08;
    if(display_on)	d |= 0x04;
    if(cursor_on)	d |= 0x02;
    if(cursor_blink)	d |= 0x01;

    espera_estar_disponible();
    write_d(0,0,d);
}

 void 
LCD_HD44780::entry_mode(bool incrementa_cursor, bool shift_display)
{
    uint8_t d = 0x04;
    if(incrementa_cursor)   d |= 0x02;
    if(shift_display)	    d |= 0x01;

    espera_estar_disponible();
    write_d(0,0,d);
}


 void 
LCD_HD44780::function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		, bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		, bool character_font_5x8) // ¿char de 5x8 ó 5x10?
{
    uint8_t d = 0x20;

    if(interface_8_bits)   d |= 0x10;
    if(tiene_2_filas)	   d |= 0x08;
    if(character_font_5x8) d |= 0x04;

    espera_estar_disponible();
    write_d(0,0,d);
}


bool LCD_HD44780::esta_ocupado()
{
    if(num_pins_d == 8)	
	return esta_ocupado8();
    else 
	return esta_ocupado4();
}


void LCD_HD44780::write_data_to_CG_or_DDRAM(char data)
{
    espera_estar_disponible(); 
    write_d(1,0,data);
}

uint8_t LCD_HD44780::read_data_from_CG_or_DDRAM()
{
    espera_estar_disponible(); 
    return read_d4();
}




 void LCD_HD44780::init()
{
    E.write_zero();

    if(num_pins_d == 8)	
	init8();
    else 
	init4();
}



 void LCD_HD44780::setup()
{
    setup_pins();
    init(); 
}

 void LCD_HD44780::clear_display()	    
{
    espera_estar_disponible();
    write_d(0,0, 0x01);
}

/// Coloca el cursor en (0,0) y si el texto se estuviera moviendo en 
/// el display, lo coloca en su posición inicial.
 void LCD_HD44780::return_home()	    
{
    espera_estar_disponible();
    write_d(0,0, 0x02);
}

}// namespace

