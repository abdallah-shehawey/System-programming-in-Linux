#include <stdio.h>

extern char **environ;

int main (int argc, char** argv)
{
	int i ;
	printf("argc = %d\n", argc);
	for (i = 0; i < argc; i++)
	{
		printf("argv[%d] = %s\n", i, argv[i]);
	}
	for (i = 0; environ[i] != NULL; i++)
	{
		printf("env[%d]: %s \n", i, environ[i]);
	}
	return 0;
}	
