#ifndef __ERROR_HANDLER_H__
#define __ERROR_HANDLER_H__

#include <error_handler.h>

void perror_handler(const char *msg, int bt)
{
	perror(msg);
	if(bt)
	{
		error_backtrace();
	}

	exit(1);
}

void error_handler(const char *msg, int bt)
{
	fprintf(stderr, "%s\n",  msg);
	if(bt)
	{
		error_backtrace();
	}

	exit(1);
}

void error_backtrace(void)
{
	void* buffer[ERROR_HANDLER_BUF_SIZE];
	int size=0;

	size= backtrace(buffer, ERROR_HANDLER_BUF_SIZE);
	backtrace_symbols_fd(buffer, size, STDERR_FILENO);
	return;
}

#endif
