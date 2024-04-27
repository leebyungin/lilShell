#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <execinfo.h>
#include <errno.h>

#define ERROR_HANDLER_BUF_SIZE 15

void perror_handler(const char *msg, int bt);
void error_handler(const char* msg, int bt);
void error_backtrace(void);
