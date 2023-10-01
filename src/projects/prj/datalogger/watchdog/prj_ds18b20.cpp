#include "prj_main.h"

void Main::print_error(std::ostream& out, Sensor::Errno error)
{
    using Errno = Sensor::Errno;

    switch(error){
	break; case Errno::ok: 
			out << "Ok"; 

	break; case Errno::not_found:
			out << "ERROR (device not found)\n";

	break; case Errno::time_out:
			out << "TIMEOUT\n";

	break; case Errno::wrong_CRC:
			out << "WRONG CRC\n";
    }
}
