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
 * @file   main.c
 * @author Late Lee
 * @date   Mon Jan 10 2011
 * 
 * @brief  A simple test of serial port writing data to the port.
 * @test   To compile the program, type <tt> make </tt>,
 *         then it will generate the executable binary @p a.out.
 *         To run it, type <tt> ./a.out</tt>.Make sure you have the permission
 *         to open the serial port.@n
 *         You can start a ternimal and type @p minicom(also make sure you can
 *         open the device). When everything is OK, you can see the data in 
 *         minicom using the same port if you connect pin 2 & pin 3 of the port
 *         (the male connector, see the picture below)(but I don't know the
 *         reason, isn't it blocked?).@n
 *         If you want to stop the program, just use 'Ctrl+c' or 'q'. @n
 *         In default, the program open the device '/dev/ttyUSB0'.@n
 *         Here are snapshots of the program:
 *         @image html serial-write.jpg "Writing to the port..."
 *         @image html serial-read-minicom.jpg "Reading the data in minicom..."
 *         @image html send_revc_self.jpg "One thread send, one thread read."
 *         @n And here comes the serial port connector:
 *         @image html com-male.jpg "A 9-pin male D-Sub connector(*)"
 *         @image html com-female.jpg "A 9-pin female D-Sub connector"
 *         (*)We use this connector in the test.
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
//#include "debug-msg.h"
#include "serialport.h"
#include "error-log.h"

pthread_t write_tid; /**< write thread */
pthread_t read_tid;  /**< read thread */
pthread_t exit_tid;  /**< exit thread */

/** The data we write to the port. */
//char *buf = "Are you going to die?\r\n";
char *buf = "Are you going to die? To be or not to be, that is the question.\r\n";
/** data we receive */
char tmp[512];

/** 
 * write_port_thread - A thread that writes data to the port
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 * @note
 * This is only a test, not the @e real one.
 */
void *write_port_thread(void *argc)
{
    int ret;
    int fd;
    static int cnt = 1;
    char send_buf[512] = {0};

    fd = (int)argc;

    while (1)
    {
        sprintf(send_buf, "%d %s", cnt, buf);
        debug_msg("writing time %d... ", cnt++);

        ret = write(fd, send_buf, strlen(send_buf));

        if (ret < 0)
            pthread_exit(NULL);
        debug_msg("write num---: %d\n", ret);
        sleep(2);
    }
    pthread_exit(NULL);
}

/** 
 * read_port_thread - Thread that reads data from the port
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 */
void *read_port_thread(void *argc)
{
    int num;
    int fd;
    
    fd = (int)argc;
    while (1)
    {
        while ((num = read(fd, tmp, 512)) > 0)
        {
            debug_msg("read num: %d\n", num);
            tmp[num+1] = '\0';
            printf("%s\n", tmp);
        }
        sleep(1);
        if (num < 0)
            pthread_exit(NULL);
	}
    pthread_exit(NULL);
}

/** 
 * sig_handle - Handle the INT signal.
 * 
 * @param sig_num : The signal.
 * @note
 * This function is not used.
 */
void sig_handle(int sig_num)
{
    debug_msg("catch signal %d\n", sig_num);
    exit(0);
}

/** 
 * exit_thread - Thread that exit the program when 'q' pressed
 * 
 * @param argc : Here means the port(specified by the fd).
 * 
 */
void* exit_thread(void* argc)
{
    while (1)
    {
        int c = getchar();
        if ('q' == c)
        {
            printf("You exit, not myself.\n");
            exit(0);
        }
        sleep(1);
    }
    pthread_exit(NULL);
}

/** 
 * main - main function
 * 
 */
int main(int argc, char* argv[])
{
    int fd;
    int ret;
	char dev_name[32] = {0};
    
    strcpy(dev_name, "/dev/ttyS0");
    if (argc == 2)
    {
        sprintf(dev_name,"%s",argv[1]);
    }

    //signal(SIGINT, sig_handle);
    fd = open_port(dev_name);          /* open the port */
    if (fd < 0)
	{
		printf("open %s err\n",dev_name);
		exit(0);
	}
        
    ret = setup_port(fd, 115200, 8, 'N', 1);  /* setup the port */
    if (ret<0)
        exit(0);

    ret = pthread_create(&write_tid, NULL, write_port_thread, (void*)fd);
    if (ret < 0)
        unix_error_exit("Create write thread error.");

    ret = pthread_create(&read_tid, NULL, read_port_thread, (void*)fd);
    if (ret < 0)
        unix_error_exit("Create read thread error.");

    ret = pthread_create(&exit_tid, NULL, exit_thread, NULL);
    if (ret < 0)
        unix_error_exit("Create exit thread error.");

    pthread_join(write_tid, NULL);
    pthread_join(read_tid, NULL);
    pthread_join(exit_tid, NULL);

    close_port(fd);

    return 0;
}
