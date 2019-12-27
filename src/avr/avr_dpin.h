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

#pragma once

#ifndef __AVR_DPIN_H__
#define __AVR_DPIN_H__
/****************************************************************************
 *
 *   - DESCRIPCION: Clases para manejar pines del AVR
 *
 *   - COMENTARIOS: 
 *	    Aquí defino los pin dinámicos. En principio son mejores los pines
 *	definidos en tiempo de compilación (Pin<14> pin) ya que son totalmente
 *	eficientes. Sin embargo, usar estos pines hace que todo el resto del
 *	código tenga que ser templates teniendo que meterlo todo en archivo
 *	de cabecera. Al definir DPin e ir a reescribir la clase de
 *	EEPROM_25LC256 me encontré con que tenía que migrar todo el código del
 *	.cpp al .h mostrando toda la implementación lo cual no parece buena
 *	idea. Además, migrar LCD_HD44780 seguramente que quedaría bastante
 *	feo.
 *
 *	Por eso suministro 2 tipos de pines: Pin y DPin, el primero es
 *	estático, totalmente eficiente, y el segundo no lo es, pero es más
 *	sencillo de manejar y crear clases con él. Siempre que se pueda usar
 *	mejor Pin.
 *
 *   - HISTORIA:
 *           alp  - 24/07/2017 Escrito
 *		    04/04/2018 square_wave y mejorada eficiencia.
 *		    02/01/2019 DPin_de_salida
 *		    27/01/2019 Reestructuro. Elimino square_wave
 *			       Si se quiere reutilizar ver versión 0.0
 *
 ****************************************************************************/
#include "avr_cfg.h"
#include "avr_time.h"	

namespace avr{

/***************************************************************************
 *			    PINES DEL AVR
 ***************************************************************************/

// Clase de ayuda para poder definir arrays de pins
struct Pin_number{
    constexpr Pin_number(uint8_t vv):v{vv}{}

    uint8_t v;	// valor del número de pin
};


inline namespace literals{
// Le paso un unsigned long long porque así lo especifica el estandar,
// pero la función espera un uint8_t!!!
inline constexpr Pin_number operator""_pin(unsigned long long num_pin)
{ return Pin_number{(uint8_t) num_pin}; }

}


/*!
 *  \brief  Un pin del AVR.
 *
 */
class DPin{
public:
    /// Indicamos el número de pin al que corresponde
    constexpr explicit DPin(uint8_t num_pin):num_pin_{num_pin},
		      port_{*PORT[num_pin]}
		    , ddr_{*DDR[num_pin]}
		    , pin_{*PIN[num_pin]}
		    , bit_mask_(BIT_MASK[num_pin])
		    { }


    /// Indicamos el número de pin que le corresponde. Este constructor
    /// está pensado para usarlo con arrays.
    /// Ejemplo: DPin leds[4] = {9_pin,10_pin,11_pin,12_pin};
    constexpr DPin(Pin_number num_pin):DPin{num_pin.v}{}

    // no tiene ningún sentido copiar pines
    // Dejo el poder construir un pin a partir de otro para poder 
    // crear arrays de pines: DPin leds[4] = {9,10,11,12};
//    DPin(const DPin&)		= delete;
    DPin& operator=(const DPin&)	= delete;

    /// Definimos el pin de salida
    void de_salida() 
    { (ddr_) |= bit_mask_; }

    /// Definimos el pin como de entrada con la pull-up resistor
    void de_entrada_con_pullup() 
    {
	(ddr_) &= ~bit_mask_;	// de entrada
	(port_) |= bit_mask_;	// enables pull-up resistor
    }

    /// Definimos el pin como de entrada sin la pull-up resistor
    void de_entrada_sin_pullup() 
    {
	(ddr_) &= ~bit_mask_;	// de entrada
	(port_) &= ~bit_mask_;// disabel pull-up resistor
    }

    // --------------------------------------------------------
    // Funciones de lectura (solo válidas para pins de entrada)
    // --------------------------------------------------------
    /// Leemos el valor del pin (0 ó 1)
    uint8_t valor() const
    {return pin_ & bit_mask_;}

    /// Devuelve si el bit es 0 o no. 
    bool is_zero() const {return !valor();}

//    /// Devuelve si el bit es 0 o no. Sinónimo: is_zero
//    bool is_low() const {return is_zero();}

    /// Devuelve si el bit es 1 o no. 
    bool is_one() const {return valor();}

//    /// Devuelve si el bit es 1 o no. Sinónimo: is_one
//    bool is_high() const {return is_one();}

    // ----------------------------------------------------------
    // Funciones de escritura (sólo válidas para pins de salida)
    // ----------------------------------------------------------
    /// Escribimos un 1 en el pin. 
    ///	
    /// TODO: revisar comentarios. Al definir como constexpr el constructor
    /// pasa a ser eficiente!!!
    /// Esta función es un poco ineficiente. Para verlo, probar a generar una
    /// onda cuadrada:
    ///	while(1){
    ///	    PORTD |= (1 << 6);	<===> pin.write_one();
    ///     _delay_ms(1);	<===> wait_ms(1);
    ///     PORTD &= ~(1 << 6)  <===> pin.write_zero();
    ///     _delay_ms(1);	<===> wait_ms(1);
    /// }
    ///
    /// Al ejecutar estos dos códigos y medir en el osciloscopio obtengo la 
    /// siguiente tabla:
    ///		    |    PORTD	    |       write
    ///  -----------+---------------+----------------
    ///    delay_ms |    2.030	    |    2.160
    ///             |   20.23       |   20.36
    ///  -----------+---------------+----------------
    ///    wait_ms  |    2.056      |    2.180
    ///             |   20.33       |   20.40
    ///
    ///    Donde la fila de arriba es para un delay de 1 ms, y la de abajo
    /// para un delay de 10 ms. Al sustituir PORTD por write se incrementa
    /// el tiempo de ejecución en 130 us!!! Al sustituir delay por wait se
    /// incrementa en 25 us. Claramente hay que mejorar la eficiencia de estos
    /// writes.
    ///
    /// Por culpa de estas mediciones opto por escribir square_wave en
    /// ensamblador.
    ///
    /// PEOR: 
    /// Resulta que al implementar el sensor DHT22 observo que write_one es
    /// muy ineficiente. Para verlo probar a escribir:
    /// pin.write_zero();
    /// pin.write_one();
    ///
    /// Al medir en el osciloscopio tarda unos 80us. Pero si escribo
    /// auto& port = pin_debug.port_;
    /// auto pos = pin_debug.pos_;
    /// port &= ~(1 << pos);
    /// port |= (1 << pin_debug.pos_);
    /// Pasan a durar 2us!!! 
    /// Al probar a modificar esto resulta que es el acceso a pos_ el que me
    /// ralentiza, por eso es necesario definir pos!!!
    //
    void write_one()
    { (port_) |= bit_mask_; }

    /// Escribimos un 0 en el pin. Sinónimo: low()
    void write_zero() 
    { (port_) &= ~bit_mask_; }

    /// Cambiamos el valor del pin (de 0 a 1 ó de 1 a 0).
    void flip_bit() 
    { (port_) ^= bit_mask_; }

    /// Escribimos en el pin. Un valor distinto de cero es 1.
    /// Esta función es un pelín más ineficiente que set_bit/clear_bit.
    /// El operador = es confuso, ya que:
    /// pin = 1, se puede entender como que queremos escribir un 1 en el 
    /// pin, o como que queremos asociar el pin con el pin 1 del hardware.
    /// (de hecho, he llegado a confundirlo en la realidad!). Por eso,
    /// en lugar de operator= uso write.
    // DPin& operator=(Bit x)
    void write(uint8_t x)
    {
	if(x) write_one();
	else  write_zero();
    }


private:
    // En principio con num_pin_ vale para saber port, ddr y pin. Sin embargo
    // las mediciones que hice (ver write_one) dan la impresión de que es
    // ineficiente. Por eso memorizo port, ddr, pin y pos. Si memorizo estos
    // valores no es necesario num_pin_. Solo necesito conocer num_pin_ en
    // square_wave. Como de momento no se hacer esta función en asm pasandole
    // el port y el ddr, memorizo num_pin. Desperdicio algo de espacio, pero
    // para empezar vale.
    // TODO: hacer más eficiente esto. Esta clase es muy básica. Recordar
    // "don't pay for what you don't use". Si no uso square_wave, estoy siendo
    // penalizado de ya que desperdicio memoria con num_pin.
    const uint8_t num_pin_;	
    volatile uint8_t& port_;
    volatile uint8_t& ddr_;
    volatile uint8_t& pin_;
    uint8_t bit_mask_;	// mascara para acceder al bit
};

/*!
 *  \brief  Creamos un pin que es de salida
 *
 */
class DPin_de_salida{
public:
   constexpr explicit DPin_de_salida(uint8_t num_pin)
       :pin_{num_pin}
   {
       pin_.de_salida();
   }


    constexpr DPin_de_salida(Pin_number num_pin):pin_{num_pin}{}

    DPin_de_salida& operator=(const DPin_de_salida&)	= delete;

    void write_one()	{pin_.write_one();}
    void write_zero()	{pin_.write_zero();}
    void flip_bit()	{pin_.flip_bit();}

//    DPin_de_salida& write(Bit x)
//    {
//	pin_.write(x);
//	return *this;
//    }


private:
   DPin pin_;
};

// Funciones varias
inline void pulso_1us(DPin& pin)
{
    pin.write_one();
    wait_us(1);		// uso de wait_us: depende de F_CPU!!! (en .h)
    pin.write_zero();
}


inline void pulso_negativo_1us(DPin& pin)
{
    pin.write_zero();
    wait_us(1);		// uso de wait_us: depende de F_CPU!!! (en .h)
    pin.write_one();
}

}// namespace avr


#endif

