#ifndef _ERROR_LOG_H
#define _ERROR_LOG_H

#define MAXLINE 4096

extern int debug;

#define DEBUG
#ifdef DEBUG
#define debug_msg(fmt, ...)	\
    fprintf(stdout, fmt, ##__VA_ARGS__)
#else
#define debug_msg(fmt,...)
#endif	 /* DEBUG */

#define TRACE

#ifdef TRACE
#define debug_trace(trace)	\
    fprintf(stdout, "%s ===File:%s----- Func:%s -----Line:%d===\n",   \
            trace, __FILE__, __func__, __LINE__)
#else
#define debug_trace(trace)
#endif	/* TRACE */

void error_exit(const char *fmt, ...);
void unix_error_exit(const char *fmt, ...);
void error_dump(const char *fmt, ...);
void error_msg(const char *fmt, ...);
void error_sys(const char *fmt, ...);

void log_open(const char *ident, int option, int facility);
void log_close(void);
void log_sys(const char *fmt, ...);
void log_msg(const char *fmt, ...);
void log_quit(const char *fmt, ...);

#endif
