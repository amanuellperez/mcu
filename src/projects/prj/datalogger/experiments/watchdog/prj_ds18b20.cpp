#include "prj_main.h"

void Main::print_error(std::ostream& out, Sensor::Result error)
{
    using Result = Sensor::Result;

    switch(error){
	break; case Result::ok: 
			out << "Ok"; 

	break; case Result::not_found:
			out << "ERROR (device not found)\n";

	break; case Result::time_out:
			out << "TIMEOUT\n";

	break; case Result::wrong_CRC:
			out << "WRONG CRC\n";
    }
}
