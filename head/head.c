#include <stdio.h>
#include <stdlib.h>

#include <error_handler.h>

static void do_head(FILE *file, const char *str);
static void _do_head(FILE *file, int n_lines);

int main(int argc, char* argv[])
{
	if(argc <= 1)
	{
		printf("Usage: %s n\n",argv[0]);
		exit(1);
	}

	switch(argc)
	{
		case 2:
			do_head(stdin, argv[1]);
			break;
		default :
			errno=0;
			for(int i=2; i<argc; i++)
			{
				FILE *fp= fopen(argv[i], "r");
				if(fp == NULL)
				{
					perror_handler(argv[i],1);
				}
				do_head(fp, argv[1]);
				fclose(fp);
			}
			break;
	}

	return 0;
}


static void do_head(FILE *file, const char *str)
{
	errno= 0;
	int n_lines= strtol(str, NULL, 10);
	if(errno == ERANGE)
	{
		perror_handler("do_head:strtol()",0);
		exit(1);
	}

	_do_head(file, n_lines);
}
static void _do_head(FILE *file, int n_lines)
{
	int c=0;
	errno= 0;

	while((c = fgetc(file)) != EOF)
	{
		if((fputc(c, stdout)) ==  EOF)
		{
			perror_handler("_do_head:fputc()",0);
			exit(1);
		}
		if(c == '\n')
		{
			if(--n_lines <= 0)
			{
				break;
			}
		}
	}

	return;
}
