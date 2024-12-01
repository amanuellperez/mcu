// Copyright (C) 2019-2020 Manuel Perez <manuel2perez@proton.me>
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

/****************************************************************************
 *
 * DESCRIPCION
 *	Implementación de HD44780
 *
 * HISTORIA
 *    Manuel Perez
 *    11/12/2022 Cambio el uso de las macros delay_us/ms por funciones de C++.
 *		 CUIDADO: no borrar los comentados delays hasta no haber
 *		 comprobado que funciona todo bien. Es necesario que el
 *		 compilador haga el inline de las funciones delay_us(1), sino
 *		 el delay será más de 1 microsegundo, no sabiendo si
 *		 funcionará.
 *
 ****************************************************************************/
#include <atd_bit.h>
#include <avr_time.h>

namespace dev{
namespace hwd{


// Escribe solo los bits d4-d7 de 'd' (los más significativos)
template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::write_d4_d7( uint8_t rs
		, uint8_t rw
		, uint8_t d)
{
    d >>= 4;
    write_d4(d);

    RS::write(rs);
    RW::write(rw);

    pulse_E<pin_E>();
}


//template <typename P>
//void HD44780_base<P>::write_d8(uint8_t rs
//	    , uint8_t rw
//	    , uint8_t d)
//{
//    RS::write(rs);
//    RW::write(rw);
//
//    write_d8(d);
//
//
//
//    pulse_E();
//}


template <typename M, typename P>
void HD44780_base<M, P>::write_d4(uint8_t d)
{
    D4::write(d & 0x01);
    d >>= 1;
    D5::write(d & 0x01);
    d >>= 1;
    D6::write(d & 0x01);
    d >>= 1;
    D7::write(d & 0x01);
}


//template <typename P>
//void HD44780_base<P>::write_d8(uint8_t d)
//{
//    D0::write(d & 0x01);
//    d >>= 1;
//    D1::write(d & 0x01);
//    d >>= 1;
//    D2::write(d & 0x01);
//    d >>= 1;
//    D3::write(d & 0x01);
//    d >>= 1;
//    D4::write(d & 0x01);
//    d >>= 1;
//    D5::write(d & 0x01);
//    d >>= 1;
//    D6::write(d & 0x01);
//    d >>= 1;
//    D7::write(d & 0x01);
//}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::write_d4(uint8_t rs
	    , uint8_t rw
	    , uint8_t d)
{
    RS::write(rs);
    RW::write(rw);

    uint8_t x = d;
    x >>= 4;
    write_d4(x);

    pulse_E<pin_E>();

    write_d4(d);

    pulse_E<pin_E>();

}

//template <typename P>
//bool HD44780_base<P>::is_busy8()
//{
//    // marcamos el pin 7 de lectura
//    D7::as_input_without_pullup();
//
//    RS::write_zero();
//    RW::write_one();
//    _delay_us(1);
//
//    bool res = false;
//
//    E::write_one();
////    _delay_us(1); // La datasheet dice que esta operación es instantanea
//		
//    // Fundamental: leer D[7] cuando E = 1 (pag. 32, datasheet)
//    if(D7::is_one()) 
//	res = true;
//    E::write_zero();
//    
//    // Dejamos el pin como estaba: de salida
//    D7::as_output();
//
//    return res;
//}
    

template <typename M, typename P>
void HD44780_base<M, P>::D_pins_as_output()
{
    if constexpr (num_D_pins == 8){
	D0::as_output();
	D1::as_output();
	D2::as_output();
	D3::as_output();
    }

    D4::as_output();
    D5::as_output();
    D6::as_output();
    D7::as_output();
}

template <typename M, typename P>
void HD44780_base<M, P>::D_pins_as_input_without_pullup()
{
    if constexpr (num_D_pins == 8){
	D0::as_input_without_pullup();
	D1::as_input_without_pullup();
	D2::as_input_without_pullup();
	D3::as_input_without_pullup();
    }

    D4::as_input_without_pullup();
    D5::as_input_without_pullup();
    D6::as_input_without_pullup();
    D7::as_input_without_pullup();
}


template <typename M, typename P>
template <typename pin_E>
bool HD44780_base<M, P>::is_busy4()
{
    // Vamos a leer de los pines
    D_pins_as_input_without_pullup();

    RS::write_zero();
    RW::write_one();
    //_delay_us(1);
    Micro::wait_us(1);

    bool res = false;

    pin_E::write_one();
    //_delay_us(1); 
    Micro::wait_us(1);
		
    // Fundamental: leer D[7] cuando E = 1 (pag. 32, datasheet)
    if(D7::is_one()) 
	res = true;
    pin_E::write_zero();
    
    // Enviamos otro pulso a E para descartar el resto de los datos D0-D4
    // (que se envían en el segundo paquete)
    pin_E::write_one();
    //_delay_us(1);
    Micro::wait_us(1);
    pin_E::write_zero();

    // Dejamos los pines como estaban
    D_pins_as_output();

    return res;
}


// Esta función está implementada de acuerdo a la figura 9 de la datasheet.
// Observar que cuando se lee es cuando el pin E está en 1, no hay que enviar
// un pulso como en el caso de escribir.
template <typename M, typename P>
template <typename pin_E>
uint8_t HD44780_base<M, P>::read_d4()
{
    // Vamos a leer de los pines
    D_pins_as_input_without_pullup();

    RS::write_one();
    RW::write_one();

    uint8_t res = 0x00;

    pin_E::write_one();
    //_delay_us(1);
    Micro::wait_us(1);

    if (D7::is_one()) atd::write_bit<7>::to<1>::in(res);
    if (D6::is_one()) atd::write_bit<6>::to<1>::in(res);
    if (D5::is_one()) atd::write_bit<5>::to<1>::in(res);
    if (D4::is_one()) atd::write_bit<4>::to<1>::in(res);

    pin_E::write_zero();
    //_delay_us(1);
    Micro::wait_us(1);
    pin_E::write_one();
    //_delay_us(1);
    Micro::wait_us(1);
    
    if (D7::is_one()) atd::write_bit<3>::to<1>::in(res);
    if (D6::is_one()) atd::write_bit<2>::to<1>::in(res);
    if (D5::is_one()) atd::write_bit<1>::to<1>::in(res);
    if (D4::is_one()) atd::write_bit<0>::to<1>::in(res);


    pin_E::write_zero();

    // Dejamos los pines como estaban
    D_pins_as_output();

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
//template <typename P>
//void HD44780_base<P>::init8()
//{
//    _delay_ms(50);   // wait for more than 40 ms
//
//    // 8 bit mode
//    // this is according to the hitachi HD44780 datasheet
//    // figure 23, pg 46
//    // TODO: revisar esto. En este caso están conectados los 8 cables, no
//    // tiene sentido usar write_d4_d7!!!
//    write_d4_d7(0,0, 0x30);	// Function set: Interface is 8 bits long
//    _delay_us(4500); // wait min 4.1ms
//
//    write_d4_d7(0,0, 0x30); // Function set: Interface is 8 bits long
//    _delay_us(150);  // wait for more than 100 us
//
//    // Function set: Interface is 8 bits long
//    write_d4_d7(0,0, 0x30);
//    
//    // definimos display 16 x 2 
//    // function_set(true, true, true);
//    write_d(0,0, 0x38);
//
//    // display_on();
//    write_d(0,0, 0x0C);
//
//    // clear();
//    write_d(0,0, 0x01);
//
//    // entry mode set
//    // entry_mode(incrementa_cursor_, shift_display_);
//    write_d(0, 0, 0x06);
//}


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
template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::init4()
{
    //delay_ms(50);   // wait for more than 40 ms after Vcc rises to 2.7V
    Micro::wait_ms(50);

    // 8 bit mode
    // this is according to the hitachi HD44780 datasheet
    // figure 23, pg 46
    write_d4_d7<pin_E>(0,0, 0x30);	// Function set: Interface is 8 bits long
    Micro::wait_us(4500);

    write_d4_d7<pin_E>(0,0, 0x30); // Function set: Interface is 8 bits long
    Micro::wait_us(4500);

    
    write_d4_d7<pin_E>(0,0, 0x30); // Function set: Interface is 8 bits long
    Micro::wait_us(150);
 
    // TODO: la datasheet pone que ya se puede mirar BF. Si no se mira
    // entonces the waiting time between instructions is longer than the
    // execution instruction time. Como todas las instrucciones tardan 37 us
    // de momento le añado un delay de 100 us.
    // Function set: Interface is 4 bits long
    write_d4_d7<pin_E>(0,0, 0x20);
    Micro::wait_us(100);

    // definimos display 16 x 2 
    // function_set(true, true, true);
    write_d4_d7<pin_E>(0,0, 0x20);
    write_d4_d7<pin_E>(0,0, 0x80);
    Micro::wait_us(100);

    // display_on();
    write_d4_d7<pin_E>(0,0, 0x00);
    write_d4_d7<pin_E>(0,0, 0xC0);
    Micro::wait_us(100);

    // clear();
    write_d4_d7<pin_E>(0,0, 0x00);
    write_d4_d7<pin_E>(0,0, 0x10);
    Micro::wait_us(100);

    // entry mode set
    // entry_mode(incrementa_cursor_, shift_display_);
    write_d4_d7<pin_E>(0,0, 0x00);
    write_d4_d7<pin_E>(0,0, 0x60);
    Micro::wait_us(100);
}

template <typename M, typename P>
void HD44780_base<M, P>::setup_pins()
{
    D_pins_as_output();

    RS::as_output();
    RW::as_output();
}



//// Definimos las conexiones hardware del display.
//// Interface 4 bits.
//template <typename P>
//inline HD44780_base<P>::HD44780_base()
//{
//    setup(); 
//}


// Para realizar una operación en el LCD necesitamos activar E
template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::pulse_E()
{
    pin_E::write_one();
    //delay_us(1);   // OJO: esto antes tenía que ponerlo a delay_ms(1)???
    Micro::wait_us(1);
    pin_E::write_zero();
}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::write_d(uint8_t rs
	    , uint8_t rw
	    , uint8_t d)
{
//    if constexpr (num_D_pins == 8)
//	write_d8(rs, rw, d);
//    else
	write_d4<pin_E>(rs, rw, d);
}


// Instrucciones del HD44780
template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::set_ddram_address(uint8_t addr)
{
    // addr tiene que tener DB7 = 1
    atd::write_bit<7>::to<1>::in(addr);

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0, addr);
}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::set_cgram_address(uint8_t addr)
{
    // bits DB7 = 0 and DB6 = 1
    atd::write_bits<7, 6>::to<0, 1>::in(addr);

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0, addr);
}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::cursor_or_display_shift(bool display_no_cursor, bool to_the_right)
{
    uint8_t d = 0x10;
    if(display_no_cursor)	d |= 0x08;
    if(to_the_right)	d |= 0x04;

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0,d);
}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::display_control(bool display_on, bool cursor_on, bool cursor_blink)
{
    uint8_t d = 0x08;
    if(display_on)	d |= 0x04;
    if(cursor_on)	d |= 0x02;
    if(cursor_blink)	d |= 0x01;

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0,d);
}

template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::entry_mode(bool incrementa_cursor, bool shift_display)
{
    uint8_t d = 0x04;
    if(incrementa_cursor)   d |= 0x02;
    if(shift_display)	    d |= 0x01;

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0,d);
}

template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::function_set(bool interface_8_bits // ¿es 8 bit o 4 bit interface?
		, bool tiene_2_filas    // ¿tiene 2 ó 1 linea?
		, bool character_font_5x8) // ¿char de 5x8 ó 5x10?
{
    uint8_t d = 0x20;

    if(interface_8_bits)   d |= 0x10;
    if(tiene_2_filas)	   d |= 0x08;
    if(character_font_5x8) d |= 0x04;

    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0,d);
}


template <typename M, typename P>
template <typename pin_E>
bool HD44780_base<M, P>::is_busy()
{
//    if constexpr (num_D_pins == 8)	
//	return is_busy8();
//    else 
	return is_busy4<pin_E>();
}


template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::write_data_to_CG_or_DDRAM(uint8_t data)
{
    wait_to_be_available<pin_E>(); 
    write_d<pin_E>(1,0,data);
}


template <typename M, typename P>
template <typename pin_E>
uint8_t HD44780_base<M, P>::read_data_from_CG_or_DDRAM()
{
    wait_to_be_available<pin_E>(); 
    return read_d4<pin_E>();
}



template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::init()
{
    pin_E::write_zero();

//    if constexpr (num_D_pins == 8)	
//	init8();
//    else 
    init4<pin_E>();
}



template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::clear_display()	    
{
    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0, 0x01);
}

/// Coloca el cursor en (0,0) y si el texto se estuviera moviendo en 
/// el display, lo coloca en su posición inicial.
template <typename M, typename P>
template <typename pin_E>
void HD44780_base<M, P>::return_home()	    
{
    wait_to_be_available<pin_E>();
    write_d<pin_E>(0,0, 0x02);
}

}// namespace
}// namespace

