#include <fcntl.h> /* lib for open (man 2 open) */
#include <stdio.h> /* for printf */
#include <stdlib.h>
#include <unistd.h> /*for read*/ /*man 2 read */
#include <unistd.h>

#define count 100
int main(int argc, char *argv[])
{
  char buf[count];
  int numread;

  if (argc != 2)
  {
    printf("Usage: %s file-name\n", argv[0]);
    exit(-1);
  }

  // int fd = open("/etc/passwd", O_RDONLY);
  int fd = open(argv[1], O_RDONLY);

  if (fd < -1) /* negative means error */ /* it's important to check return val */
  {
    printf("couldn't open a file \n ");
    exit(-2);
  }

  while ((numread = read(fd, buf, count)) > 0)
  {
    if (write(1, buf, numread) < 0)
    {
      printf("write failed\n");
      exit(-3);
    }
  }
  close(fd);

  return 0;
}
