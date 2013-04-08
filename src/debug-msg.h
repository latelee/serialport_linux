/**
 *                           _/_/     _/_/_/_/   _/_/_/_/_/
 *                        _/    _/  _/              _/
 *                       _/         _/             _/
 *                      _/            _/_/        _/
 *                     _/                 _/     _/
 *                     _/    _/    _/     _/    _/
 *                     _/_/_/      _/_/_/_/  _/_/_/
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
 * @file   debug-msg.h
 * @author Late Lee
 * @date   2010
 * @brief  Some macros(debug, error handle) usually used in my programs.
 *
 * @test   I only use this file on linux(with x86 & arm) platform.
 */

#ifndef _DEBUG_MSG_H
#define _DEBUG_MSG_H

#include <stdio.h>	/* perror() */
#include <string.h>	/* strerror() */
#include <errno.h>	/* errno */

/***
 * Print the error msg is one thing
 * but print the msg is another!!
 */

/**
 * @def error_exit
 * @brief A macro that prints the @a error msg and exit.
 */

#define error_exit(error)	\
    do{                                         \
        fprintf(stderr, "%s\n", error);         \
        exit(0);                                \
    } while(0)

/**
 * @def error_ret
 * @brief A macro that prints the @a error msg and return -1.
 */
#define error_ret(error)                          \
    do{                                           \
        fprintf(stderr, "%s\n", error);           \
        return -1;                                \
    } while(0)

/**
 * @def unix_error_exit
 * @brief A macro that prints the @a error msg(with errno) and then exit.
 *        I put 'unix' before the 'function' name because I am using 'errno'.
 */
#define unix_error_exit(error)                  \
    do{                                         \
        fprintf(stderr, "%s Info[%d]:%s\n",     \
                error, errno, strerror(errno));	\
        exit(1);                                \
    } while(0)

/**
 * @def unix_error_ret
 * @brief A macro that prints the @a error msg(with errno) and then return -1.
 *        I put 'unix' before the 'function' name because I am using 'errno'.
 */
#define unix_error_ret(error)                   \
    do{                                         \
        fprintf(stderr, "%s Info[%d]:%s\n",     \
                error, errno, strerror(errno));	\
        return -1;                              \
    } while(0)

/* error handle ,detail version */
/* it can show the file,function and the line */
/**
 * @def unix_print_error
 * @brief A macro that prints the @a error msg(with errno) and then exit.
 *        I put 'unix' before the 'function' name because I am using 'errno'.@n
 *        This error handle(detail version) can show the @p file, @p function
 *        and @p line where the @a error happens.
 * @note  I do not often use this 'function' in my program.
 */
#define unix_print_error(error)                                 \
    do {                                                        \
        fprintf(stderr, "Oh God!\nFile:%s Line:%d Function:%s:\n",  \
                __FILE__, __LINE__, __func__);                      \
        perror(error);                                              \
        exit(0);                                                    \
    } while(0)

/* DEBUG=0  will show nothing */
#ifndef DEBUG
#define DEBUG	1               /**< we debug all the time */
#endif

/**
 * @def debug_msg
 * @brief A macro that prints the debug msg, as the same with @a printf.
 *
 */
#ifdef DEBUG
#define debug_msg(fmt, ...)	\
    fprintf(stdout, fmt, ##__VA_ARGS__)
#else
#define debug_msg(fmt,...)
#endif	 /* DEBUG */

#ifndef TRACE
#define TRACE	1               /**< we trace all the time */
#endif

/**
 * @def debug_trace
 * @brief A macro that traces the @p file, @p function and @p line.
 *
 */
#ifdef TRACE
#define debug_trace(trace)	\
    fprintf(stdout, "%s File:%s Line:%d Func:%s.\n",   \
            trace, __FILE__, __LINE__, __func__)
#else
#define debug_trace(trace)
#endif

#endif	 /* _DEBUG_MSG_H */
