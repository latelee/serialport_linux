/**
 *                      Copyleft (C) 2010  Late Lee
 *        This program is tested on LINUX PLATFORM, WITH GCC 4.x.
 *        The program is distributed in the hope that it will be
 *        useful, but WITHOUT ANY WARRANTY. Please feel free to 
 *        use the program, and I feel free to ignore the related
 *        issues. Any questions or suggestions, or bugs, please 
 *        contact me at
 *        <$ echo -n "aHR0cDovL3d3dy5sYXRlbGVlLm9yZwo=" | base64 -d>
 *        or e-mail to 
 *        <$ echo -n "bGF0ZWxlZUAxNjMuY29tCg==" | base64 -d>
 *        if you want to do this.
 *
 * @file   serialport.c
 * @author Late Lee
 * @date   Mon Jan 10 2011
 * 
 * @brief  Some utils of the serial port, such as open the port, close
 *         the port and setup the port.
 * @note   This is a note.
 * @warning This is a warning.
 * @bug    This is a bug.
 * @todo   To do something.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>

#include "debug-msg.h"

/** if you have a usb-serial converter, specify it to 1 */
#define USB_SERIAL 0


/** 
 * open_port - Open a serial port
 * 
 * @param port : The port number, eg, open_port(1) will open port 1.
 * 
 * @return Return fd if success, otherwise will return -1 with some msg.
 *
 * @note
Each serial port on a UNIX system has one or more device files
(files in the /dev directory) associated with it: 

\verbatim
System         Port 1      Port 2
IRIX          /dev/ttyf1   /dev/ttyf2
HP-UX         /dev/tty1p0  /dev/tty2p0
Solaris/SunOS /dev/ttya    /dev/ttyb
Linux         /dev/ttyS0   /dev/ttyS1
Linux         /dev/ttyUSB0 /dev/ttyUSB1 (usb-serial converter)
Digital UNIX  /dev/tty01   /dev/tty02
(Windows       COM1        COM2)
\endverbatim

We open the port specified by port number, for instance, if you want to open
port 1, you just call the function like this: open_port(1).

 *        
 */
int open_port(int port)
{
    int fd = -1; /* File descriptor for the port, we return it. */
    int ret;
    char device[13] = {0}; /* ??? sizeof("/dev/ttyUSB0")=12 */

    if (port < 1 || port > 4)
        error_ret("Sorry, the port number must be 1~4.");
    if (USB_SERIAL)
        sprintf(device, "/dev/ttyUSB%d", port-1);
    else
        sprintf(device, "/dev/ttyS%d", port-1);
    //printf("%s %d\n", device, sizeof(device));

    fd = open(device, O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
        unix_error_ret("Unable to open the port");

    /* block */
    ret = fcntl(fd, F_SETFL, 0);
    if (ret < 0)
        unix_error_ret("fcntl");
    ret = isatty(STDIN_FILENO);
    if (ret == 0)
        error_ret("Standard input is not a terminal device.");
    debug_msg("Open the port success!\n");
    
    return fd;
}

/** 
 * close_port - Close the port
 * 
 * @param fd : The file description returned by open_port().
 * 
 * @return Return 0 if success, return -1 with some msg if error happens.
 */
int close_port(int fd)
{
    if(close(fd) < 0)
        unix_error_ret("Unable to close the port.");
    debug_msg("Close the port success!\n");

    return 0;
}

/** 
 * setup_port - Configure the port, eg. baud rate, data bits,etc.
 * 
 * @param fd        : The serial port
 * @param speed     : The baud rate
 * @param data_bits : The data bits
 * @param parity    : The parity bits
 * @param stop_bits : The stop bits
 * 
 * @return Return 0 if everything is OK, otherwise -1 with some error msg.
 * @note
Here are termios structure members:

\verbatim
Member      Description 
c_cflag     Control options 
c_lflag     Line options 
c_iflag     Input options 
c_oflag     Output options 
c_cc        Control characters 
c_ispeed    Input baud (new interface) 
c_ospeed    Output baud (new interface) 

\endverbatim
The c_cflag member controls the baud rate, number of data bits, parity, 
stop bits, and hardware flow control. There are constants for all of the 
supported configurations.
Constant Description

\verbatim
CBAUD	Bit mask for baud rate 
B0	0 baud (drop DTR) 
B50	50 baud 
B75	75 baud 
B110	110 baud 
B134	134.5 baud 
B150	150 baud 
B200	200 baud 
B300	300 baud 
B600	600 baud 
B1200	1200 baud 
B1800	1800 baud 
B2400	2400 baud 
B4800	4800 baud 
B9600	9600 baud 
B19200	19200 baud 
B38400	38400 baud 
B57600	57,600 baud 
B76800	76,800 baud 
B115200	115,200 baud 
EXTA	External rate clock 
EXTB	External rate clock 
CSIZE	Bit mask for data bits 
CS5 5	data bits 
CS6 6	data bits 
CS7 7	data bits 
CS8 8	data bits 
CSTOPB	2 stop bits (1 otherwise) 
CREAD	Enable receiver 
PARENB	Enable parity bit 
PARODD	Use odd parity instead of even 
HUPCL	Hangup (drop DTR) on last close 
CLOCAL	Local line - do not change "owner" of port 
LOBLK	Block job control output 
CNEW_RTSCTS CRTSCTS	Enable hardware flow control (not supported on all 
			platforms) 

\endverbatim

The input modes member c_iflag controls any input processing that is done to 
characters received on the port. Like the c_cflag field, the final value 
stored in c_iflag is the bitwise OR of the desired options.


\verbatim
Constant	Description 
INPCK	Enable parity check 
IGNPAR	Ignore parity errors 
PARMRK	Mark parity errors 
ISTRIP	Strip parity bits 
IXON	Enable software flow control (outgoing) 
IXOFF	Enable software flow control (incoming) 
IXANY	Allow any character to start flow again 
IGNBRK	Ignore break condition 
BRKINT	Send a SIGINT when a break condition is detected 
INLCR	Map NL to CR 
IGNCR	Ignore CR 
ICRNL	Map CR to NL 
IUCLC	Map uppercase to lowercase 
IMAXBEL	Echo BEL on input line too long 
\endverbatim

Here are some examples of setting parity checking: @n

No parity (8N1): 
\verbatim
options.c_cflag &= ~PARENB
options.c_cflag &= ~CSTOPB
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS8;
\endverbatim

Even parity (7E1): 
\verbatim
options.c_cflag |= PARENB
options.c_cflag &= ~PARODD
options.c_cflag &= ~CSTOPB
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS7;
\endverbatim

Odd parity (7O1): 
\verbatim
options.c_cflag |= PARENB
options.c_cflag |= PARODD
options.c_cflag &= ~CSTOPB
options.c_cflag &= ~CSIZE;
options.c_cflag |= CS7;
\endverbatim
 */
int setup_port(int fd, int speed, int data_bits, int parity, int stop_bits)
{
    int speed_arr[] = {B115200, B9600, B38400, B19200, B4800};
    int name_arr[] = {115200, 9600, 38400, 19200, 4800};
    struct termios opt;
    int ret=-1;
    int i=0;
    int len=0;

    ret = tcgetattr(fd, &opt);		/* get the port attr */
    if (ret < 0)
        unix_error_ret("Unable to get the attribute");

    opt.c_cflag |= (CLOCAL | CREAD); /* enable the receiver, set local mode */
    opt.c_cflag &= ~CSIZE;			/* mask the character size bits*/

    /* baud rate */
    len = sizeof(speed_arr) / sizeof(int);
    for (i = 0; i < len; i++)
    {
        if (speed == name_arr[i])
        {
            cfsetispeed(&opt, speed_arr[i]);
            cfsetospeed(&opt, speed_arr[i]);
        }
        if (i == len)
            error_ret("Unsupported baud rate.");
    }
    
    /* data bits */
    switch (data_bits)
    {
    case 8:
        opt.c_cflag |= CS8;
        break;
    case 7:
        opt.c_cflag |= CS7;
        break;
    default:
        error_ret("Unsupported data bits.");
    }

    /* parity bits */
    switch (parity)
    {
    case 'N':
    case 'n':
        opt.c_cflag &= ~PARENB;
        opt.c_cflag &= ~INPCK; /* ?? */
        break;
    case 'O':
    case 'o':
        opt.c_cflag|=(INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= (PARODD | PARENB);
        break;
    case 'E':
    case 'e':
        opt.c_cflag|=(INPCK|ISTRIP); /*enable parity check, strip parity bits*/
        opt.c_cflag |= PARENB;
        opt.c_cflag &= ~PARODD;
        break;
    default:
        error_ret("Unsupported parity bits.");
    }

    /* stop bits */
    switch (stop_bits)
    {
    case 1:
        opt.c_cflag &= ~CSTOPB;
        break;
    case 2:
        opt.c_cflag |= CSTOPB;
        break;
    default:
        error_ret("Unsupported stop bits.");
    }

    /* raw input */
    opt.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
    /* raw ouput */
    opt.c_oflag &= ~OPOST;

    tcflush(fd, TCIFLUSH);
    opt.c_cc[VTIME] = 0; /* no time out */
    opt.c_cc[VMIN] = 0; /* minimum number of characters to read */

    ret = tcsetattr(fd, TCSANOW, &opt); /* update it now */
    if (ret < 0)
        unix_error_ret("Unable to setup the port.");
    debug_msg("Setup the port OK!\n");

    return 0; /* everything is OK! */
}
