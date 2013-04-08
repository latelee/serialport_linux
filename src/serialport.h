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
 * @file   serialport.h
 * @author Late Lee
 * @date   Mon Jan 10 2011
 * 
 * @brief  Some utils of the serial port, such as open the port, close
 *         the port and setup the port.
 */

#ifndef SERIALPORT_H
#define SERIALPORT_H

int open_port(const char* device);
int close_port(int fd);
int setup_port(int fd, int speed, int data_bits, int parity, int stop_bits);

#endif /* SERIALPORT_H */
