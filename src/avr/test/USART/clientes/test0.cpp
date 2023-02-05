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

// Esto es copia de https://en.wikibooks.org/wiki/Serial_Programming/termios

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include <string.h> // needed for memset

int main(int argc,char** argv)
{
    struct termios tio;
    int tty_fd;

    unsigned char c='D';

    memset(&tio,0,sizeof(tio));
    tio.c_iflag=0;
    tio.c_oflag=0;
    tio.c_cflag=CS8|CREAD|CLOCAL;           // 8n1, see termios.h for more information
    tio.c_lflag=0;
    tio.c_cc[VMIN]=1;
    tio.c_cc[VTIME]=5;

// O_NONBLOCK might override VMIN and VTIME, so read() may return immediately.
    tty_fd=open("/dev/ttyUSB0", O_RDWR | O_NONBLOCK);        
    cfsetospeed(&tio,B9600);            // 115200 baud
    cfsetispeed(&tio,B9600);            // 115200 baud

    tcsetattr(tty_fd,TCSANOW,&tio);
    while (c!='q')
	if (read(tty_fd,&c,1)>0)        
	    write(STDOUT_FILENO,&c,1);// if new data is available on the serial port, print it out

    close(tty_fd);
}
