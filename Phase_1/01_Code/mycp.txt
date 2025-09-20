#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#define COUNT 100

int main(int argc, char* argv[])
{
	char buff[COUNT];
	int numread;

	if (argc != 3)
	{
		printf("Usage: %s file-name\n", argv[0]);
		exit(-1);
	}

	int fd_1 = open(argv[1], O_RDONLY);
	int fd_2 = open(argv[2], O_RDWR | O_CREAT, 0644);
	if (fd_1 < 0)
	{
		printf("Couldn't open a file \n");
		exit(-2);
	}

	if (fd_2 < 0)
	{
		printf("Couldn't create a file \n");
                exit(-4);

	}

	while((numread = read(fd_1, buff, COUNT)) > 0)
	{
		if (write(fd_2, buff, numread) < 0)
		{
			printf("write failed\n");
			exit(-3);
		}

	}
	close(fd_1);
	close(fd_2);
	
	return 0;
}
