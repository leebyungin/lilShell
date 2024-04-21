#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <execinfo.h>
#include <string.h>

#define OPTION_DEFAULT 0
#define OPTION_LOG 1

#define BUF_SIZE 1024
#define TRACE_BUF_SIZE 10

static void doCat(const char *path);
static void errorHandler(const char *s);
static void addOption(const char* opt);
static int isOption(const char *opt);

static int OPTION= OPTION_DEFAULT;

int main(int argc, const char* argv[])
{

	if(argc < 2)
	{
		fprintf(stderr, "Usage: %s [option] <file names>\n",argv[0]);
		exit(1);
	}

	for(int i=1; i<argc; i++)
	{
		if(isOption(argv[i]) > 0)
		{
			addOption(argv[i]);
			continue;
		}

		doCat(argv[i]);
	}

	return 0;
}

static int isOption(const char *opt)
{
	if(opt[0] != '-')
	{
		return 0;
	}

	if(strcmp(opt, "-l") == 0)
	{
		return 1;
	}

	fprintf(stderr,"invalid option : %s\n",opt);
	exit(1);
}

static void addOption(const char * opt)
{
	if(strcmp(opt, "-l") == 0)
	{
		OPTION |= OPTION_LOG;
	}
}

static void doCat(const char *path)
{
	int fd;
	unsigned char buf[BUF_SIZE];
	ssize_t size;

	fd= open(path,O_RDONLY);
	if(fd == -1)
	{
		errorHandler(path);
	}


	// while(size > 0)으로 설정하고 밖에서 if(size == -1)을 하면 더 효율적일 거 같지만, while과 if 사이에
	// 새로운 코드가 추가되면 문제가 발생할 수 있음. 이정도는 컴파일러가 알아서 최적화 하지 않을까?
	while( (size= read(fd, buf, sizeof(buf)-1) ) != 0)
	{
		if(size < 0)
		{
			errorHandler(path);
		}

		buf[size]= 0;
		if( write(STDOUT_FILENO, buf, size) < 0)
		{
			errorHandler(path);
		}
	}

	close(fd);
}

static void errorHandler(const char *s)
{
	void*callStack[TRACE_BUF_SIZE];
	char **buf;
	int bCount;

	perror(s);

	if( OPTION & OPTION_LOG)
	{
		bCount= backtrace(callStack,TRACE_BUF_SIZE);
		buf= backtrace_symbols(callStack, bCount);

		for(int i=0; i<bCount; i++)
		{
			fprintf(stderr, "%s\n",buf[i]);
		}
		free(buf);
	}

	exit(1);
}
