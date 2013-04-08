#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>	/* exit */
#include <string.h>	/* str* */
#include <errno.h>	/* errno */
#include <stdarg.h>	/* va_* */
#include <syslog.h>	/* *log */
#include "error-log.h"

static void error_doit(int errno_flag, const char *fmt, va_list ap)
{
	int errno_save;
	char buf[MAXLINE];

	errno_save = errno;
	vsprintf(buf, fmt, ap);
	if (errno_flag)
		sprintf(buf + strlen(buf), ": [%d]%s.", errno_save, 
		strerror(errno_save));
	strcat(buf, "\n");
	fflush(stdout);
	fputs(buf, stderr);
	fflush(NULL);
	return;
}

void error_exit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_doit(0, fmt, ap);
	va_end(ap);
	exit(1);
}

void unix_error_exit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

void error_dump(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_doit(1, fmt, ap);
	va_end(ap);
	abort();
	exit(1);
}

void error_msg(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_doit(0, fmt, ap);
	va_end(ap);
	return;
}

void error_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	error_doit(1, fmt, ap);
	va_end(ap);
	exit(1);
}

int debug = 1;
/* daemon */
static void log_doit(int errno_flag, int priority, const char *fmt, va_list ap)
{
	int errno_save;
	char buf[MAXLINE];

	errno_save = errno;
	vsprintf(buf, fmt, ap);
	if (errno_flag)
		sprintf(buf+strlen(buf), ": [%d]%s.", errno_save, 
		strerror(errno_save));
	strcat(buf, "\n");
	if (debug)
	{
		fflush(stdout);
		fputs(buf, stderr);
		fflush(stderr);
	}
	else
		syslog(priority, "%s", buf); /* fix gcc warning... */
	return;
}

void log_open(const char *ident, int option, int facility)
{
	if (debug == 0)
		open(ident, option, facility);
	return;
}

/* optional */
void log_close(void)
{
	if (debug == 0)
		closelog();
}
void log_sys(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(1, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}

void log_msg(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	return;
}

void log_quit(const char *fmt, ...)
{
	va_list ap;
	va_start(ap, fmt);
	log_doit(0, LOG_ERR, fmt, ap);
	va_end(ap);
	exit(2);
}
