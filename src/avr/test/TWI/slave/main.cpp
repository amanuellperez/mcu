// Copyright (C) 2019-2020 A.Manuel L.Perez <amanuel.lperez@gmail.com>
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

// OJO: para que el TWI funcione a 100kHz es necesario operar
// a 8 MHz!!
// Conectar 2 termómetros LM75 con direcciónes 1 y 2. Muestra
// por USART la temperatura que marcan.
#include "../../../avr_TWI.h"
#include "../../../avr_USART.h"
#include "../../../avr_time.h"
#include "../../../avr_interrupt.h"
#include <algorithm>
#include <array>

constexpr uint8_t buffer_size = 5;

inline void traza(const char* fname)
{
    auto tmp = TWCR;

    avr::UART_iostream uart;
    uart << ">>> " << fname << ": TWCR = " << static_cast<uint16_t>(tmp) << "; TWINT = ";

    if (atd::is_one_bit<TWINT>::of(TWCR))
        uart << "1\n";
    else
        uart << "0\n";
}

inline void traza_twcr()
{
    auto tmp = TWCR;
    avr::UART_iostream uart;
    uart << "\tTWCR = " << static_cast<uint16_t>(tmp) << "\n";
}

// Si fuera secuencial, ejecutaríamos:
//	    int ret = TWI::send_start();
//
//  Pero si es asíncrono, ejecutamos:
//	    TWI::send_start();
//	    ...
//	    if (TWI::return() != running) 
//		int ret = TWI::return();  // resultado de la última operación
//					  // pedida a TWI
enum class TWI_return{
    listening,		// No tiene datos.
    data_nack,
    bus_error,

// slave receiver mode
// -------------------
    get_area_full,	// se está recibiendo un mensaje pero el buffer de 
			// entrada se ha llenado.
    eor,		// end of reading: ha finalizado la recepción de un 
			// mensaje; quedan datos en el buffer de entrada
			// pendientes de leer
    
// slave transmitter mode
// ----------------------
//    put_area_empty,	// La put area está vacía y nos piden datos.
//			// Hay que escribir en ella.
//
//    eow,		// end of writting, everything ok
//    eow_pane		// end of writting, put area not empty. No se han
//			// enviado todos los datos de la put area.
};


// TODO: La idea es que esto no sea más que una ampliacion del TWI con 1 byte
// de buffer. Le ponemos una put/get area a TWI en esta clase y ocultamos el
// protocolo al usuario.
// Responsabilidades de esta clase:
//	1.- Ponerle un buffer al hardware de TWI (que carece de el).
//	    Dos posibilidades:	
//		(1) put area = get area: mismo buffer de entrada/salida.
//		(2) put area != get area. Usamos dos buffers diferentes.
//
//	2.- Ocultar el protocolo al usuario. El usuario no tiene que saber
//	    nada de START/SLA+WR ... /STOP. 
//
// Realmente es un std::streambuf. De momento no lo voy a heredar, pero voy 
// a seguir el mismo planteamiento que con los streambuf, de esa forma las
// dos implementaciones (streambuf y TWI_slave) serán parecidas siendo más
// consistente todo y más sencillo de mantener.
class TWI_slave{
public:
    using TWI = avr::TWI_basic; // TODO: parametrizarlo con esto. De esta forma
				// queda independizado del microcontrolador.
    using iostate= TWI_return;
    using streamsize = uint8_t;

    // Tamaño de la put/get area.
    static constexpr uint8_t buffer_size = ::buffer_size; // TODO: parametro de template

    /// Enables TWI interface definiendo la frecuencia del SCL 
    /// a la que vamos a operar.
    /// f_scl = frecuencia en kilohercios de SCL (tipica: 100 y 400 kHz).
    /// f_clock = frecuencia a la que funciona el reloj del avr.
    template <uint16_t f_scl, uint32_t f_clock = AVR_CLOCK_FREQUENCY_IN_HZ>
    struct on{
	// Enables as a slave in the address SLAVE_ADDRESS.
	template <uint8_t TWI_slave_address>
	static void in()
	{
	    TWI::SCL_frequency_in_kHz<f_scl, f_clock>();
	    avr::enable_all_interrupts();
	    TWI::init_slave<TWI_slave_address,1>(); // 1 = TWI::interrupt_enable()

	    init();
	}
    };

    /// Lee del TWI como máximo N bytes.
    /// En caso de que el dispositivo esté ocupado o el buffer esté vacío
    /// devuelve 0.
    /// Postcondición: get_area() == empty()
    static streamsize read_buffer(std::byte* buffer, streamsize N);

    /// Indica si el dispositivo está ocupado ejecutando una función o no.
    static bool is_busy() {return busy_;}

    /// Estado en el que queda el slave después de ejecutar algo.
    /// Sólo es válido cuando is_busy() == false!!!
    static iostate state() {return state_;}

    // Función que va dentro de la ISR
    static void handle_interrupt();

private:
    static inline iostate state_;
    static inline bool busy_;	

    static inline std::array<std::byte, buffer_size> buffer_;
    static inline std::byte* pa_;   // posición actual de la put/get area.

    // Buffer de entrada
    // -----------------
    // buffer = [begin, end)
    // bytes recibidos por TWI = [begin, pa)
    // buffer disponible para recibir más bytes = [pa, end)
    static constexpr std::byte* in_begin() { return buffer_.begin();}
    static std::byte* in_pa()  { return pa_; }
    static constexpr std::byte* in_end() { return buffer_.end();}

    // Adds n to the next pointer for the input sequence
    static void gbump(int n) { pa_ += n;}

    // Inicializa la get area.
    static void reset_get_area() {pa_ = in_begin();}

    // ¿Esta la get area vacía?
    static bool get_area_empty() { return in_begin() == pa_;}

    // Vuelca la get_area en buf, vaciándola. 
    // Si el tamaño de la get area es mayor que N, no se podrá vaciar del todo
    // quedando datos pendientes de leer en la get area.
    // Return: número de bytes copiados en buf.
    static streamsize dump_input_buffer(std::byte* buf, streamsize N);

//
//    // Put area
//    // --------
//    // La put area es: [pbase(), pptr())
//    static constexpr std::byte* pbase() const {return buffer_.begin();}
//    static constexpr std::byte* pptr()  const {return pa_;}
//    static constexpr std::byte* epptr() const {return buffer_.end();}
//
//
//    // Adds n to the next pointer for the output sequence
//    static void pbump(int n) {pa_ += n;}
//
//    // Number of bytes 
//    static  constexpr streamsize kk() { return epptr() - pptr(); }
//
//    // Inicializa la put area.
//    static void reset_put_area() {pa_ = pbase();}



    // Helper functions
    // ----------------
    static void init()
    {
	state_ = iostate::listening;
	pa_ = nullptr;	// ni leyendo ni escribiendo
	busy_ = true;	// está escuchando
    }

    static void set_busy()
    {
	busy_ = true;
	TWI::interrupt_enable();
    }

    static void set_no_busy()
    {
	TWI::interrupt_disable();  
	busy_ = false;
    }




// Respuesta a los estados de TWI en slave receiver mode
    static void srm_sla_w();
    static void srm_data_ack();
    static void srm_data_nack();
    static void srm_stop_or_repeated_start();


//// Respuesta a los estados de TWI en slave transmitter mode:
    static void stm_sla_r();
    static void stm_data_ack();
    static void stm_data_nack();

// Respuesta a miscellaneous states
    static void bus_error();
    static void unknown_error();
};


// DUDA: la datasheet admite poder responder a esto con nack, esto es, no
// reconocemos la recepción de datos. ¿Cuándo puede ser útil eso?
// ¿el slave está ocupado haciendo cosas y no puede responder y por eso
// responde con NACK? Pero basta con desconectarlo para que no responda a
// SLA+w. <-- pero esto sería como decirle al master: "error: dispositivo
// desconocido", mientras que si responde OK a SLA+W, pero luego te devuelve
// NACK al data le estas diciendo al master: "estoy ocupado, inténtalo más
// tarde".
inline void TWI_slave::srm_sla_w()
{
    reset_get_area();
    TWI::slave_receive_data_with_ACK();
}


inline void TWI_slave::srm_data_ack()
{
    *in_pa() = TWI::data();
    gbump(1);
    if (in_pa() == in_end()){
	set_no_busy();
	state_ = iostate::get_area_full;
    }
    else{
	TWI::slave_receive_data_with_ACK();
    }

}


inline void TWI_slave::srm_data_nack()
{
    set_no_busy();
    TWI::recover_from_bus_error();      
    state_ = iostate::data_nack;
}


inline void TWI_slave::srm_stop_or_repeated_start()
{
    set_no_busy();
    TWI::not_addressed_slave_mode();
    state_ = iostate::eor; 
}





//inline void TWI_slave::stm_sla_r()
//{
//    reset_put_area();
//    set_no_busy();
//    state_ = iostate::put_area_empty;
//}
//
//
//inline void TWI_slave::stm_data_ack()
//{
//    streamsize sz = put_area_size();
//
//    if (sz == 0){
//	state_ = 
//    }
//}
//
//inline void TWI_slave::stm_data_nack()
//{
//    kk
//}



inline void TWI_slave::bus_error()
{
    set_no_busy();
    TWI::recover_from_bus_error();      
    state_ = iostate::bus_error;

}

// Este caso lo tienen así implementado en la app-note, pero
// ¿qué estatus puede ser este desconocido? ¿error de hardware?
// ¿protección contra error de software?
inline void TWI_slave::unknown_error()
{
    // set_no_busy(); (qué hacer???)
    TWI::not_addressed_slave_mode();
    state_ = iostate::listening;
}


// Copiamos los bytes recibidos en el buffer q[0,n_q)
// Esto es: copiamos in.[begin, pa) en q[0,N)
//			p0[0, pe - p0) -> q[0, N)
//			    n_p		    n_q
//
// TODO: esta es una función generica: atd::dump_n(...)
TWI_slave::streamsize TWI_slave::dump_input_buffer(std::byte* buf, streamsize N_buf)
{
    streamsize N_p = in_pa() - in_begin();
    streamsize n = std::min(N_buf, N_p);
	
    std::copy_n(in_begin(), n, buf);

    if (n < N_p)
	pa_ = std::shift_left(in_begin(), in_end(), n);
    else
	reset_get_area();

    return n;
}


//TWI_slave::streamsize TWI_slave::write_put_area(const std::byte* buf, streamsize N)
//{
//    streamsize n = std::min(N, put_area_size());
//
//    std::copy_n(buf, n, *pptr());
//
//}



// Mira a ver si hay datos que leer en el bufer del TWI. 
// En caso de que los haya los lee guardándolos en buffer.
// Si no hay datos a leer, devuelve inmediatamente el control.
// Devuelve el número de bytes leidos.
TWI_slave::streamsize TWI_slave::read_buffer(std::byte* buffer, streamsize N)
{
//    avr::Interrupts_lock lock;

    if (is_busy())
	return 0;
 

    streamsize n = dump_input_buffer(buffer, N);

    if (state_ == iostate::get_area_full){
	TWI::slave_receive_data_with_ACK();
	set_busy();
    }

    else{ // state_ == iostate::eor
	if (get_area_empty()){
	    state_ = iostate::listening;
	    set_busy(); 
	}
    }

    return n;
}

//TWI_slave::streamsize TWI_slave::write_buffer(const std::byte* buffer,
//                                              streamsize N)
//{
//    if (is_busy())
//	return 0;
//
//    return write_put_area(buffer, N);
//}


// TODO: para poder independizarlo del microcontrolador es necesario
// darle un nombre a los estados que dependan del protocolo de TWI!!!
void TWI_slave::handle_interrupt()
{

    using SRM = TWI::ioreturn::slave_receiver_mode;
//    using STM = TWI::iostate::slave_transmitter_mode;

    // TODO: cuando el estado quede pendiente de que el usuario haga algo
    // desactivar TWIE, de esa forma no se llamara a esta función quedando a
    // la espera de que el usuario tome las acciones oportunas.
    switch (TWI::status()){

    // slave receiver mode
    // -------------------
	case SRM::sla_w: 
	    srm_sla_w(); 
	    break;

	case SRM::sla_w_data_ack:
	case SRM::general_call_data_ack:
	    srm_data_ack();
	    break;

	case SRM::sla_w_data_nack:
	case SRM::general_call_data_nack:
	    srm_data_nack();
            break;


	case SRM::stop_or_repeated_start:
	    srm_stop_or_repeated_start();
            break;



    // slave transmitter mode
    // ----------------------
//	case STM::sla_r:
//	    stm_sla_r();
//	    break;
//
//	case STM::data_ack:
//	    stm_data_ack();
//	    break;
//
//	case STM::data_nack:
//	    stm_data_nack();
//	    break;


    // miscellaneous states
    // --------------------
	case TWI::ioreturn::bus_error:
	    bus_error();
            break;

        default: // ¿Qué ha sucedido? ¡¡¡status desconocido!!!
	    unknown_error();
	    break;
    }
}













constexpr uint8_t slave_address = 0x10;

enum class Service{
    error   ,
    unknown ,
    service1, 
    service2
};

constexpr std::byte service1_id {0x34};
constexpr std::byte service2_id {0x87};

Service read_service_name()
{
TODO: ponerle un nombre adecuado a esto:
    while (!TWI_slave::eor()) ... // ojo: qué pasa si se envia algo diferente?

    while (TWI_slave::state() != TWI_slave::iostate::eor){
	if (TWI_slave::state() == TWI_slave::iostate::get_area_full) {
	    uart << "Error: get_area_full!!! buffer muy pequeño!!!\n\n";
TODO: en este caso finalizar la transmisión!!!
	  return Service::error;
	}
    } // while

    if (TWI_slave::state() != TWI_slave::iostate::eor) {
            uart << "Tendría que estar en eor, pero esta en otro estado\n";
TODO: en este caso finalizar la transmisión !!!
	    return Service::error;
    }

    std::byte msg;
    if (TWI_slave::read_buffer(msg, 1) != 1) {
	uart << "No se ha leído nada!\n";
TODO: en este caso finalizar la transmisión !!!
	return Service::error;
    }

    if (msg == service1_id)
	return Service::service1;
    else if (msg == service2_id)
	return Service::service2;

    return Service::unknown;



}



void test_servidor()
{
    avr::UART_iostream uart;
    uart << "test_servidor\n"
         << "-------------\n"
         << "Elegir buffer_size de manera que se garantice que todo el mensaje "
            "entre en el buffer!!!\n\n";

    while (1){

	while (TWI_slave::listening()){
	    // do something
	    uart << '.';
	}

	auto srv_name = read_service_name();
	switch (srv_name){
	    case Service::service1:
		uart << "Recibido service1\n";
		service1(); // envia al master la respuesta adecuada
		break;

// TODO: añadir más
//	    case Service::service2:
//		uart << "Recibido service2\n";
//		service2(); // envia al master la respuesta adecuada
//		break;

	    case Service::unknown:
		uart << "Servicio desconocido!\n";
		break;

	    default:
		uart << "Unknown service\n";
		break;
	}
    }
}




void test_read()
{
    avr::UART_iostream uart;
    uart << "test_read\n"
	 << "---------\n"
	 << "Probar con distintos buffer_size\n\n";

    while (1){
	std::byte buffer[buffer_size];
	
	while (TWI_slave::state() != TWI_slave::iostate::eor){

            if (TWI_slave::state() == TWI_slave::iostate::get_area_full) {
                uint8_t n = TWI_slave::read_buffer(buffer, buffer_size);
                uart << "\nXXX Recibidos " << static_cast<uint16_t>(n)
                     << " datos:\n";
                for (uint8_t i = 0; i < n; ++i)
                    uart << '\t' << std::to_integer<uint16_t>(buffer[i])
                         << '\n';
            }
        } // while

        while (TWI_slave::state() == TWI_slave::iostate::eor){
	if (uint8_t n = TWI_slave::read_buffer(buffer, buffer_size)){
	    uart << "\nFFF Recibidos " << static_cast<uint16_t>(n)  << " datos:\n";
	    for (uint8_t i = 0; i < n; ++i)
		uart << '\t' << std::to_integer<uint16_t>(buffer[i]) << '\n';
	}
	}

	uart << "\n\n------------------------\n";
	
	wait_ms(100);

    }// while(1)

}



int main() 
{
    avr::UART_iostream uart;
    avr::cfg_basica(uart);
    uart.on();

    uart << "Empezando\n";

    TWI_slave::on<50>::in<slave_address>();

    uart << "TWI enable\n";
    
    test_read();
}



ISR(TWI_vect)
{
    TWI_slave::handle_interrupt();
}// ISR



